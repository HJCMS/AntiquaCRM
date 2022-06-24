// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksifacewidget.h"
#include "abebooksconfig.h"
#include "abebookspurchaser.h"
#include "abebooksrequester.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRegExp>
#include <QTableWidgetItem>
#include <QVBoxLayout>

AbeBooksIfaceWidget::AbeBooksIfaceWidget(const QString &widgetId,
                                         QWidget *parent)
    : Antiqua::InterfaceWidget{widgetId, parent} {
  setWidgetResizable(true);
  setObjectName(widgetId);
  setWindowTitle(widgetId);

  QWidget *mainWidget = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(mainWidget);
  layout->setContentsMargins(0, 0, 0, 0);
  // BEGIN Artikelanzeige
  m_orderTable = new Antiqua::PurchaserOrderTable(mainWidget);
  layout->addWidget(m_orderTable);
  // END

  QHBoxLayout *infoLayout = new QHBoxLayout();
  infoLayout->setContentsMargins(10, 2, 10, 2);
  QLabel *lbInfo = new QLabel(tr("purchaser"));
  lbInfo->setIndent(10);
  infoLayout->addWidget(lbInfo);
  infoLayout->addStretch(1);
  QPushButton *btn_check = new QPushButton(this);
  btn_check->setText(tr("article check"));
  btn_check->setIcon(style()->standardIcon(QStyle::SP_MessageBoxQuestion));
  infoLayout->addWidget(btn_check);
  layout->addLayout(infoLayout);

  // BEGIN Verkäufer
  m_purchaserWidget = new AbeBooksPurchaser(this);
  layout->addWidget(m_purchaserWidget);
  // END
  layout->addStretch(1);
  mainWidget->setLayout(layout);
  setWidget(mainWidget);

  connect(m_orderTable, SIGNAL(findArticleNumbers()), this,
          SLOT(readCurrentArticleIds()));
  connect(btn_check, SIGNAL(clicked()), this, SLOT(readCurrentArticleIds()));
}

const QString AbeBooksIfaceWidget::stripString(const QString &str) const {
  QString buf(str);
  buf.replace("'", "´");
  buf = buf.trimmed();
  return buf;
}

const QVariant AbeBooksIfaceWidget::tableData(int row, int column) {
  QTableWidgetItem *item = m_orderTable->item(row, column);
  if (item != nullptr)
    return item->data(Qt::DisplayRole);

  return QVariant();
}

void AbeBooksIfaceWidget::setTableData(int row, int column,
                                       const QVariant &val) {
  QTableWidgetItem *item = m_orderTable->createItem(val.toString());
  m_orderTable->setItem(row, column, item);
}

const QJsonValue AbeBooksIfaceWidget::getString(const QString &objName) {
  QString data = m_purchaserWidget->getValue(objName).toString();
  return QJsonValue(stripString(data));
}

const QJsonValue AbeBooksIfaceWidget::getNumeric(const QString &objName) {
  int data = m_purchaserWidget->getValue(objName).toInt();
  return QJsonValue(data);
}

const QJsonValue AbeBooksIfaceWidget::getPrice(const QString &objName) {
  double data = m_purchaserWidget->getValue(objName).toDouble();
  return QJsonValue(data);
}

void AbeBooksIfaceWidget::createCustomerDocument() {
  if (p_currentDocument.isNull()) {
    qWarning("Current DomDocument is empty!");
    return;
  }

  if (m_purchaserWidget->getValue("costumerId").toInt() > 0) {
    qInfo("CustomerId already exists!");
    return;
  }

  AbeBooksDocument xml(p_currentDocument);
  QJsonObject jsObject;
  jsObject.insert("provider", QJsonValue(CONFIG_PROVIDER));
  jsObject.insert("type", "customer_create");
  QString invoice = m_purchaserWidget->rechnungsadresse->toPlainText();
  invoice = stripString(invoice);
  if (!invoice.isEmpty()) {
    jsObject.insert("c_postal_address", QJsonValue(invoice));
  }

  QString deliveryAddress = m_purchaserWidget->lieferadresse->toPlainText();
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
  queryObject.insert("orderid", QJsonValue(str.replace(PLUGIN_ID_PREFIX, "")));
  queryObject.insert("type", "customer_request");
  queryObject.insert("c_firstname", object["c_firstname"]);
  queryObject.insert("c_lastname", object["c_lastname"]);
  queryObject.insert("c_postalcode", object["c_postalcode"]);
  queryObject.insert("c_location", object["c_location"]);
  queryObject.insert("c_country", object["c_country"]);
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

  m_purchaserWidget->setValue(section, buffer.join("\n"));
  buffer.clear();
}

