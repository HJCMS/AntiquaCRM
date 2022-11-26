// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooks.h"
#include "abebooksdocument.h"

/** @brief Konfigurationsgruppe */
#ifndef ABEBOOKS_CONFIG_GROUP
#define ABEBOOKS_CONFIG_GROUP "provider/abebooks"
#endif

/** @brief Wird für Menüeintrag und Gruppenzuweisung benötigt! */
#ifndef ABEBOOKS_CONFIG_PROVIDER
#define ABEBOOKS_CONFIG_PROVIDER "AbeBooks"
#endif

#include <AntiquaCRM>
#include <QDebug>
#include <QTextCodec>
#include <QTextEncoder>
#include <QTimer>

static const QString to_iso8859_1(const QString &str) {
  QTextCodec *codec = QTextCodec::codecForLocale();
  QTextEncoder encoder(codec);
  return QString(encoder.fromUnicode(str));
}

Abebooks::Abebooks(QObject *parent) : AntiquaCRM::APluginInterface{parent} {
  setObjectName(ABEBOOKS_CONFIG_PROVIDER);
  m_network = nullptr;
}

bool Abebooks::initConfigurations() {
  QUrl url;
  AntiquaCRM::ASettings cfg(this);
  cfg.beginGroup(ABEBOOKS_CONFIG_GROUP);
  url.setScheme("https");
  url.setHost(cfg.value("api_host", "orderupdate.abebooks.com").toString());
  url.setPort(cfg.value("api_port", 10003).toInt());
  apiUser = cfg.value("api_user", QString()).toString();
  apiKey = cfg.value("api_key", QString()).toString();
  historyCall = cfg.value("api_history_call", -7).toInt();
  cfg.endGroup();
  apiUrl = url;
  return true;
}

AbeBooksDocument Abebooks::initDocument() {
  AbeBooksAccess ac;
  ac.user = apiUser;
  ac.key = apiKey;
  return AbeBooksDocument(ac);
}

const QUrl Abebooks::apiQuery(const QString &section) {
  QString action = configProvider().toLower();
  action.append("_");
  action.append(section.toLower());
  action.append(".xml");

  actionsCookie = QNetworkCookie("action", action.toLocal8Bit());
  actionsCookie.setDomain(apiUrl.host());
  actionsCookie.setSecure(true);

  return apiUrl;
}

const QString Abebooks::dateString(const QDate &date) const {
  return date.toString(Qt::ISODate);
}

const QVariant Abebooks::createValue(QMetaType::Type id,
                                     const QJsonValue &value) const {
  // Note bei XML kommt alles als String an
  QString str = value.toString();
  switch (id) {
  case QMetaType::Bool:
    return (str == "true") ? true : false;

  case QMetaType::Int:
  case QMetaType::Long:
  case QMetaType::LongLong:
    return str.toInt();

  case QMetaType::QDateTime:
    return QDateTime::fromString(str, Qt::ISODate);

  case QMetaType::Double:
    return str.toDouble();

  default:
    return str;
  }
}

void Abebooks::setOrderItem(AntiquaCRM::AProviderOrder *order,
                            const QString &key, const QJsonValue &value) const {
  QHashIterator<QString, QMetaType::Type> it(order->orderEditKeys());
  while (it.hasNext()) {
    it.next();
    if (it.key() == key) {
      order->setValue(it.key(), createValue(it.value(), value));
      break;
    }
  }
}

const AntiquaCRM::ArticleOrderItem
Abebooks::setArticleItem(AntiquaCRM::AProviderOrder *order, const QString &key,
                         const QJsonValue &value) const {
  AntiquaCRM::ArticleOrderItem item;
  QHashIterator<QString, QMetaType::Type> it(order->articleKeys());
  while (it.hasNext()) {
    it.next();
    if (it.key() == key) {
      item.key = key;
      if (key.contains("price")) {
        item.value = getPrice(value);
      } else if (value.type() == QJsonValue::Double) {
        item.value = (qint64)value.toDouble();
      } else if (key == "a_provider_id") {
        item.value = value.toString();
      } else {
        item.value = createValue(it.value(), value);
      }
      break;
    }
  }
  return item;
}

