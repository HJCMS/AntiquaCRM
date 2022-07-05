// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklookerifacewidget.h"
#include "booklookerconfig.h"
#include "booklookerrequester.h"

#include <QDebug>
#include <QHash>

BooklookerIfaceWidget::BooklookerIfaceWidget(const QString &widgetId,
                                             QWidget *parent)
    : Antiqua::InterfaceWidget{widgetId, parent} {}

const QJsonDocument
BooklookerIfaceWidget::customerRequest(const QJsonObject &obj) {
  QJsonObject customer;
  customer.insert("provider", CONFIG_PROVIDER);
  customer.insert("orderid", objectName().trimmed());
  customer.insert("type", "customer_request");
  customer.insert("c_firstname", obj["firstName"]);
  orderData.insert("c_firstname", obj["firstName"]);
  customer.insert("c_lastname", obj["name"]);
  orderData.insert("c_lastname", obj["name"]);
  customer.insert("c_postalcode", obj["zip"]);
  orderData.insert("c_postalcode", obj["zip"]);
  customer.insert("c_location", obj["city"]);
  orderData.insert("c_location", obj["city"]);
  return QJsonDocument(customer);
}

void BooklookerIfaceWidget::createOrderDataSet(const QJsonArray &array) {
  if (array.size() > 0) {
    // Bestellartikel einfügen
    m_order->setTableCount(0);
    QJsonArray items(array);
    QJsonArray::iterator at;
    for (at = items.begin(); at != items.end(); ++at) {
      QJsonObject article = (*at).toObject();
      int row = m_order->getTableCount();
      m_order->setTableCount((m_order->getTableCount() + 1));
      m_order->setTableData(row, 0, windowTitle());
      m_order->setTableData(row, 1, article.value("orderNo"));
      qint64 mount = article.value("amount").toInt();
      m_order->setTableData(row, 2, QString::number(mount));
      m_order->setTableData(row, 3, article.value("singlePrice"));
      m_order->setTableData(row, 4, article.value("orderTitle"));
    }
  }
}

void BooklookerIfaceWidget::parseAddressBody(const QString &key,
                                             const QJsonObject &obj) {
  QStringList buffer;
  QString person;
  QString gender(obj.value("title").toString().trimmed());
  if (!gender.isEmpty()) {
    person.append(gender);
    person.append(" ");
  }
  person.append(obj.value("firstName").toString().trimmed());
  person.append(" ");
  person.append(obj.value("name").toString().trimmed());
  buffer.append(person);
  m_order->setValue("person", person);

  QString street(obj.value("street").toString().trimmed());
  buffer.append(street);

  QString location(obj.value("zip").toString().trimmed());
  location.append(" ");
  location.append(obj.value("city").toString().trimmed());
  buffer.append(location);
  m_order->setValue(key, buffer.join("\n"));
  buffer.clear();

  // Sende SQL Abfrage an Hauptfenster!
  if (key == "c_postal_address") {
    QJsonDocument qDoc = customerRequest(obj);
    if (!qDoc.isEmpty())
      emit checkCustomer(qDoc);
  }
}

void BooklookerIfaceWidget::readCurrentArticleIds() {
  QList<int> ids = m_order->getArticleIDs();
  if (ids.count() > 0)
    emit checkArticleIds(ids);
}

void BooklookerIfaceWidget::createCustomerDocument() {
  if (p_currentDocument.isEmpty()) {
    qWarning("Current Json Document is empty!");
    return;
  }

  if (m_order->getCustomerId() > 0) {
    qInfo("CustomerId already exists!");
    return;
  }

  QJsonArray array = p_currentDocument["returnValue"].toArray();
  QJsonObject obj = array[0].toObject();
  QJsonObject customer;
  customer.insert("provider", QJsonValue(CONFIG_PROVIDER));
  customer.insert("type", "customer_create");
  QHashIterator<QString, QVariant> it(orderData);
  while (it.hasNext()) {
    it.next();
    customer.insert(it.key(), it.value().toString());
  }
  customer.insert("c_email", obj.value("email").toString());

  QString pAddress = m_order->getValue("c_postal_address").toString();
  if (!pAddress.isEmpty())
    customer.insert("c_postal_address", pAddress);

  // emit createCustomer(QJsonDocument(customer));
}

void BooklookerIfaceWidget::setContent(const QJsonDocument &doc) {
  if (QJsonValue(doc["status"]).toString() == "OK") {
    // Speichern
    p_currentDocument = doc;
    QJsonArray array = QJsonValue(doc["returnValue"]).toArray();
    for (int i = 0; i < array.size(); i++) {
      if (array[i].isObject()) {
        QJsonObject obj = array[i].toObject();
        QJsonObject::iterator it;
        for (it = obj.begin(); it != obj.end(); ++it) {
          QString f = it.key();
          QJsonValue val = it.value();
          if (f == "invoiceAddress") {
            parseAddressBody("c_postal_address", it->toObject());
            continue;
          } else if (f == "orderItems") {
            createOrderDataSet(val.toArray());
          }
          if (!f.isEmpty() && !val.toString().isEmpty()) {
            m_order->setValue(sqlParam(f), val.toVariant());
          }
        }
      }
    }
  }
}

void BooklookerIfaceWidget::createOrderRequest(const QString &orderId) {
#if PLUGIN_BOOKLOOKER_DEBUG
  setContent(testFile());
  return;
#endif

  BooklookerRequester *req = new BooklookerRequester(this);
  req->setObjectName(CONFIG_PROVIDER);
  connect(req, SIGNAL(response(const QJsonDocument &)), this,
          SLOT(setContent(const QJsonDocument &)));

  req->queryList();
}

void BooklookerIfaceWidget::setCustomerId(int customerId) {
  qDebug() << Q_FUNC_INFO << "TODO" << customerId;
}

const QMap<QString, QString> BooklookerIfaceWidget::fieldTranslate() const {
  QMap<QString, QString> map;
  // public.customers @{
  map.insert("buyerUsername", "a_customer_id");
  map.insert("title", "c_gender");
  map.insert("firstName", "c_firstname");
  map.insert("name", "c_lastname");
  map.insert("street", "c_street");
  map.insert("zip", "c_postalcode");
  map.insert("city", "c_location");
  map.insert("country", "c_country");
  map.insert("phone", "c_phone_0");
  map.insert("email", "c_email_0");
  // @}

  // public.article_orders @{
  map.insert("orderNo", "a_article_id");
  map.insert("amount", "a_count");
  map.insert("singlePrice", "a_sell_price");
  map.insert("orderTitle", "a_title");
  // @}
  return map;
}

const ProviderOrder BooklookerIfaceWidget::getProviderOrder() {
  ProviderOrder order;
  order.setProvider(CONFIG_PROVIDER);
  order.setProviderId(objectName());
  qDebug() << Q_FUNC_INFO << "TODO";
  return order;
}
