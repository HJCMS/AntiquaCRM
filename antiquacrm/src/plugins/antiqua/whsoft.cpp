// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "whsoft.h"

bool WHSoft::createInterface(QObject *parent) {
  if (parent) {
    m_whsoft = new QObject(parent);
    m_whsoft->setObjectName("whsoft_plugin");
    return true;
  }
  return false;
}

const QUrl WHSoft::apiQuery(const QString &apiKey, const QString &operation) {
  QUrl url;
  url.setScheme("https");
  url.setHost("www.buchfreund.de");
  QString path("/verkaeufer/api/");
  path.append(apiKey + "/" + operation);
  return url;
}

const QMap<QString, QString> WHSoft::fieldTranslate() {
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