void Abebooks::prepareResponse(const QJsonDocument &js) { Q_UNUSED(js); }

void Abebooks::prepareResponse(const QDomDocument &xml) {
  QString fileName = configProvider();
  if (!actionsCookie.name().isNull()) {
    fileName = QString(actionsCookie.value());
  }

  AntiquaCRM::ASharedCacheFiles cacheFile;
  cacheFile.storeTempFile(fileName.toLower(), xml.toString(1));

  emit sendQueryFinished();
}

void Abebooks::queryFinished(QNetworkReply *reply) {
  if (reply->error() != QNetworkReply::NoError) {
    emit sendErrorResponse(AntiquaCRM::WARNING,
                           tr("AbeBooks response with errors!"));
  }
}

void Abebooks::queryNewOrders(int waitSecs) {
  Q_UNUSED(waitSecs);
  QString operation("getAllNewOrders");
  AbeBooksDocument doc = initDocument();
  doc.createAction(operation);

  QUrl url(apiQuery(operation));
  m_network->xmlPostRequest(url, doc);
}

void Abebooks::queryOrder(const QString &orderId) {
  QString operation("getOrder");
  AbeBooksDocument doc = initDocument();
  doc.createAction(operation);
  QDomElement e = doc.createElement("purchaseOrder");
  e.setAttribute("id", orderId.trimmed());
  doc.documentElement().appendChild(e);

  QUrl url(apiQuery(operation));
  m_network->xmlPostRequest(url, doc);
}

void Abebooks::orderUpdateAction(const QJsonObject &options) {
  if (!options.contains("provider")) {
    qWarning("Invalid caller for Abebooks::postOperation!");
    return;
  }
  // Prüfen ob es für ihn ist!
  if (options.value("provider").toString() != configProvider())
    return;

  qDebug() << Q_FUNC_INFO << options;
}

const QString Abebooks::configProvider() const {
  return QString(ABEBOOKS_CONFIG_PROVIDER).toLower();
}

const QString Abebooks::displayName() const {
  return QString(ABEBOOKS_CONFIG_PROVIDER).trimmed();
}

