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
  insert("kundenkommentar", "m_customerComment");
  insert("lieferadresse", "c_shipping_address");
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

const QString BF_Translater::apiParam(const QString &key) {
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
  cfg.beginGroup(BF_CONFIG_GROUP);
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

Buchfreund::Buchfreund(const QString &id, QWidget *parent)
    : QScrollArea{parent} {
  setWidgetResizable(true);
  /**
   * @warning Der Objekt Name wird als Identifikator verwendet!
   */
  setObjectName(id);
  setWindowTitle(id);

  QString oid(id);
  oid.replace("-", "_");
  oid = oid.toLower();

  m_sql = new HJCMS::SqlCore(this);
  m_sql->setObjectName("sql_query_" + oid);

  m_jsonQuery = new BF_JSonQuery(this);
  m_jsonQuery->setObjectName("json_query_" + oid);

  QWidget *mainWidget = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(mainWidget);
  layout->setContentsMargins(0, 0, 0, 0);

  m_ordersTable = new ProvidersOrderTable(mainWidget);
  layout->addWidget(m_ordersTable);

  QLabel *lbInfo = new QLabel(tr("purchaser"));
  lbInfo->setIndent(10);
  layout->addWidget(lbInfo);

  m_customerData = new ProvidersCustomerData(mainWidget);
  layout->addWidget(m_customerData);

  layout->addStretch(1);

  QToolBar *m_toolBar = new QToolBar(this);
  layout->addWidget(m_toolBar);

  mainWidget->setLayout(layout);
  setWidget(mainWidget);

  connect(m_jsonQuery, SIGNAL(orderResponsed(const QJsonDocument &)), this,
          SLOT(setContent(const QJsonDocument &)));

  m_jsonQuery->queryOrder(id);
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
    QJsonObject deliveryAddress =
        QJsonValue(doc["response"]["lieferadresse"]).toObject();
    if (!deliveryAddress.isEmpty()) {
      QStringList delivery;
      QString person(deliveryAddress["vorname"].toString().trimmed());
      person.append(" ");
      person.append(deliveryAddress["name"].toString().trimmed());
      delivery.append(person);

      QString location(deliveryAddress["plz"].toString().trimmed());
      location.append(" ");
      location.append(deliveryAddress["ort"].toString().trimmed());
      delivery.append(location);

      QString street(deliveryAddress["adresse"].toString().trimmed());
      delivery.append(street);

      QString f = bfTr.sqlParam("lieferadresse");
      setValue(f, delivery.join("\n"));
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

void Buchfreund::checkCustomer() {
  int cid = m_customerData->customerId();
  if (cid >= 1) {
    emit openCustomer(cid);
    return;
  }

  QString buffer;
  QString sql("SELECT c_id FROM customers WHERE ");
  buffer = m_customerData->getValue("c_firstname").toString();
  sql.append("c_firstname ILIKE '" + buffer + "'");
  sql.append(" AND ");
  buffer = m_customerData->getValue("c_lastname").toString();
  sql.append("c_lastname ILIKE '" + buffer + "'");
  sql.append(" AND ");
  buffer = m_customerData->getValue("c_postalcode").toString();
  sql.append("c_postalcode ILIKE '" + buffer + "'");
  sql.append(" AND ");
  buffer = m_customerData->getValue("c_location").toString();
  sql.append("c_location ILIKE '" + buffer + "'");
  sql.append(" ORDER BY c_id;");
  if (SHOW_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << sql;
  }
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    q.next();
    if (q.value("c_id").toInt() > 0) {
      int cid = q.value("c_id").toInt();
      m_customerData->setCustomerId(cid);
    }
  } else {
    QString errors = m_sql->lastError();
    if (!errors.isEmpty()) {
      qDebug() << Q_FUNC_INFO << errors;
    }
  }
}

const QList<int> Buchfreund::getArticleIds() {
  QList<int> list;
  int c = m_ordersTable->articleColumn();
  for (int r = 0; r < m_ordersTable->rowCount(); r++) {
    int aid = m_ordersTable->getData(r, c).toInt();
    list.append(aid);
  }
  return list;
}

void Buchfreund::queryListEntries(QWidget *receiver) {
  BF_JSonQuery *m_q = new BF_JSonQuery(receiver);
  connect(m_q, SIGNAL(listResponsed(const QJsonDocument &)), receiver,
          SLOT(readBFOrders(const QJsonDocument &)));

  m_q->queryList();
}

void Buchfreund::testContent() {
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
