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
  m_network = new AntiquaCRM::ANetworker(AntiquaCRM::XML_QUERY, this);
  initConfigurations();

  connect(m_network, SIGNAL(sendXmlResponse(const QDomDocument &)),
          SLOT(prepareResponse(const QDomDocument &)));
  connect(m_network, SIGNAL(sendContentCodec(QTextCodec *)),
          SLOT(setContentDecoder(QTextCodec *)));
  connect(m_network, SIGNAL(finished(QNetworkReply *)),
          SLOT(queryFinished(QNetworkReply *)));
}

void Abebooks::initConfigurations() {
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
    item.setValue("o_provider_order_id", strOrderId);
    item.setValue("o_provider_purchase_id", orderId);
    item.setValue("o_since", dateTime);
    item.setValue("o_media_type", AntiquaCRM::BOOK);

    // AntiquaCRM::PaymentStatus
    QDomNode statusNode = xml.firstChildNode(orderElement, "status");
    QString orderStatus = xml.getNodeValue(statusNode).toString();
    if (orderStatus.contains("Ordered")) {
      item.setValue("o_provider_order_status", AntiquaCRM::SHIPMENT_CREATED);
    } else if (orderStatus.contains("Shipped")) {
      item.setValue("o_provider_order_status", AntiquaCRM::WAIT_FOR_PAYMENT);
    } else if (orderStatus.contains("Cancelled")) {
      item.setValue("o_provider_order_status", AntiquaCRM::ORDER_CANCELED);
    } else {
      item.setValue("o_provider_order_status", AntiquaCRM::STATUS_NOT_SET);
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

    // Buyer info
    QDomNodeList buyerNodes = orderElement.elementsByTagName("buyer");
    if (buyerNodes.size() > 0) {
      QDomElement buyerNode = buyerNodes.at(0).toElement();
      // eMail
      QDomNode eMailNode = xml.firstChildNode(buyerNode, "email");
      if (!eMailNode.isNull()) {
        QString eMail = xml.getNodeValue(eMailNode).toString();
        item.setValue("c_email_0", eMail.toLower());
      }
      // Postal Address
      QDomNode addressNode = buyerNode.namedItem("mailingAddress");
      QPair<QString, QString> person = xml.getPerson(addressNode);
      item.setValue("c_provider_import", xml.getFullname(addressNode));
      item.setValue("c_firstname", person.first);
      item.setValue("c_lastname", person.second);
      item.setValue("c_postalcode", xml.getPostalCode(addressNode).toString());
      item.setValue("c_location", xml.getLocation(addressNode));
      item.setValue("c_country", xml.getCountry(addressNode));
      item.setValue("c_street", xml.getStreet(addressNode));
      item.setValue("c_phone_0", xml.getPhone(addressNode));
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
        AntiquaCRM::AProviderOrderItems orderItem;
        QDomNode book = xml.firstChildNode(orderItemNode, "book");
        if (!book.isNull()) {
          QDomElement bookElement = book.toElement();
          // Article Type
          AntiquaCRM::AProviderOrderItem a_type;
          a_type.key = "a_type";
          a_type.value = AntiquaCRM::BOOK;
          item.setValue("o_media_type", AntiquaCRM::BOOK);
          orderItem.append(a_type);
          // Article title
          AntiquaCRM::AProviderOrderItem a_title;
          a_title.key = "a_title";
          a_title.value = xml.getTagText(bookElement, "title");
          orderItem.append(a_title);
          // Provider Order ID
          AntiquaCRM::AProviderOrderItem a_provider_id;
          a_provider_id.key = "a_provider_id";
          a_provider_id.value = bookElement.attribute("id", "");
          orderItem.append(a_provider_id);
          // Article ID
          QDomNode vendorKey = xml.firstChildNode(bookElement, "vendorKey");
          AntiquaCRM::AProviderOrderItem a_article_id;
          a_article_id.key = "a_article_id";
          a_article_id.value = xml.getNodeValue(vendorKey).toInt();
          orderItem.append(a_article_id);
          // count
          AntiquaCRM::AProviderOrderItem a_count;
          a_count.key = "a_count";
          a_count.value = 1;
          orderItem.append(a_count);
          // Price
          QDomNode price = xml.firstChildNode(bookElement, "price");
          AntiquaCRM::AProviderOrderItem a_price;
          a_price.key = "a_price";
          a_price.value = xml.getNodeValue(price).toDouble();
          orderItem.append(a_price);
          // TODO
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
  return true;
}
