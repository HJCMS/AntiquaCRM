// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "whsoftwidget.h"
#include "whsoftconfig.h"
#include "whsoftjsonquery.h"
#include "whsoftpurchaser.h"

#include <QLabel>
#include <QList>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QVBoxLayout>

WHSoftWidget::WHSoftWidget(const QString &widgetId, QWidget *parent)
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
  m_purchaserWidget = new WHSoftPurchaser(this);
  layout->addWidget(m_purchaserWidget);
  // END

  layout->addStretch(1);
  mainWidget->setLayout(layout);
  setWidget(mainWidget);

  connect(m_orderTable, SIGNAL(findArticleNumbers()), this,
          SLOT(readCurrentArticleIds()));
  connect(btn_check, SIGNAL(clicked()), this, SLOT(readCurrentArticleIds()));
}

const QVariant WHSoftWidget::tableData(int row, int column) {
  QTableWidgetItem *item = m_orderTable->item(row, column);
  if (item != nullptr)
    return item->data(Qt::DisplayRole);

  return QVariant();
}

void WHSoftWidget::createCustomerDocument() {
  if (p_currentDocument.isEmpty()) {
    qWarning("Current Json Document is empty!");
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

  m_purchaserWidget->setValue(section, buffer.join("\n"));
  buffer.clear();

  // Sende SQL Abfrage an Hauptfenster!
  if (section == "rechnungsadresse") {
    QJsonDocument qDoc = customerRequest(object);
    if (!qDoc.isEmpty())
      emit checkCustomer(qDoc);
  }
}

void WHSoftWidget::readCurrentArticleIds() {
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
        m_purchaserWidget->setValue(f, val.toVariant());
      }
    }

    QJsonObject addr1 = QJsonValue(doc[mainKey]["rechnungsadresse"]).toObject();
    if (!addr1.isEmpty()) {
      for (it = addr1.begin(); it != addr1.end(); ++it) {
        QString f = it.key();
        QJsonValue val = it.value();
        if (!f.isEmpty() && !val.toString().isEmpty()) {
          m_purchaserWidget->setValue(f, val.toString());
        }
      }
      parseAddressBody("rechnungsadresse", addr1);
    }
    QJsonObject addr2 = QJsonValue(doc[mainKey]["lieferadresse"]).toObject();
    if (!addr2.isEmpty()) {
      parseAddressBody("lieferadresse", addr2);
    }
  }

  // Bestellartikel einfügen
  m_orderTable->setRowCount(0);
  QJsonArray positionen = QJsonValue(doc["response"]["positionen"]).toArray();
  if (positionen.count() > 0) {
    QJsonArray::iterator at;
    for (at = positionen.begin(); at != positionen.end(); ++at) {
      QJsonObject article = (*at).toObject();
      QJsonObject::iterator it;
      int column = 0;
      int row = m_orderTable->rowCount();
      m_orderTable->setRowCount((m_orderTable->rowCount() + 1));
      QTableWidgetItem *item = m_orderTable->createItem(windowTitle());
      m_orderTable->setItem(row, column++, item);
      for (it = article.begin(); it != article.end(); ++it) {
        QString f = sqlParam(it.key());
        QVariant curValue = it.value().toVariant();
        if (!f.isEmpty() && !curValue.isNull() && f.contains("a_")) {
          QString txt = curValue.toString().trimmed();
          QTableWidgetItem *item = m_orderTable->createItem(txt);
          m_orderTable->setItem(row, column++, item);
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
    m_purchaserWidget->setCustomerId(customerId);
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

const ProviderOrder WHSoftWidget::getProviderOrder() {
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
