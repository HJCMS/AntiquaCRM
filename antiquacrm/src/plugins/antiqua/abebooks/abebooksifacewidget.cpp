// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksifacewidget.h"
#include "abebooksconfig.h"
#include "abebooksrequester.h"

#include <QDir>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRegExp>
#include <QTableWidgetItem>
#include <QVBoxLayout>

AbeBooksIfaceWidget::AbeBooksIfaceWidget(const QString &widgetId,
                                         QWidget *parent)
    : Antiqua::InterfaceWidget{widgetId, parent} {
}

void AbeBooksIfaceWidget::createCustomerDocument() {
  if (p_currentDocument.isNull()) {
    qWarning("Current DomDocument is empty!");
    return;
  }

  if (m_order->getCustomerId() > 0) {
    qInfo("CustomerId already exists!");
    return;
  }

  AbeBooksDocument xml(p_currentDocument);
  QJsonObject jsObject;
  jsObject.insert("provider", QJsonValue(CONFIG_PROVIDER));
  jsObject.insert("type", "customer_create");
  QString invoice = m_order->getValue("c_postal_address").toString();
  invoice = stripString(invoice);
  if (!invoice.isEmpty()) {
    jsObject.insert("c_postal_address", QJsonValue(invoice));
  }

  QString deliveryAddress = m_order->getValue("c_shipping_address").toString();
  deliveryAddress = stripString(deliveryAddress);
  if (!deliveryAddress.isEmpty()) {
    jsObject.insert("c_shipping_address", QJsonValue(deliveryAddress));
  }

  jsObject.insert("c_firstname", getString("c_firstname"));
  jsObject.insert("c_lastname", getString("c_lastname"));
  jsObject.insert("c_email_0", getString("c_email_0"));

  QRegExp phonePattern("\\d+");
  QString phone = xml.getAddressValue("phone").toString();
  phone.replace("-", "");
  phone = stripString(phone);
  if (!phone.isEmpty() && phonePattern.exactMatch(phone)) {
    qint64 phoneNumber = phone.toInt();
    jsObject.insert("c_phone_0", QJsonValue(phoneNumber));
  }

  QString country = xml.getAddressValue("country").toString();
  country = stripString(country);
  if (!country.isEmpty()) {
    jsObject.insert("c_country", QJsonValue(country));
  }

  QString location = xml.getAddressValue("city").toString();
  location = stripString(location);
  if (!location.isEmpty()) {
    jsObject.insert("c_location", QJsonValue(location));
  }

  QString street = xml.getAddressValue("street").toString();
  street = stripString(street);
  if (!street.isEmpty()) {
    jsObject.insert("c_street", QJsonValue(street));
  }

  QString postal = xml.getAddressValue("code").toString();
  postal = stripString(postal);
  if (!postal.isEmpty()) {
    jsObject.insert("c_postalcode", QJsonValue(postal));
  }

  emit createCustomer(QJsonDocument(jsObject));
}

const QJsonDocument
AbeBooksIfaceWidget::customerRequest(const QJsonObject &object) {
  QJsonObject queryObject;
  queryObject.insert("provider", QJsonValue(CONFIG_PROVIDER));
  QString str(objectName());
  queryObject.insert("orderid", QJsonValue(str.trimmed()));
  queryObject.insert("type", "customer_request");
  queryObject.insert("c_firstname", object["c_firstname"]);
  queryObject.insert("c_lastname", object["c_lastname"]);
  queryObject.insert("c_postalcode", object["c_postalcode"]);
  queryObject.insert("c_location", object["c_location"]);
  return QJsonDocument(queryObject);
}

void AbeBooksIfaceWidget::parseAddressBody(const QString &section,
                                           const QJsonObject &obj) {
  QStringList buffer;
  QString person;
  person.append(obj["c_firstname"].toString().trimmed());
  person.append(" ");
  person.append(obj["c_lastname"].toString().trimmed());
  buffer.append(person);
  m_order->setValue("person", person);

  QString street(obj["c_street"].toString().trimmed());
  buffer.append(street);

  QString location(obj["c_postalcode"].toString().trimmed());
  location.append(" ");
  location.append(obj["c_location"].toString().trimmed());
  QString country = obj["c_country"].toString().trimmed();
  if (!country.isEmpty()) {
    location.append("/" + country);
  }
  buffer.append(location);

  m_order->setValue(section, buffer.join("\n"));
  buffer.clear();
}

void AbeBooksIfaceWidget::readCurrentArticleIds() {
  QList<int> ids = m_order->getArticleIDs();
  if (ids.count() > 0)
    emit checkArticleIds(ids);
}

