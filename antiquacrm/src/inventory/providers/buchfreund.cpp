// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "buchfreund.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "myicontheme.h"
#include "provider.h"
#include "providerscustomerdata.h"
#include "providersordertable.h"
#include "providersstatements.h"
#include "sqlcore.h"

#include <QDir>
#include <QGridLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>

static const QString getConfigGroup() { return QString("provider/whsoft"); }

BF_Translater::BF_Translater() : QMap<QString, QString>{} {
  // public.customers @{
  insert("person", "a_customer_id");
  insert("anrede", "c_gender");
  insert("vorname", "c_firstname");
  insert("name", "c_lastname");
  insert("adresse", "c_street");
  insert("plz", "c_postalcode");
  insert("ort", "c_location");
  insert("land", "c_country");
  insert("telefon", "c_phone_0");
  insert("email", "c_email_0");
  // @}

  // public.article_orders @{
  insert("bestellnr", "a_article_id");
  insert("menge_bestellt", "a_count");
  insert("preis_pro_einheit", "a_sell_price");
  insert("datum", "a_modified");
  insert("titel", "a_title");
  // @}

  // public.inventory_orders @{
  insert("id", "o_provider_order_id");
  insert("provider", "o_provider_name");
  insert("menge_storniert", "o_cancellation");
  insert("stornogrund", "o_cancellation_text");
  insert("storniert_am", "o_cancellation_datetime");
  insert("lagerfach", "sl_storage");
  // @}
}

const QString BF_Translater::sqlParam(const QString &key) {
  QMap<QString, QString>::iterator fi;
  for (fi = begin(); fi != end(); ++fi) {
    if (fi.key() == key)
      return fi.value();
  }
  return QString();
}

const QString BF_Translater::jsonParam(const QString &key) {
  QMap<QString, QString>::iterator fi;
  for (fi = begin(); fi != end(); ++fi) {
    if (fi.value() == key)
      return fi.key();
  }
  return QString();
}

BF_JSonQuery::BF_JSonQuery(QObject *parent) : QObject{parent} {
  setObjectName("buchfreund_json_query");
}

const QUrl BF_JSonQuery::apiQuery(const QString &operation) {
  QRegExp pattern("([\\/]{2,})");
  ApplSettings cfg;

  QUrl url;
  cfg.beginGroup(getConfigGroup());
  url.setScheme(cfg.value("api_scheme").toString());
  url.setHost(cfg.value("api_host").toString());
  QString path(cfg.value("api_basepath").toString());
  path.append("/");
  path.append(cfg.value("api_key").toString());
  path.append("/" + operation);
  cfg.endGroup();
  url.setPath(path.replace(pattern, "/"));
  return url;
}

void BF_JSonQuery::queryList() {
  QDateTime curDt = QDateTime::currentDateTime();
  QTime t(curDt.time().hour(), curDt.time().minute(), 0);
  curDt.setTime(t);
  QDateTime from = curDt.addDays(-14);
  QJsonObject obj;
  obj.insert("datum_von",
             QJsonValue::fromVariant(from.toString(BF_DATE_FORMAT)));
  obj.insert("datum_bis",
             QJsonValue::fromVariant(curDt.toString(BF_DATE_FORMAT)));
  QJsonDocument doc(obj);
  QByteArray data = doc.toJson(QJsonDocument::Compact);
  QUrl url = apiQuery("bestellungen");
  Provider *prQuery = new Provider(this, false);
  prQuery->setObjectName("buchfreund_query_list");
  connect(prQuery, SIGNAL(responsed(const QJsonDocument &)), this,
          SIGNAL(listResponsed(const QJsonDocument &)));
  connect(prQuery, SIGNAL(finished()), prQuery, SLOT(deleteLater()));
  prQuery->sendPost(url, data);
}

void BF_JSonQuery::queryOrder(const QString &bfId) {
  QJsonObject obj;
  obj.insert("id", QJsonValue(bfId));
  QJsonDocument doc(obj);
  QByteArray data = doc.toJson(QJsonDocument::Compact);
  QUrl url = apiQuery("bestellung");
  Provider *prQuery = new Provider(this, false);
  prQuery->setObjectName("buchfreund_query_view");
  connect(prQuery, SIGNAL(responsed(const QJsonDocument &)), this,
          SIGNAL(orderResponsed(const QJsonDocument &)));
  connect(prQuery, SIGNAL(finished()), prQuery, SLOT(deleteLater()));
  prQuery->sendPost(url, data);
}

