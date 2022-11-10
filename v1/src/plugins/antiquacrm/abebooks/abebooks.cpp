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

const AntiquaCRM::ArticleOrderItem
Abebooks::articleItem(const QString &key, const QJsonValue &value) const {
  QHash<QString, QMetaType::Type> keys =
      AntiquaCRM::AProviderOrder::articleKeys();
  if (!keys.contains(key)) {
    qWarning("AbeBooks: Unknown Article Key(%s)!", qPrintable(key));
    return AntiquaCRM::ArticleOrderItem();
  }

  QVariant _value;
  if (key == "a_article_id") {
    // Artikel Nummer
    _value = value.toInt();
  } else if (key == "a_provider_id") {
    // Dienstleister Bestellnummer
    _value = QString::number(value.toInt());
  } else if (key.contains("_price")) {
    // Preise
    if (value.type() == QJsonValue::String) {
      _value = QString(value.toString()).toDouble();
    } else if (value.type() == QJsonValue::Double) {
      _value = value.toDouble();
    }
  } else if (keys.value(key) == QMetaType::Int) {
    _value = value.toInt();
  } else if (keys.value(key) == QMetaType::QString) {
    _value = value.toString();
  }

  AntiquaCRM::ArticleOrderItem item;
  item.key = key;
  item.value = _value;
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

  QDomNodeList purchaseOrders = xml.getPurchaseOrderList();
  for (int i = 0; i < purchaseOrders.count(); i++) {
    QDomNode orderNode = purchaseOrders.at(i);
    QDomElement orderElement = orderNode.toElement();
    QString strOrderId = orderNode.toElement().attribute("id", "0");
    qint64 orderId = strOrderId.toInt();
    QDateTime dateTime = xml.getOrderDate(orderElement);
    // Start fill
    AntiquaCRM::AProviderOrder item(display_name, strOrderId);
    item.setValue("o_provider_name", displayName());
    item.setValue("o_provider_order_id", strOrderId);
    item.setValue("o_provider_purchase_id", orderId);
    item.setValue("o_since", dateTime);
    item.setValue("o_media_type", AntiquaCRM::BOOK);

    // AntiquaCRM::PaymentStatus
    QDomNode statusNode = xml.firstChildNode(orderElement, "status");
    QString orderStatus = xml.getNodeValue(statusNode).toString();
    if (orderStatus.contains("Ordered")) {
      item.setValue("o_order_status", AntiquaCRM::SHIPMENT_CREATED);
    } else if (orderStatus.contains("Shipped")) {
      item.setValue("o_order_status", AntiquaCRM::WAIT_FOR_PAYMENT);
    } else if (orderStatus.contains("Cancelled")) {
      item.setValue("o_order_status", AntiquaCRM::ORDER_CANCELED);
    } else {
      item.setValue("o_order_status", AntiquaCRM::STATUS_NOT_SET);
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
    item.setValue("o_payment_method", payment_method);
    item.setValue("o_payment_status", false);
    item.setValue("o_delivery_add_price", false);

    // Buyer info
    QDomNodeList buyerNodeList = orderElement.elementsByTagName("buyer");
    if (buyerNodeList.size() > 0) {
      QDomElement buyerNode = buyerNodeList.at(0).toElement();
      // Postaldata
      QDomNode addressNode = buyerNode.namedItem("mailingAddress");
      QPair<QString, QString> person = xml.getPerson(addressNode);
      item.setValue("c_provider_import", xml.getFullname(addressNode));
      item.setValue("c_gender", AntiquaCRM::Gender::NO_GENDER);
      item.setValue("c_firstname", person.first);
      item.setValue("c_lastname", person.second);
      item.setValue("c_street", xml.getStreet(addressNode));
      item.setValue("c_postalcode", xml.getPostalCode(addressNode).toString());
      item.setValue("c_location", xml.getLocation(addressNode));
      QString p_country = xml.getCountry(addressNode);
      if (!p_country.isEmpty()) {
        item.setValue("c_country_bcp47", bcp47Country(p_country));
        item.setValue("o_vat_country", bcp47Country(p_country));
        AntiquaCRM::AEuropeanCountries EUCountries;
        QString c = EUCountries.value(bcp47Country(p_country).toUpper());
        item.setValue("c_country", (c.isEmpty() ? p_country : c));
      }
      item.setValue("c_phone_0", xml.getPhone(addressNode));
      item.setValue("c_email_0", xml.getEMail(buyerNode));
      // Address Bodies
      QStringList buffer;
      buffer << xml.getFullname(addressNode);
      buffer << xml.getStreet(addressNode);
      buffer << xml.getPostalCode(addressNode).toString() + " " +
                    xml.getLocation(addressNode);
      item.setValue("c_postal_address", buffer.join("\n"));
      item.setValue("c_shipping_address", buffer.join("\n"));
    }
    // Article Orders
    QDomNodeList orderItems = xml.getOrderItemList();
    if (orderItems.size() > 0) {
      for (int i = 0; i < orderItems.size(); i++) {
        QDomElement orderItemNode = orderItems.at(i).toElement();
        AntiquaCRM::OrderArticleItems orderItem;
        QDomNode book = xml.firstChildNode(orderItemNode, "book");
        if (!book.isNull()) {
          QJsonValue jvalue;
          QDomElement bookElement = book.toElement();
          // Article Type
          item.setValue("o_media_type", AntiquaCRM::BOOK);
          jvalue = QJsonValue(AntiquaCRM::BOOK);
          orderItem.append(articleItem("a_type", jvalue));
          // Article title
          jvalue = QJsonValue(xml.getTagText(bookElement, "title"));
          orderItem.append(articleItem("a_title", jvalue));
          // Provider Order ID
          jvalue = QJsonValue(bookElement.attribute("id", ""));
          orderItem.append(articleItem("a_provider_id", jvalue));
          // Article ID
          QDomNode queryNode = xml.firstChildNode(bookElement, "vendorKey");
          jvalue = QJsonValue(xml.getNodeValue(queryNode).toInt());
          orderItem.append(articleItem("a_article_id", jvalue));
          // Article Count
          orderItem.append(articleItem("a_count", QJsonValue(1)));
          // Article Price
          queryNode = xml.firstChildNode(bookElement, "price");
          jvalue = QJsonValue(xml.getNodeValue(queryNode).toDouble());
          orderItem.append(articleItem("a_price", jvalue));
          // Article Sell Price (VK)
          queryNode = xml.firstChildNode(bookElement, "price");
          jvalue = QJsonValue(xml.getNodeValue(queryNode).toDouble());
          orderItem.append(articleItem("a_sell_price", jvalue));
          // restore
          jvalue = QJsonValue();
        }
        item.insertOrderItems(orderItem);
      }
    }

    allOrders.append(item);
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
