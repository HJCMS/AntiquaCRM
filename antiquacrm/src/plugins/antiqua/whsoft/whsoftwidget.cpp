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

WHSoftWidget::WHSoftWidget(const QString &orderId, QWidget *parent)
    : Antiqua::InterfaceWidget{orderId, parent} {
  // Wegen langen Abfragewartezeiten den Zugriff unterdrücken!
  setEnabled(false);
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

  QJsonObject customer;
  customer.insert("provider", QJsonValue(CONFIG_PROVIDER));
  customer.insert("type", "customer_create");
  QHashIterator<QString, QString> hash_it(p_customer);
  while (hash_it.hasNext()) {
    hash_it.next();
    if (!hash_it.value().isEmpty())
      customer.insert(hash_it.key(), hash_it.value());
  }

  if (!customer.contains("c_postal_address")) {
    QString pAddress = m_order->getValue("c_postal_address").toString();
    if (!pAddress.isEmpty())
      customer.insert("c_postal_address", pAddress);
  }

  emit createCustomer(QJsonDocument(customer));
}

const QJsonDocument WHSoftWidget::customerRequest(const QJsonObject &object) {
  Q_UNUSED(object);
  QJsonObject customer;
  customer.insert("provider", QJsonValue(CONFIG_PROVIDER));
  customer.insert("orderid", QJsonValue(objectName()));
  customer.insert("type", "customer_request");
  foreach (QString f, m_order->customerSearchFields()) {
    customer.insert(f, p_customer.value(f));
  }
  return QJsonDocument(customer);
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
  p_customer.insert(section, buffer.join("\n"));
  buffer.clear();

  // Sende SQL Abfrage an Hauptfenster!
  if (section == "c_postal_address") {
    QJsonDocument qDoc = customerRequest(object);
    if (!qDoc.isEmpty())
      emit checkCustomer(qDoc);
  }
}

void WHSoftWidget::checkCustomerClicked() {
  QJsonDocument qDoc = customerRequest(QJsonObject());
  if (!qDoc.isEmpty())
    emit checkCustomer(qDoc);
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
#ifdef ANTIQUA_DEVELOPEMENT
  saveSources(doc, getOrderId());
#endif

  QString mainKey("response");
  QJsonObject response = QJsonValue(doc[mainKey]).toObject();
  if (!response.isEmpty()) {
    QJsonObject::iterator it;
    for (it = response.begin(); it != response.end(); ++it) {
      QString f = it.key();
      QJsonValue val = it.value();
      if (!f.isEmpty() && !val.toString().isEmpty()) {
        m_order->setValue(sqlParam(f), val.toVariant());
        p_customer.insert(sqlParam(f), stripString(val));
      }
    }

    QJsonObject deliver = QJsonValue(response["versand"]).toObject();
    if (!deliver.isEmpty()) {
      for (it = deliver.begin(); it != deliver.end(); ++it) {
        QString f = it.key();
        QJsonValue val = it.value();
        if (!sqlParam(f).isEmpty()) {
          QVariant dval = val.isNull() ? "" : val.toString();
          m_order->setValue(sqlParam(f), dval);
        }
      }
    }

    QJsonObject addr1 = QJsonValue(doc[mainKey]["rechnungsadresse"]).toObject();
    if (!addr1.isEmpty()) {
      for (it = addr1.begin(); it != addr1.end(); ++it) {
        QString f = it.key();
        QJsonValue val = it.value();
        if (!f.isEmpty() && !val.toString().isEmpty()) {
          m_order->setValue(sqlParam(f), val.toString());
          p_customer.insert(sqlParam(f), stripString(val));
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
  // jetzt aktivieren
  setEnabled(true);
}

void WHSoftWidget::createOrderRequest() {
  if (getOrderId().isEmpty()) {
    qWarning("Invalid W+HSoft OrderId");
    return;
  }

  WHSoftJSonQuery *mq = new WHSoftJSonQuery(this);
  mq->setObjectName("json_query_" + objectName());
  connect(mq, SIGNAL(orderResponsed(const QJsonDocument &)), this,
          SLOT(setContent(const QJsonDocument &)));

  mq->queryOrder(getOrderId());
}

void WHSoftWidget::setCustomerId(int customerId) {
  if (customerId > 0) {
    currentCustomerId = customerId;
    m_order->setCustomerId(customerId);
  }
}

const QMap<QString, QString> WHSoftWidget::fieldTranslate() const {
  QMap<QString, QString> map;
  map.insert("person", "a_customer_id");
  map.insert("anrede", "c_gender");
  map.insert("zusatz", "c_title");
  map.insert("vorname", "c_firstname");
  map.insert("name", "c_lastname");
  map.insert("adresse", "c_street");
  map.insert("plz", "c_postalcode");
  map.insert("ort", "c_location");
  map.insert("telefon", "c_phone_0");
  map.insert("email", "c_email_0");
  map.insert("land_iso2", "c_country");
  map.insert("land", "c_country_name");
  map.insert("waehrung", "o_payment_currency");
  map.insert("versandkosten", "o_delivery_cost");
  map.insert("versender", "o_delivery_name");
  map.insert("sendungsnummer", "o_delivery_send_id");
  map.insert("versandart", "o_delivery_service");
  map.insert("einlieferungsdatum", "o_delivery_fullfill");
  map.insert("kundenkommentar", "o_delivery_comment");
  //  map.insert("menge_storniert", "");
  //  map.insert("storniert_am", "");
  //  map.insert("stornogrund", "");
  map.insert("id", "o_provider_order_id");
  map.insert("datum", "o_provider_order_date");
  map.insert("bestellnr", "a_article_id");
  map.insert("lagerfach", "sl_storage");
  map.insert("menge_bestellt", "a_count");
  map.insert("bestellwert", "a_sell_price");
  map.insert("preis_pro_einheit", "a_price");
  map.insert("titel", "a_title");
  map.insert("bezahlart", "o_payment_method");
  return map;
}