Buchfreund::Buchfreund(QWidget *parent) : QScrollArea{parent} {
  setWidgetResizable(true);

  m_sql = new HJCMS::SqlCore(this);

  QWidget *mainWidget = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(mainWidget);
  layout->setContentsMargins(0, 0, 0, 0);

  m_ordersTable = new ProvidersOrderTable(mainWidget);
  layout->addWidget(m_ordersTable);

  QLabel *lbInfo = new QLabel(tr("purchaser"));
  lbInfo->setIndent(5);
  layout->addWidget(lbInfo);

  m_customerData = new ProvidersCustomerData(mainWidget);
  layout->addWidget(m_customerData);

  layout->addStretch(1);
  mainWidget->setLayout(layout);
  setWidget(mainWidget);
}

void Buchfreund::setValue(const QString &objName, const QVariant &value) {
  UtilsMain *obj = findChild<UtilsMain *>(objName, Qt::FindChildrenRecursively);
  if (obj != nullptr) {
    obj->setValue(value);
    return;
  }
}

void Buchfreund::setContent(const QJsonDocument &doc) {
  if (doc.isEmpty())
    return;

  int errors = QJsonValue(doc["error"]).toBool();
  if (errors)
    return;

  QString bf_id(windowTitle());

  BF_Translater bfTr;
  QJsonObject response = QJsonValue(doc["response"]).toObject();
  if (!response.isEmpty()) {
    QJsonObject::iterator it; // Iterator
    for (it = response.begin(); it != response.end(); ++it) {
      QString f = bfTr.sqlParam(it.key());
      QJsonValue val = it.value();
      if (!f.isEmpty() && !val.toString().isEmpty()) {
        setValue(f, val.toVariant());
      }
    }
    QJsonObject shippingsaddr =
        QJsonValue(doc["response"]["rechnungsadresse"]).toObject();
    if (!shippingsaddr.isEmpty()) {
      for (it = shippingsaddr.begin(); it != shippingsaddr.end(); ++it) {
        QString f = bfTr.sqlParam(it.key());
        QJsonValue val = it.value();
        if (!f.isEmpty() && !val.toString().isEmpty()) {
          setValue(f, val.toVariant());
        }
      }
    }
  }

  QJsonArray positionen = QJsonValue(doc["response"]["positionen"]).toArray();
  if (positionen.count() > 0) {
    QJsonArray::iterator at;
    for (at = positionen.begin(); at != positionen.end(); ++at) {
      QJsonObject article = (*at).toObject();
      QJsonObject::iterator it;
      int column = 0;
      int row = m_ordersTable->rowCount();
      m_ordersTable->setRowCount((m_ordersTable->rowCount() + 1));
      QTableWidgetItem *item = m_ordersTable->createItem(bf_id);
      m_ordersTable->setItem(row, column++, item);
      for (it = article.begin(); it != article.end(); ++it) {
        QString f = bfTr.sqlParam(it.key());
        QVariant curValue = it.value().toVariant();
        if (!f.isEmpty() && !curValue.isNull() && f.contains("a_")) {
          QString txt = curValue.toString().trimmed();
          QTableWidgetItem *item = m_ordersTable->createItem(txt);
          m_ordersTable->setItem(row, column++, item);
        }
      }
    }
  }
}

void Buchfreund::fetchOrderContent(const QString &bfid) {

  // DUMMY TEST
  QString buffer;
  QString p("/Developement/antiqua/database/tmp/testfile.json");
  QFile fp(QDir::homePath() + p);
  if (fp.open(QIODevice::ReadOnly)) {
    QTextStream str(&fp);
    str.setCodec("UTF8");
    buffer.append(str.readAll());
    fp.close();
  }

  QJsonParseError parser;
  QJsonDocument doc = QJsonDocument::fromJson(buffer.toLocal8Bit(), &parser);
  if (parser.error != QJsonParseError::NoError) {
    qWarning("Json Parse Error!");
    return;
  }
  setContent(doc);
}
