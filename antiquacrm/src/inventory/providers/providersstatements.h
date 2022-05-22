// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERSSTATEMENTS_H
#define INVENTORY_PROVIDERSSTATEMENTS_H

#include <QMap>
#include <QObject>
#include <QString>

/**
 * @group Providers SQL Statements
 * Hier werden für das Dienstleistersystem die
 * SQL Abfragen definiert.
 */

/**
 * @def SHOW_SQL_QUERIES
 * @ingroup Providers SQL Statements
 * @section Debugging
 * @brief SQL Abfrageanzeige
 * Schaltet die SQL-Statements im Terminal
 * auf sichtbar.
 */
#ifndef SHOW_SQL_QUERIES
#define SHOW_SQL_QUERIES false
#endif

/**
 * @def BF_DATE_FORMAT
 * @ingroup Providers SQL Statements
 * @brief Datumsformat von Buchfreund.de
 * Wird benötigt für die Json anfragen zur erstellen.
 * Gleichzeitig wird es auch für den Import benötigt!
 */
#ifndef BF_DATE_FORMAT
#define BF_DATE_FORMAT "yyyy-MM-dd hh:mm:ss"
#endif

/**
 * @brief Übersetzt die Json Datenfelder zu den SQL Spalten.
 * @param key  - QJsonValue Schlüsselname
 * @param swap - Wenn true dann Umgekehrt suchen (wert == key)
 * @return SQL Spaltenname
 */
static const QString fieldTranslate(const QString &key, bool swap = false) {
  QMap<QString, QString> fields;
  // public.costumers @{
  fields.insert("vorname", "c_firstname");
  fields.insert("name", "c_lastname");
  fields.insert("adresse", "c_street");
  fields.insert("plz", "c_postalcode");
  fields.insert("ort", "c_location");
  fields.insert("land", "c_country");
  fields.insert("telefon", "c_phone_0");
  fields.insert("email", "c_email_0");
  // @}

  // public.article_orders @{
  fields.insert("id", "a_provider_id");
  fields.insert("bestellnr", "a_article_id");
  fields.insert("menge_bestellt", "a_count");
  fields.insert("preis_pro_einheit", "a_sell_price");
  fields.insert("datum", "a_modified");
  fields.insert("titel", "a_title");
  // @}

  QMap<QString, QString>::iterator fi;
  for (fi = fields.begin(); fi != fields.end(); ++fi) {
    if (swap) {
      if (fi.value() == key)
        return fi.key();
    } else {
      if (fi.key() == key)
        return fi.value();
    }
  }
  return QString();
}

#endif // INVENTORY_PROVIDERSSTATEMENTS_H