void AbeBooksIfaceWidget::setXmlContent(const QDomDocument &doc) {
  if (doc.isNull())
    return;

  AbeBooksDocument xml(doc);
  if (doc.documentElement().tagName() == "requestError") {
    QPair<int, QString> err = xml.errorResponseCode();
    qDebug() << Q_FUNC_INFO << err.first << err.second;
    emit errorResponse(err.first, err.second);
    return;
  }

  p_currentDocument = doc;
  QDomNodeList n_list = xml.getPurchaseOrder().childNodes();
  for (int i = 0; i < n_list.count(); i++) {
    QDomNode n = n_list.at(i);
    if (n.nodeName() == "buyer") {
      QString email = n.namedItem("email").firstChild().nodeValue();
      m_order->setValue("c_email_0", email);
      if (!n.namedItem("mailingAddress").isNull()) {
        QDomNodeList a_list = n.namedItem("mailingAddress").childNodes();
        QJsonObject customerInfo;
        for (int l = 0; l < a_list.count(); l++) {
          QDomNode cn = a_list.at(l);
          QJsonValue val = QJsonValue(xml.getNodeValue(cn).toString());
          if (cn.nodeName() == "name") {
            QStringList full_name = xml.getNodeValue(cn).toString().split(" ");
            m_order->setValue("c_firstname", full_name.first());
            m_order->setValue("c_lastname", full_name.last());
            customerInfo.insert("c_firstname", QJsonValue(full_name.first()));
            customerInfo.insert("c_lastname", QJsonValue(full_name.last()));
          } else if (cn.nodeName() == "country") {
            customerInfo.insert("c_country", val);
            m_order->setValue("c_country", stripString(val));
          } else if (cn.nodeName() == "city") {
            customerInfo.insert("c_location", val);
            m_order->setValue("c_location", stripString(val));
          } else if (cn.nodeName() == "code") {
            customerInfo.insert("c_postalcode", val);
            m_order->setValue("c_postalcode", stripString(val));
          } else if (cn.nodeName() == "street") {
            customerInfo.insert("c_street", val);
            m_order->setValue("c_street", stripString(val));
          } else if (cn.nodeName() == "phone") {
            customerInfo.insert("c_phone_0", val);
            m_order->setPhone("c_phone_0", stripString(val));
          }
        }
        parseAddressBody("c_postal_address", customerInfo);
        parseAddressBody("c_shipping_address", customerInfo);
        // Sende SQL Abfrage an Hauptfenster!
        QJsonDocument cIjs = customerRequest(customerInfo);
        if (!cIjs.isEmpty())
          emit checkCustomer(cIjs);
      }
    }
  }

  // Bestellartikel einfügen
  m_order->setTableCount(0);
  n_list = xml.getOrderItemList().childNodes();
  for (int i = 0; i < n_list.count(); i++) {
    int column = 0;
    int row = m_order->getTableCount();
    QDomNode n = n_list.at(i);
    if (n.nodeName() == "purchaseOrderItem") {
      m_order->setTableCount((m_order->getTableCount() + 1));
      QDomNodeList a_list = n.namedItem("book").childNodes();
      QString id = windowTitle().trimmed();
      m_order->setTableData(row, 0, id);
      m_order->setTableData(row, 2, 1); // Menge
      for (int l = 0; l < a_list.count(); l++) {
        QDomNode cn = a_list.at(l);
        QVariant val = cn.firstChild().nodeValue();
        if (cn.nodeName() == "price") {
          m_order->setTableData(row, 3, val);
        } else if (cn.nodeName() == "title") {
          m_order->setTableData(row, 4, val);
        } else if (cn.nodeName() == "vendorKey") {
          m_order->setTableData(row, 1, val);
        }
      }
    }
  }
}

void AbeBooksIfaceWidget::createOrderRequest(const QString &purchaseId) {
#ifdef ABEBOOKS_TESTMODE
  setXmlContent(testSources("abebooks-order-652255901.xml"));
  return;
#endif
  AbeBooksRequester *req = new AbeBooksRequester(this);
  req->setObjectName(CONFIG_PROVIDER);
  connect(req, SIGNAL(response(const QDomDocument &)), this,
          SLOT(setXmlContent(const QDomDocument &)));

  qDebug() << Q_FUNC_INFO << purchaseId;
  req->queryOrder(purchaseId);
}

void AbeBooksIfaceWidget::setCustomerId(int customerId) {
  if (customerId > 0) {
    currentCustomerId = customerId;
    m_order->setCustomerId(customerId);
  }
}

const QMap<QString, QString> AbeBooksIfaceWidget::fieldTranslate() const {
  QMap<QString, QString> map;
  // public.customers @{
  map.insert("gender", "c_gender");
  map.insert("name0", "c_firstname");
  map.insert("name", "c_lastname");
  map.insert("street", "c_street");
  map.insert("code", "c_postalcode");
  map.insert("city", "c_location");
  map.insert("country", "c_country");
  map.insert("phone", "c_phone_0");
  map.insert("email", "c_email_0");
  // @}

  // public.article_orders @{
  map.insert("vendorKey", "a_article_id");
  map.insert("price", "a_sell_price");
  map.insert("title", "a_title");
  // @}
  return map;
}

const ProviderOrder AbeBooksIfaceWidget::getProviderOrder() {
  ProviderOrder order;
  order.setProvider(CONFIG_PROVIDER);
  order.setProviderId(objectName());
  int cid = m_order->getCustomerId();
  if (cid < 1) {
    order.setCustomerId(-1);
    qWarning("Missing Customer Id");
    return order;
  }
  order.setCustomerId(cid);

  int col = 1; /**< ArticleId Cell */
  QStringList ids;
  for (int r = 0; r < m_order->getTableCount(); r++) {
    QString aid = m_order->getTableData(r, col).toString();
    if (!aid.isEmpty())
      ids.append(aid);
  }
  order.setArticleIds(ids);
  return order;
}
