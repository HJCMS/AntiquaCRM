// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef WHSOFTCONFIG_PLUGIN_H
#define WHSOFTCONFIG_PLUGIN_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>

/**
 * @def DATE_FORMAT
 * @ingroup Whsoft Plugin
 * @brief Datumsformat von Buchfreund.de
 * Wird für lesen/schreiben der JSon Datenfelder benötigt.
 */
#ifndef DATE_FORMAT
#define DATE_FORMAT "yyyy-MM-dd hh:mm:ss"
#endif

/**
 * @brief Konfigurationsgruppe
 * @ingroup Whsoft Plugin
 */
#ifndef CONFIG_GROUP
#define CONFIG_GROUP "provider/whsoft"
#endif

/**
 * @brief Wird für Menüeintrag und Gruppenzuweisung benötigt!
 * @ingroup Whsoft Plugin
 */
#ifndef CONFIG_PROVIDER
#define CONFIG_PROVIDER "Buchfreund"
#endif

#ifndef PLUGIN_WHSOFT_DEBUG
#define PLUGIN_WHSOFT_DEBUG false
#endif

/**
 * Wir verwenden keine Anrede in der Datenbank.
 * Es ist kein Adresskopf-Pflichtfeld, zu dem nicht mehr
 * Zeitgemäß. @see Landesgleichstellungsgesetz - LGG
 * In der Datenbank werden 4 Definitionen behandelt.
 * @li 0 = without disclosures => Keine Angaben
 * @li 1 = Male  => Männlich
 * @li 2 = Female => Weiblich
 * @li 3 = Various => Diverse
 * @return int
 */
static int genderFromString(const QString &anrede) {
  QString str = anrede.trimmed();
  if (str.startsWith("herr", Qt::CaseInsensitive) ||
      str.startsWith("mr.", Qt::CaseInsensitive))
    return 1;
  else if (str.startsWith("frau", Qt::CaseInsensitive) ||
           str.startsWith("mrs", Qt::CaseInsensitive))
    return 2;
  else
    return 0;
}

static QJsonDocument createUpdateArtcileCount(int id, int count) {
  QJsonObject obj;
  obj.insert("bestellnr", QJsonValue(QString::number(id)));
  obj.insert("bestand", QJsonValue(count));
  QJsonArray arr;
  arr.append(obj);

  QJsonObject sender;
  sender.insert("produkte", arr);

  QJsonDocument doc(sender);
  return doc;
}

#endif // WHSOFTCONFIG_PLUGIN_H
