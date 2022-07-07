// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "whsoftwidget.h"
#include "whsoftconfig.h"
#include "whsoftjsonquery.h"

#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

WHSoftWidget::WHSoftWidget(const QString &widgetId, QWidget *parent)
    : Antiqua::InterfaceWidget{widgetId, parent} {
}

void WHSoftWidget::createCustomerDocument() {
  if (p_currentDocument.isEmpty()) {
    qWarning("Current Json Document is empty!");
    return;
  }

  if (m_order->getCustomerId() > 0) {
    qInfo("CustomerId already exists!");
    return;
  }

  QString mainKey("response");
  QJsonDocument doc = p_currentDocument;
  QJsonObject obj = QJsonValue(doc[mainKey]).toObject();

  QJsonObject queryObject;
  queryObject.insert("provider", QJsonValue(CONFIG_PROVIDER));
  queryObject.insert("type", "customer_create");

  QJsonObject::iterator it; // Iterator
  for (it = obj.begin(); it != obj.end(); ++it) {
    QString f = sqlParam(it.key());
    QString v = it.value().toString();
    if (!v.isEmpty())
      queryObject.insert(f, v);
  }

  QJsonObject addr = QJsonValue(doc[mainKey]["rechnungsadresse"]).toObject();
  if (!addr.isEmpty()) {
    for (it = addr.begin(); it != addr.end(); ++it) {
      QString f = sqlParam(it.key());
      QString v = it.value().toString();
      if ((f == "c_gender" || f == "anrede") && !v.isEmpty()) {
        queryObject.insert(f, QString::number(genderFromString(v)));
      } else if (!f.isEmpty() && !v.isEmpty()) {
        queryObject.insert(f, v);
      }
    }
  }

  emit createCustomer(QJsonDocument(queryObject));
}

const QJsonDocument WHSoftWidget::customerRequest(const QJsonObject &object) {
  QJsonObject queryObject;
  queryObject.insert("provider", QJsonValue(CONFIG_PROVIDER));
  queryObject.insert("orderid", QJsonValue(objectName()));
  queryObject.insert("type", "customer_request");
  queryObject.insert("c_firstname", object["vorname"]);
  queryObject.insert("c_lastname", object["name"]);
  queryObject.insert("c_postalcode", object["plz"]);
  queryObject.insert("c_location", object["ort"]);
  return QJsonDocument(queryObject);
}

void WHSoftWidget::parseAddressBody(const QString &section,
                                    const QJsonObject &object) {
  QStringList buffer;
  QString person;
  QString gender(object["anrede"].toString().trimmed());
  if (!gender.isEmpty()) {
    person.append(gender);
    person.append(" ");
  }
  person.append(object["vorname"].toString().trimmed());
  person.append(" ");
  person.append(object["name"].toString().trimmed());
  buffer.append(person);
  m_order->setValue("person", person);

  QString street(object["adresse"].toString().trimmed());
  buffer.append(street);

  QString location(object["plz"].toString().trimmed());
  location.append(" ");
  location.append(object["ort"].toString().trimmed());
  QString country = object["land"].toString().trimmed();
  if (!country.isEmpty()) {
    location.append("/" + country);
  }
  buffer.append(location);

  m_order->setValue(section, buffer.join("\n"));
  buffer.clear();

  // Sende SQL Abfrage an Hauptfenster!
  if (section == "c_postal_address") {
    QJsonDocument qDoc = customerRequest(object);
    if (!qDoc.isEmpty())
      emit checkCustomer(qDoc);
  }
}

void WHSoftWidget::readCurrentArticleIds() {
  QList<int> ids = m_order->getArticleIDs();
  if (ids.count() > 0)
    emit checkArticleIds(ids);
}

void WHSoftWidget::setContent(const QJsonDocument &doc) {
  if (doc.isEmpty())
    return;

  int errors = QJsonValue(doc["error"]).toBool();
  if (errors)
    return;

  // Speichern
  p_currentDocument = doc;

  QString mainKey("response");
  QJsonObject response = QJsonValue(doc[mainKey]).toObject();
  if (!response.isEmpty()) {
    QJsonObject::iterator it;
    for (it = response.begin(); it != response.end(); ++it) {
      QString f = it.key();
      QJsonValue val = it.value();
      if (!f.isEmpty() && !val.toString().isEmpty()) {
        m_order->setValue(sqlParam(f), val.toVariant());
      }
    }

    QJsonObject addr1 = QJsonValue(doc[mainKey]["rechnungsadresse"]).toObject();
    if (!addr1.isEmpty()) {
      for (it = addr1.begin(); it != addr1.end(); ++it) {
        QString f = it.key();
        QJsonValue val = it.value();
        if (!f.isEmpty() && !val.toString().isEmpty()) {
          m_order->setValue(sqlParam(f), val.toString());
        }
      }
      parseAddressBody("c_postal_address", addr1);
    }
    QJsonObject addr2 = QJsonValue(doc[mainKey]["lieferadresse"]).toObject();
    if (!addr2.isEmpty()) {
      parseAddressBody("c_shipping_address", addr2);
    }
  }

  // Bestellartikel einfügen
  m_order->setTableCount(0);
  QJsonArray positionen = QJsonValue(doc["response"]["positionen"]).toArray();
  if (positionen.count() > 0) {
    QJsonArray::iterator at;
    for (at = positionen.begin(); at != positionen.end(); ++at) {
      QJsonObject article = (*at).toObject();
      QJsonObject::iterator it;
      int column = 0;
      int row = m_order->getTableCount();
      m_order->setTableCount((m_order->getTableCount() + 1));
      m_order->setTableData(row, column++, windowTitle());
      for (it = article.begin(); it != article.end(); ++it) {
        QString f = sqlParam(it.key());
        QVariant curValue = it.value().toVariant();
        if (!f.isEmpty() && !curValue.isNull() && f.contains("a_")) {
          QString txt = curValue.toString().trimmed();
          m_order->setTableData(row, column++, txt);
        }
      }
    }
  }
}

void WHSoftWidget::createOrderRequest(const QString &bfId) {
  WHSoftJSonQuery *mq = new WHSoftJSonQuery(this);
  mq->setObjectName("json_query_" + objectName());
  connect(mq, SIGNAL(orderResponsed(const QJsonDocument &)), this,
          SLOT(setContent(const QJsonDocument &)));

  mq->queryOrder(bfId);
}

void WHSoftWidget::setCustomerId(int customerId) {
  if (customerId > 0) {
    currentCustomerId = customerId;
    m_order->setCustomerId(customerId);
  }
}

const QMap<QString, QString> WHSoftWidget::fieldTranslate() const {
  QMap<QString, QString> map;
  // public.customers @{
  map.insert("person", "a_customer_id");
  map.insert("anrede", "c_gender");
  map.insert("vorname", "c_firstname");
  map.insert("name", "c_lastname");
  map.insert("adresse", "c_street");
  map.insert("plz", "c_postalcode");
  map.insert("ort", "c_location");
  map.insert("land", "c_country");
  map.insert("telefon", "c_phone_0");
  map.insert("email", "c_email_0");
  // @}

  // public.article_orders @{
  map.insert("bestellnr", "a_article_id");
  map.insert("menge_bestellt", "a_count");
  map.insert("preis_pro_einheit", "a_sell_price");
  map.insert("titel", "a_title");
  // @}
  return map;
}
