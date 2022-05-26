// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "whsoft.h"

#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QTime>
#include <QVariant>

/**
 * @def DATE_FORMAT
 * @ingroup Providers SQL Statements
 * @brief Datumsformat von Buchfreund.de
 * Wird benötigt für die Json anfragen zur erstellen.
 * Gleichzeitig wird es auch für den Import benötigt!
 */
#ifndef DATE_FORMAT
#define DATE_FORMAT "yyyy-MM-dd hh:mm:ss"
#endif

/**
 * @brief Konfigurationsgruppe
 */
#ifndef CONFIG_GROUP
#define CONFIG_GROUP "provider/whsoft"
#endif

WHSoftJSonQuery::WHSoftJSonQuery(QObject *parent) : QObject{parent} {
  setObjectName("whsoft_json_query");
}

const QUrl WHSoftJSonQuery::apiQuery(const QString &operation) {
  QRegExp pattern("([\\/]{2,})");
  ApplSettings cfg;

  QUrl url;
  cfg.beginGroup(CONFIG_GROUP);
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

void WHSoftJSonQuery::queryList() {
  QDateTime curDt = QDateTime::currentDateTime();
  QTime t(curDt.time().hour(), curDt.time().minute(), 0);
  curDt.setTime(t);
  QDateTime from = curDt.addDays(-14);
  QJsonObject obj;
  obj.insert("datum_von", QJsonValue::fromVariant(from.toString(DATE_FORMAT)));
  obj.insert("datum_bis", QJsonValue::fromVariant(curDt.toString(DATE_FORMAT)));
  QJsonDocument doc(obj);
  QByteArray data = doc.toJson(QJsonDocument::Compact);
  QUrl url = apiQuery("bestellungen");
  Antiqua::Provider *prQuery = new Antiqua::Provider(this, false);
  prQuery->setObjectName("buchfreund_query_list");
  connect(prQuery, SIGNAL(responsed(const QJsonDocument &)), this,
          SIGNAL(listResponsed(const QJsonDocument &)));
  connect(prQuery, SIGNAL(finished()), prQuery, SLOT(deleteLater()));
  prQuery->sendPost(url, data);
}

void WHSoftJSonQuery::queryOrder(const QString &bfId) {
  QJsonObject obj;
  obj.insert("id", QJsonValue(bfId));
  QJsonDocument doc(obj);
  QByteArray data = doc.toJson(QJsonDocument::Compact);
  QUrl url = apiQuery("bestellung");
  Antiqua::Provider *prQuery = new Antiqua::Provider(this, false);
  prQuery->setObjectName("buchfreund_query_view");
  connect(prQuery, SIGNAL(responsed(const QJsonDocument &)), this,
          SIGNAL(orderResponsed(const QJsonDocument &)));
  connect(prQuery, SIGNAL(finished()), prQuery, SLOT(deleteLater()));
  prQuery->sendPost(url, data);
}

WHSoftWidget::WHSoftWidget(const QString &widgetId, QWidget *parent)
    : Antiqua::InterfaceWidget{widgetId, parent} {
  setObjectName(widgetId);
  setWindowTitle(widgetId);

}

void WHSoftWidget::setContent(const QJsonDocument &) { /* TODO */
}

void WHSoftWidget::checkCustomer() { /* TODO */
}

const QList<int> WHSoftWidget::getArticleIds() {
  QList<int> list;

  return list;
}

void WHSoft::prepareJsonListResponse(const QJsonDocument &doc) {
  bool errors = QJsonValue(doc["error"]).toBool();
  if (!errors) {
    QJsonArray senderArray;
    QJsonArray array = QJsonValue(doc["response"]).toArray();
    for (int i = 0; i < array.count(); i++) {
      QJsonObject convert;
      QJsonObject obj = array[i].toObject();
      convert.insert("id", obj["id"]);
      QDateTime d = QDateTime::fromString(obj["datum"].toString(), DATE_FORMAT);
      convert.insert("datum", QJsonValue(d.toString(Qt::ISODate)));
      senderArray.append(convert);
    }
    QJsonObject senderObject;
    senderObject.insert("provider", QJsonValue("Buchfreund"));
    senderObject.insert("items", senderArray);
    QJsonDocument jsDoc(senderObject);
    emit listResponse(jsDoc);
  }
}

bool WHSoft::createInterface(QObject *parent) {
  if (parent) {
    m_whsoft = new QObject(parent);
    m_whsoft->setObjectName("whsoft_plugin");
    return true;
  }
  return false;
}

Antiqua::InterfaceWidget *WHSoft::addWidget(const QString &widgetId,
                                            QWidget *parent) {
  WHSoftWidget *m_widget = new WHSoftWidget(widgetId, parent);
  m_widget->setObjectName(widgetId);
  return m_widget;
}

const QMap<QString, QString> WHSoft::fieldTranslate() const {
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
  map.insert("kundenkommentar", "m_customerComment");
  map.insert("lieferadresse", "c_shipping_address");
  // @}

  // public.article_orders @{
  map.insert("bestellnr", "a_article_id");
  map.insert("menge_bestellt", "a_count");
  map.insert("preis_pro_einheit", "a_sell_price");
  map.insert("datum", "a_modified");
  map.insert("titel", "a_title");
  // @}

  // public.inventory_orders @{
  map.insert("id", "o_provider_order_id");
  map.insert("provider", "o_provider_name");
  map.insert("menge_storniert", "o_cancellation");
  map.insert("stornogrund", "o_cancellation_text");
  map.insert("storniert_am", "o_cancellation_datetime");
  map.insert("lagerfach", "sl_storage");
  // @}
  return map;
}

const QString WHSoft::sqlParam(const QString &key) {
  QMap<QString, QString> map = fieldTranslate();
  QMap<QString, QString>::iterator fi;
  for (fi = map.begin(); fi != map.end(); ++fi) {
    if (fi.key() == key)
      return fi.value();
  }
  return QString();
}

const QString WHSoft::apiParam(const QString &key) {
  QMap<QString, QString> map = fieldTranslate();
  QMap<QString, QString>::iterator fi;
  for (fi = map.begin(); fi != map.end(); ++fi) {
    if (fi.value() == key)
      return fi.key();
  }
  return QString();
}

void WHSoft::queryMenueEntries() {
  WHSoftJSonQuery *mjs = new WHSoftJSonQuery(this);
  mjs->setObjectName("Buchfreund");
  connect(mjs, SIGNAL(listResponsed(const QJsonDocument &)), this,
          SLOT(prepareJsonListResponse(const QJsonDocument &)));

  mjs->queryList();
}