void AbeBooksIfaceWidget::readCurrentArticleIds() {
  QList<int> ids;
  for (int r = 0; r < m_orderTable->rowCount(); r++) {
    QTableWidgetItem *item = m_orderTable->item(r, 1);
    if (item != nullptr) {
      ids.append(item->data(Qt::DisplayRole).toInt());
    }
  }
  if (ids.count() > 0)
    emit checkArticleIds(ids);
}

void AbeBooksIfaceWidget::setXmlContent(const QDomDocument &doc) {
  if (doc.isNull())
    return;

  p_currentDocument = doc;
  AbeBooksDocument xml(doc);
  QDomNodeList n_list = xml.getPurchaseOrder().childNodes();
  for (int i = 0; i < n_list.count(); i++) {
    QDomNode n = n_list.at(i);
    if (n.nodeName() == "buyer") {
      QString email = n.namedItem("email").firstChild().nodeValue();
      m_purchaserWidget->setValue("c_email_0", email);
      if (!n.namedItem("mailingAddress").isNull()) {
        QDomNodeList a_list = n.namedItem("mailingAddress").childNodes();
        QJsonObject customerInfo;
        for (int l = 0; l < a_list.count(); l++) {
          QDomNode cn = a_list.at(l);
          QJsonValue val = QJsonValue(xml.getNodeValue(cn).toString());
          if (cn.nodeName() == "name") {
            QStringList full_name = xml.getNodeValue(cn).toString().split(" ");
            m_purchaserWidget->setValue("c_firstname", full_name.first());
            m_purchaserWidget->setValue("c_lastname", full_name.last());
            customerInfo.insert("c_firstname", QJsonValue(full_name.first()));
            customerInfo.insert("c_lastname", QJsonValue(full_name.last()));
          } else if (cn.nodeName() == "country") {
            customerInfo.insert("c_country", val);
            m_purchaserWidget->setValue("c_country", val.toString());
          } else if (cn.nodeName() == "city") {
            customerInfo.insert("c_location", val);
            m_purchaserWidget->setValue("c_location", val.toString());
          } else if (cn.nodeName() == "code") {
            customerInfo.insert("c_postalcode", val);
            m_purchaserWidget->setValue("c_postalcode", val.toString());
          } else if (cn.nodeName() == "street") {
            customerInfo.insert("c_street", val);
            m_purchaserWidget->setValue("c_street", val.toString());
          } else if (cn.nodeName() == "phone") {
            customerInfo.insert("c_phone_0", val);
            m_purchaserWidget->setValue("c_phone_0", val.toString());
          }
        }
        QJsonDocument cIjs = customerRequest(customerInfo);
        if (!cIjs.isEmpty())
          emit checkCustomer(cIjs);

        parseAddressBody("rechnungsadresse", customerInfo);
      }
    }
  }
  // Bestellartikel einfügen
  m_orderTable->setRowCount(0);
  n_list = xml.getOrderItemList().childNodes();
  for (int i = 0; i < n_list.count(); i++) {
    int column = 0;
    int row = m_orderTable->rowCount();
    QDomNode n = n_list.at(i);
    if (n.nodeName() == "purchaseOrderItem") {
      m_orderTable->setRowCount((m_orderTable->rowCount() + 1));
      QDomNodeList a_list = n.namedItem("book").childNodes();
      QString id = windowTitle().replace(PLUGIN_ID_PREFIX, "");
      setTableData(row, 0, id);
      setTableData(row, 2, 1); // Menge
      for (int l = 0; l < a_list.count(); l++) {
        QDomNode cn = a_list.at(l);
        QVariant val = cn.firstChild().nodeValue();
        if (cn.nodeName() == "price") {
          setTableData(row, 3, val);
        } else if (cn.nodeName() == "title") {
          setTableData(row, 4, val);
        } else if (cn.nodeName() == "vendorKey") {
          setTableData(row, 1, val);
        }
      }
    }
  }
}

void AbeBooksIfaceWidget::createOrderRequest(const QString &purchaseId) {
  AbeBooksRequester *req = new AbeBooksRequester(this);
  req->setObjectName(CONFIG_PROVIDER);
  connect(req, SIGNAL(response(const QDomDocument &)), this,
          SLOT(setXmlContent(const QDomDocument &)));

  req->queryOrder(purchaseId);
}

void AbeBooksIfaceWidget::setCustomerId(int customerId) {
  if (customerId > 0) {
    currentCustomerId = customerId;
    m_purchaserWidget->setCustomerId(customerId);
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
  int cid = m_purchaserWidget->getValue("costumerId").toInt();
  order.setProvider(CONFIG_PROVIDER);
  order.setProviderId(objectName());
  order.setCustomerId(cid);
  int col = 1; /**< CustomerId Cell */
  QStringList ids;
  for (int r = 0; r < m_orderTable->rowCount(); r++) {
    QString aid = tableData(r, col).toString();
    if (!aid.isEmpty())
      ids.append(aid);
  }
  order.setArticleIds(ids);
  return order;
}