const AntiquaCRM::AProviderOrders Abebooks::getOrders() const {
  AntiquaCRM::AProviderOrders allOrders;
  QString fileName = configProvider();
  if (!actionsCookie.name().isNull()) {
    fileName = QString(actionsCookie.value());
  }

  if (fileName.isEmpty())
    return allOrders;

  AntiquaCRM::ASharedCacheFiles cacheFile;
  QString data = cacheFile.getTempFile(fileName.toLower());
  // QString data = cacheFile.getTempFile("abebooks_getallneworders_test.xml");
  if (data.isEmpty())
    return allOrders;

  QString display_name = displayName().toLower();
  QDomDocument doc(display_name);
  QString errorMsg;
  if (!doc.setContent(data, false, &errorMsg)) {
    qWarning("AbeBooks orderdata parse Error:'%s'", qPrintable(errorMsg));
    return allOrders;
  }

  AbeBooksDocument xml(doc);
  if (doc.documentElement().tagName() == "requestError") {
    QPair<int, QString> err = xml.errorResponseCode();
    qWarning("AbeBooks Request Error %d (%s)", err.first,
             qPrintable(err.second));
    return allOrders;
  }

  // purchaseOrderList
  QDomNodeList purchaseOrders = xml.getPurchaseOrderList();
  for (int i = 0; i < purchaseOrders.count(); i++) {
    // purchaseOrder::Node @{
    QDomNode orderNode = purchaseOrders.at(i);
    QDomElement orderElement = orderNode.toElement();
    QString strOrderId = orderNode.toElement().attribute("id", "0");
    qint64 orderId = strOrderId.toInt();
    // @}
    QDateTime dateTime = xml.getOrderDate(orderElement);
    // Start fill
    AntiquaCRM::AProviderOrder order(display_name, strOrderId);
    order.setValue("o_provider_name", displayName());
    order.setValue("o_provider_order_id", strOrderId);
    order.setValue("o_provider_purchase_id", orderId);
    order.setValue("o_since", dateTime);
    order.setValue("o_media_type", AntiquaCRM::BOOK);

    /*
     * @brief Konvertiere Provider PaymentStatus => OrderStatus!
     * Wir verwenden im Auftragssystem nur den OrderStatus!
     * Der PaymentStatus ist eine reine Dienstleistergeschichte!
     */
    QDomNode statusNode = xml.firstChildNode(orderElement, "status");
    QString orderStatus = xml.getNodeValue(statusNode).toString();
    if (orderStatus.contains("Ordered")) {
      // AntiquaCRM::SHIPMENT_CREATED => AntiquaCRM::STARTED
      order.setValue("o_order_status", AntiquaCRM::OrderStatus::STARTED);
    } else if (orderStatus.contains("Cancelled")) {
      // AntiquaCRM::ORDER_CANCELED => AntiquaCRM::CANCELED
      order.setValue("o_order_status", AntiquaCRM::OrderStatus::CANCELED);
    } else {
      // AntiquaCRM::STATUS_NOT_SET => AntiquaCRM::OPEN
      order.setValue("o_order_status", AntiquaCRM::OrderStatus::OPEN);
    }

    // AntiquaCRM::PaymentMethod
    AntiquaCRM::PaymentMethod payment_method;
    payment_method = AntiquaCRM::PAYMENT_NOT_SET;
    QDomNode pmNode = xml.firstChildNode(orderElement, "purchaseMethod");
    QString purchaseMethod = xml.getNodeValue(pmNode).toString();
    if (purchaseMethod.toUpper() == "CC") {
      // AbeBooks Creditcard
      payment_method = AntiquaCRM::CREDIT_CARD_PREPAYMENT;
    } else if (purchaseMethod.toUpper() == "SD") {
      // Seller Direct
      QString sdType = pmNode.toElement().attribute("type", "Invoice");
      if (sdType.contains("Check"))
        payment_method = AntiquaCRM::CREDIT_CARD_PREPAYMENT;
      else if (sdType.contains("PayPal"))
        payment_method = AntiquaCRM::PAYPAL_PREPAYMENT;
      else if (sdType.contains("Debit"))
        payment_method = AntiquaCRM::DIRECT_DEBIT_PREPAYMENT;
      else if (sdType.contains("Bank"))
        payment_method = AntiquaCRM::BANK_PREPAYMENT;
      else if (sdType.contains("Invoice"))
        payment_method = AntiquaCRM::DELIVER_WITH_INVOICE;
      else if (sdType.contains("Money Order"))
        payment_method = AntiquaCRM::UNKNOWN_PREPAYMENT;
    }
    order.setValue("o_payment_method", payment_method);
    order.setValue("o_payment_status", false);
    order.setValue("o_delivery_add_price", false);

    // Buyer info
    QDomNodeList buyerNodeList = orderElement.elementsByTagName("buyer");
    if (buyerNodeList.size() > 0) {
      QDomElement buyerNode = buyerNodeList.at(0).toElement();
      // Postaldata
      QDomNode addressNode = buyerNode.namedItem("mailingAddress");
      QPair<QString, QString> person = xml.getPerson(addressNode);
      order.setValue("c_provider_import", xml.getFullname(addressNode));
      order.setValue("c_gender", AntiquaCRM::Gender::NO_GENDER);
      order.setValue("c_firstname", person.first);
      order.setValue("c_lastname", person.second);
      order.setValue("c_street", xml.getStreet(addressNode));
      order.setValue("c_postalcode", xml.getPostalCode(addressNode).toString());
      order.setValue("c_location", xml.getLocation(addressNode));
      QString p_country = xml.getCountry(addressNode);
      if (!p_country.isEmpty()) {
        order.setValue("c_country_bcp47", bcp47Country(p_country));
        order.setValue("o_vat_country", bcp47Country(p_country));
        AntiquaCRM::AEuropeanCountries EUCountries;
        QString c = EUCountries.value(bcp47Country(p_country).toUpper());
        order.setValue("c_country", (c.isEmpty() ? p_country : c));
      }
      order.setValue("c_phone_0", xml.getPhone(addressNode));
      order.setValue("c_email_0", xml.getEMail(buyerNode));
      // Address Bodies
      QStringList buffer;
      buffer << xml.getFullname(addressNode);
      buffer << xml.getStreet(addressNode);
      buffer << xml.getPostalCode(addressNode).toString() + " " +
                    xml.getLocation(addressNode);
      order.setValue("c_postal_address", buffer.join("\n"));
      order.setValue("c_shipping_address", buffer.join("\n"));
    }
    // Article Orders - QDomNode::purchaseOrderItemList
    QDomNodeList orderItems = xml.getOrderItemList(orderElement);
    if (orderItems.size() > 0) {
      for (int i = 0; i < orderItems.size(); i++) {
        // QDomNode::purchaseOrderItem
        QDomElement orderItemNode = orderItems.at(i).toElement();
        AntiquaCRM::OrderArticleItems orderItem;
        // QDomNode::book
        QDomNode book = xml.firstChildNode(orderItemNode, "book");
        if (!book.isNull()) {
          QJsonValue jvalue;
          QDomElement bookElement = book.toElement();
          // Article Type
          order.setValue("o_media_type", AntiquaCRM::BOOK);
          jvalue = QJsonValue(AntiquaCRM::BOOK);
          orderItem.append(setArticleItem(&order, "a_type", jvalue));
          // Article title
          jvalue = QJsonValue(xml.getTagText(bookElement, "title"));
          orderItem.append(setArticleItem(&order, "a_title", jvalue));
          // Provider Order ID
          jvalue = QJsonValue(bookElement.attribute("id", strOrderId));
          orderItem.append(setArticleItem(&order, "a_provider_id", jvalue));
          // Article ID
          QDomNode queryNode = xml.firstChildNode(bookElement, "vendorKey");
          jvalue = QJsonValue(xml.getNodeValue(queryNode).toInt());
          orderItem.append(setArticleItem(&order, "a_article_id", jvalue));
          // Article Count
          orderItem.append(setArticleItem(&order, "a_count", QJsonValue(1)));
          orderItem.append(setArticleItem(&order, "a_payment_id", 0));
          // Article Price
          queryNode = xml.firstChildNode(bookElement, "price");
          jvalue = QJsonValue(xml.getNodeValue(queryNode).toDouble());
          orderItem.append(setArticleItem(&order, "a_price", jvalue));
          // Article Sell Price (VK)
          queryNode = xml.firstChildNode(bookElement, "price");
          jvalue = QJsonValue(xml.getNodeValue(queryNode).toDouble());
          orderItem.append(setArticleItem(&order, "a_sell_price", jvalue));
        }
        order.insertOrderItems(orderItem);
      }
    }

    allOrders.append(order);
  }

  //#ifdef ANTIQUA_DEVELOPEMENT
  //  QListIterator<AntiquaCRM::AProviderOrder> get_orders(allOrders);
  //  while (get_orders.hasNext()) {
  //    AntiquaCRM::AProviderOrder data = get_orders.next();
  //    qDebug() << data.provider() << data.id() << data.orders().size();
  //    foreach (QString k, data.filledKeys()) {
  //      qDebug() << k << data.getValue(k);
  //    }
  //  }
  //#endif
  return allOrders;
}

bool Abebooks::createInterface(QObject *parent) {
  Q_UNUSED(parent);
  m_network = new AntiquaCRM::ANetworker(AntiquaCRM::XML_QUERY, this);
  connect(m_network, SIGNAL(sendXmlResponse(const QDomDocument &)),
          SLOT(prepareResponse(const QDomDocument &)));
  connect(m_network, SIGNAL(sendContentCodec(QTextCodec *)),
          SLOT(setContentDecoder(QTextCodec *)));
  connect(m_network, SIGNAL(finished(QNetworkReply *)),
          SLOT(queryFinished(QNetworkReply *)));

  return initConfigurations();
}
