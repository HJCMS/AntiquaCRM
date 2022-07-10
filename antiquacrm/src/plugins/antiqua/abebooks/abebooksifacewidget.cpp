// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksifacewidget.h"
#include "abebooksconfig.h"
#include "abebooksrequester.h"

//#include <QDir>
//#include <QFile>
#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRegExp>
#include <QTableWidgetItem>
#include <QVBoxLayout>

AbeBooksIfaceWidget::AbeBooksIfaceWidget(const QString &widgetId,
                                         QWidget *parent)
    : Antiqua::InterfaceWidget{widgetId, parent} {}

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

const QString AbeBooksIfaceWidget::getTagText(const QDomNode &node,
                                              const QString &tag) const {
  QString o = node.namedItem(tag).firstChild().nodeValue();
  return o.trimmed();
}

const QJsonDocument
AbeBooksIfaceWidget::customerRequest(const QJsonObject &object) {
  Q_UNUSED(object);
  QJsonObject customer;
  customer.insert("provider", QJsonValue(CONFIG_PROVIDER));
  QString str(objectName());
  customer.insert("orderid", QJsonValue(str.trimmed()));
  customer.insert("type", "customer_request");
  foreach (QString f, m_order->customerSearchFields()) {
    // qDebug() << Q_FUNC_INFO << f << p_customer.value(f);
    customer.insert(f, p_customer.value(f));
  }
  return QJsonDocument(customer);
}

void AbeBooksIfaceWidget::parseOrderDate(const QDomNode &node) {
  QString dt_str;
  QDomNode dateNode = node.namedItem("date");
  dt_str.append(getTagText(dateNode, "day") + ".");
  dt_str.append(getTagText(dateNode, "month") + ".");
  dt_str.append(getTagText(dateNode, "year"));
  dt_str.append(" ");
  QDomNode timeNode = node.namedItem("time");
  dt_str.append(getTagText(timeNode, "hour") + ":");
  dt_str.append(getTagText(timeNode, "minute") + ":");
  dt_str.append(getTagText(timeNode, "second"));

  QDateTime dt = QDateTime::fromString(dt_str, "d.M.yyyy h:m:ss");
  if (dt.isValid()) {
    QString dt_out = dt.toString(ANTIQUA_DATETIME_FORMAT);
    p_customer.insert("o_since", dt_out);
    m_order->setValue("o_since", dt_out);
  }
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

  p_customer.insert(section, buffer.join("\n"));
  m_order->setValue(section, buffer.join("\n"));
  buffer.clear();
}

void AbeBooksIfaceWidget::checkCustomerClicked() {
  QJsonDocument cIjs = customerRequest(QJsonObject());
  if (!cIjs.isEmpty())
    emit checkCustomer(cIjs);
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

#ifdef ABEBOOKS_TESTMODE
  saveSources(doc, objectName());
#endif

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
            p_customer.insert("c_firstname", full_name.first());
            m_order->setValue("c_lastname", full_name.last());
            p_customer.insert("c_lastname", full_name.last());
            customerInfo.insert("c_firstname", QJsonValue(full_name.first()));
            customerInfo.insert("c_lastname", QJsonValue(full_name.last()));
          } else if (cn.nodeName() == "country") {
            customerInfo.insert("c_country", val);
            m_order->setValue("c_country", stripString(val));
            p_customer.insert("c_country", stripString(val));
          } else if (cn.nodeName() == "city") {
            customerInfo.insert("c_location", val);
            m_order->setValue("c_location", stripString(val));
            p_customer.insert("c_location", stripString(val));
          } else if (cn.nodeName() == "code") {
            customerInfo.insert("c_postalcode", val);
            m_order->setValue("c_postalcode", stripString(val));
            p_customer.insert("c_postalcode", stripString(val));
          } else if (cn.nodeName() == "street") {
            customerInfo.insert("c_street", val);
            m_order->setValue("c_street", stripString(val));
            p_customer.insert("c_street", stripString(val));
          } else if (cn.nodeName() == "phone") {
            customerInfo.insert("c_phone_0", val);
            m_order->setPhone("c_phone_0", stripString(val));
            p_customer.insert("c_phone_0", stripString(val));
          }
        }
        parseAddressBody("c_postal_address", customerInfo);
        parseAddressBody("c_shipping_address", customerInfo);
        // Sende SQL Abfrage an Hauptfenster!
        checkCustomerClicked();
      }
    }
    // Datum
    if (n.nodeName() == "orderDate") {
      parseOrderDate(n);
    } else if (n.hasChildNodes()) {
      QString param = sqlParam(n.nodeName());
      QString value = stripString(n.firstChild().nodeValue());
      if (!param.isEmpty() && !value.isEmpty())
        m_order->setValue(param, value);
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
      QDomElement status = n.namedItem("status").toElement();
      if (status.hasChildNodes()) {
        QString statusText = status.firstChild().nodeValue();
        m_order->setValue("o_payment_status", purchaseType(statusText));
        p_customer.insert("o_payment_status", purchaseType(statusText));
      }
    }
  }
}

void AbeBooksIfaceWidget::createOrderRequest(const QString &purchaseId) {
  AbeBooksRequester *req = new AbeBooksRequester(this);
  req->setObjectName(CONFIG_PROVIDER);
  connect(req, SIGNAL(response(const QDomDocument &)), this,
          SLOT(setXmlContent(const QDomDocument &)));

  // qDebug() << Q_FUNC_INFO << purchaseId;
  req->queryOrder(purchaseId);
}

void AbeBooksIfaceWidget::setCustomerId(int customerId) {
  if (customerId > 0) {
    currentCustomerId = customerId;
    m_order->setCustomerId(customerId);
  }
}

const QString AbeBooksIfaceWidget::purchaseType(const QString &key) const {
  QMap<QString, QString> map;
  /**
   * @brief Die vom Besteller gewählte Zahlungsart.
   * @class OrdersPaymentTypes
   */
  // BEGIN purchaseMethod Tag
  map.insert("Check", "12");
  map.insert("PayPal", "6");
  map.insert("Bank/Wire Transfer", "10");
  map.insert("Money Order", "13");
  map.insert("Bank Draft", "12");
  map.insert("Invoice", "11");
  // END

  // BEGIN status Tag
  map.insert("Buyer Cancelled", tr("Buyer Cancelled"));
  map.insert("Cancelled", tr("Cancelled"));
  map.insert("Expired", tr("Expired"));
  map.insert("Ordered", tr("Ordered"));
  map.insert("Previously Sold", tr("Previously Sold"));
  map.insert("Rejected", tr("Rejected"));
  map.insert("Shipped", tr("Shipped"));
  map.insert("Availability confirmed", tr("Availability confirmed"));
  // END
  return map.value(key);
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

  map.insert("purchaseMethod", "o_payment_method");
  map.insert("reseller", "o_provider_name");

  // public.article_orders @{
  map.insert("vendorKey", "a_article_id");
  map.insert("price", "a_price");
  map.insert("total", "a_sell_price");
  map.insert("title", "a_title");
  // @}
  return map;
}
