// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

/**
 * @defgroup AntiquaCRM (Global definitions)
 * @{
 */

#ifndef ANTIQUACRM_GLOBAL_H
#define ANTIQUACRM_GLOBAL_H

#include <QtGlobal>
#include <QObject>
#include <QRegExp>
#include <QString>
#include <QIODevice>
#include <QSysInfo>

/**
 * @def ANTIQUACRM_NAME
 * @brief executable name
 * @note required by QCoreApplication and QSettings
 */
#ifndef ANTIQUACRM_NAME
#define ANTIQUACRM_NAME "@PROJECTNAME@"
#endif

/**
 * @def HJCMS_CONFIG_DOMAIN
 * @brief Required by ASettings
 * @example ~/.config/{HJCMS_CONFIG_DOMAIN}/{HJCMS_CONFIG_NAME}/{HJCMS_CONFIG_VERSION}
 */
#ifndef HJCMS_CONFIG_DOMAIN
#define HJCMS_CONFIG_DOMAIN "de.hjcms"
#endif

#ifndef HJCMS_CONFIG_NAME
#define HJCMS_CONFIG_NAME "@PROJECTNAME@"
#endif

#ifndef HJCMS_CONFIG_VERSION
#define HJCMS_CONFIG_VERSION "@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@"
#endif

/**
 * @def ANTIQUACRM_CONNECTION_DOMAIN
 * @brief required by PostgreSQL, D-Bus and TCP-Socket
 */
#ifndef ANTIQUACRM_CONNECTION_DOMAIN
#define ANTIQUACRM_CONNECTION_DOMAIN "de.hjcms.@PROJECTNAME@"
#endif

/**
 * @def ANTIQUACRM_USERAGENT
 * @brief required by NetworkAccessManager
 * @warning This fixed string is needet for OAuth
 */
#ifndef ANTIQUACRM_USERAGENT
#define ANTIQUACRM_USERAGENT "AntiquaCRM/@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@"
#endif

/**
 * @def ANTIQUACRM_VERSION
 * @brief versioning
 */
#ifndef ANTIQUACRM_VERSION
#define ANTIQUACRM_VERSION "@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@.@ANTIQUACRM_VERSION_RELEASE@"
#endif

/**
 * @def ANTIQUACRM_DISPLAYNAME
 * @brief Window title
 */
#ifndef ANTIQUACRM_DISPLAYNAME
#define ANTIQUACRM_DISPLAYNAME "@DISPLAYNAME@"
#endif

#ifndef ANTIQUACRM_WINDOW_TITLE
#define ANTIQUACRM_WINDOW_TITLE "@DISPLAYNAME@ (v@ANTIQUACRM_VERSION_STRING@)"
#endif

/**
 * @def ANTIQUACRM_HOMEPAGE
 * @brief Support link
 */
#ifndef ANTIQUACRM_HOMEPAGE
#define ANTIQUACRM_HOMEPAGE "@HOMEPAGEURL@"
#endif

/**
 * @def ANTIQUACRM_INTERFACE
 * @brief Plugin Interface Description
 */
#ifndef ANTIQUACRM_INTERFACE
#define ANTIQUACRM_INTERFACE "de.hjcms.antiquacrm.ProviderInterface"
#endif

/**
 * @def ANTIQUACRM_DATA_TARGET
 * @brief Shared Data location
 */
#ifndef ANTIQUACRM_DATA_TARGET
#define ANTIQUACRM_DATA_TARGET "@ANTIQUACRM_DATA_TARGET@"
#endif

/**
 * @def ANTIQUACRM_PLUGIN_TARGET
 * @brief Plugins target
 */
#ifndef ANTIQUACRM_PLUGIN_TARGET
#define ANTIQUACRM_PLUGIN_TARGET "@ANTIQUACRM_PLUGIN_TARGET@"
#endif

/**
 * @def ANTIQUACRM_TRANSLATION_TARGET
 * @brief Translations target
 */
#ifndef ANTIQUACRM_TRANSLATION_TARGET
#define ANTIQUACRM_TRANSLATION_TARGET "@ANTIQUACRM_TRANSLATION_TARGET@"
#endif

/**
 * @def ANTIQUACRM_SOCKET_TIMEOUT
 * @brief Ablaufzeit für die Socketverbindungen
 */
#ifndef ANTIQUACRM_SOCKET_TIMEOUT
#ifdef Q_OS_WIN
#define ANTIQUACRM_SOCKET_TIMEOUT 3000
#else
#define ANTIQUACRM_SOCKET_TIMEOUT 1500
#endif
#endif

/**
 * @def ANTIQUACRM_TRANSMITTER_MODE
 * @brief Standard Modus für Verbindungen
 */
#ifndef ANTIQUACRM_TRANSMITTER_MODE
#define ANTIQUACRM_TRANSMITTER_MODE QIODevice::WriteOnly
#endif

/**
 * @def ANTIQUACRM_EXPORT
 * @brief library export definition
 */
#ifdef ANTIQUACRM_SHARED_LIBRARY
# define ANTIQUACRM_LIBRARY Q_DECL_EXPORT
#else
# define ANTIQUACRM_LIBRARY Q_DECL_IMPORT
#endif

/**
 * @brief Import Format for PGSQL::CURRENT_TIMESTAMP
 * @note AntiquaCRM didn't use Time Zones in SQL tables!
 */
#ifndef ANTIQUACRM_TIMESTAMP_IMPORT
#define ANTIQUACRM_TIMESTAMP_IMPORT "dd.MM.yyyy hh:mm:ss.zzz"
#endif

/**
 * @def ANTIQUACRM_DATETIME_FORMAT
 * @ingroup Antiqua Plugin Interface
 * @brief Das vom System vorgegebene Datumsformat ist ISO 8601!
 * Soll verhindern das bei der Umwandlung von einem String, die Konvertierung nicht fehlschlägt!
 * @code
 *  QJsonObject import;
 *  QJsonObject obj = jsonArray[i].toObject();
 *  QDateTime d = QDateTime::fromString(obj["datetime"].toString(), "yyyy-MM-dd hh:mm:ss");
 *  import.insert("datetime", QJsonValue(d.toString(ANTIQUACRM_DATETIME_EXPORT)));
 * @endcode
 */
#ifndef ANTIQUACRM_DATETIME_EXPORT
#define ANTIQUACRM_DATETIME_EXPORT Qt::ISODate
#endif

/**
 * @brief Datums und Zeitangaben für die UI Anzeige
 */
#ifndef ANTIQUACRM_DATETIME_TOOLTIP
#define ANTIQUACRM_DATETIME_TOOLTIP "dd.MM.yyyy hh:mm:ss"
#endif

#ifndef ANTIQUACRM_DATETIME_DISPLAY
#define ANTIQUACRM_DATETIME_DISPLAY "dddd dd. MMMM yyyy hh:mm"
#endif

#ifndef ANTIQUACRM_DATE_DISPLAY
#define ANTIQUACRM_DATE_DISPLAY "dddd dd. MMMM yyyy"
#endif

#ifndef ANTIQUACRM_SHORT_DATE_DISPLAY
#define ANTIQUACRM_SHORT_DATE_DISPLAY "dd MMM yyyy"
#endif

/**
 * @def ANTIQUACRM_QUERY_PASTDAYS
 * @ingroup Antiqua Plugin Interface
 * @brief Dienstleisteranfragen der letzten ... Tage anzeigen.
 */
#ifndef ANTIQUACRM_QUERY_PASTDAYS
#define ANTIQUACRM_QUERY_PASTDAYS -4
#endif

/**
 * @def ANTIQUACRM_TEXTCODEC
 * @brief Default Charset for Database Connections, Application files, is utf8!
 */
#ifndef ANTIQUACRM_TEXTCODEC
#define ANTIQUACRM_TEXTCODEC "UTF8"
#endif

namespace AntiquaCRM {
 Q_NAMESPACE

 /**
  * @brief prepare and normalize single strings
  */
 static const QString trim(const QString &str) {
  QString out = str;
  out.replace(QRegExp("\\s+"), " ");
  return out.trimmed();
 }

 /**
  * @brief Regular Expression for eMail
  */
 static const QRegExp mailRegExp() {
  QRegExp reg;
  reg.setCaseSensitivity(Qt::CaseInsensitive);
  reg.setPattern("^([\\d\\w\\-\\.]{3,})@([\\d\\w\\-\\.]{3,})\\.([a-z]{2,6})$");
  return reg;
 }

 /**
  * @brief Remove none Numbers from Mobile/Phone input
  */
 static const QString stripPhone(const QString &phone) {
  QString out = phone.trimmed();
  out.replace(QRegExp("\\D+"), "");
  return out.trimmed();
 }

 /**
  * @brief Regular Expression for Phonenumbers
  * @note No url.scheme() supported!
  */
 static const QRegExp phoneRegExp() {
  QRegExp reg;
  reg.setPattern("^([\\d]{6}[\\d]+)$");
  return reg;
 }


 /**
  * @brief Socket serverName
  */
 static const QString antiquaSocketName() {
   QString name(ANTIQUACRM_CONNECTION_DOMAIN);
   name.append(".");
   name.append(QString::fromLocal8Bit(QSysInfo::machineUniqueId()));
   QString userName;
#ifdef Q_OS_LINUX
   userName = qEnvironmentVariable("USER").trimmed().replace(" ", "");
   if (!userName.isEmpty()) {
     name.append(".");
     name.append(userName);
   }
#endif
   return name;
 }

 /**
  * @brief Nachrichtentyp
  * Definiert die Dringlichkeit einer Nachricht an das Meldungssystem!
  */
 enum Message {
  NORMAL = 0,  /**< Standard Info */
  WARNING = 1, /**< Warnung wird ausgegeben! */
  FATAL = 2,   /**< Schwehrwiegender Fehler ist aufgetreten! */
  LOGGING = 3  /**< Nur für das Protokollieren vorgesehen! */
 };
 Q_ENUM_NS(AntiquaCRM::Message)

 /**
  * @brief Typenangabe für die Netzwerkanfragen Schnittstelle!
  */
 enum PluginQueryType { JSON_QUERY = 0, XML_QUERY = 1 };
 Q_ENUM_NS(AntiquaCRM::PluginQueryType)

 /**
  * @brief Auftrags Status
  * @section orders
  * Wird im Auftragssystem verwendet!
  */
 enum OrderStatus {
   OPEN = 0,      /**< Offen */
   STARTED = 1,   /**< Auftrag angenommen */
   FETCHET = 2,   /**< Bereit zur Abholung */
   DELIVERED = 3, /**< Ausgeliefert */
   REMINDET = 4,  /**< Erinnerung */
   COMPLETED = 5, /**< Abgeschlossen */
   CANCELED = 6,  /**< Storniert */
   RETURNING = 7  /**< Retour */
 };
 Q_ENUM_NS(AntiquaCRM::OrderStatus)

 /**
  * @brief Bestellstatus des Dienstleisters
  * @section orders
  * Wird im Auftragssystem verwendet!
  */
 enum PaymentStatus {
   STATUS_NOT_SET = 0,           /**< Kein Status fesgelegt */
   WAIT_FOR_PAYMENT = 1,         /**< Warte auf Zahlung */
   SHIPMENT_CREATED = 2,         /**< Fertig zum Versand */
   SHIPPED_WAIT_FOR_PAYMENT = 3, /**< Geliefert warte auf Zahlung */
   SHIPPED_AND_PAID = 4,         /**< Bezahlt und versendet */
   BUYER_NO_REACTION = 5,        /**< Keine Reaktion des Käufers */
   ORDER_CANCELED = 6            /**< Käufer hat die Bestellung storniert */
 };
 Q_ENUM_NS(AntiquaCRM::PaymentStatus)

 /**
  * @brief Zahlungsart der Bestellung
  * @section orders
  * Wird im Auftragssystem verwendet um die Zahlungsart zu Zeigen.
  * Die Umsetzung dieser Vorgabe liegt bei der Plugin Integration!
  */
 enum PaymentMethod {
   PAYMENT_NOT_SET = 0,              /**< Keine Auswahl vorhanden */
   BANK_PREPAYMENT = 1,              /**< Banküberweisung Vorkasse */
   DELIVER_WITH_INVOICE = 2,         /**< Lieferung mit offener Rechnung */
   DIRECT_DEBIT_PREPAYMENT = 3,      /**< Lastschrift Vorkasse */
   CREDIT_CARD_PREPAYMENT = 4,       /**< Kreditkarte Vorkasse */
   CASH_ON_DELIVERY = 5,             /**< Zahlung mit Nachnahme */
   PAYPAL_PREPAYMENT = 6,            /**< PayPal Vorkasse */
   SKRILL_PREPAYMENT = 7,            /**< Skrill Vorkasse */
   GIROPAY_PREPAYMENT = 8,           /**< GiroPay Vorkasse */
   GOOGLEPAY_PREPAYMENT = 9,         /**< GooglePay Vorkasse */
   UNKNOWN_PREPAYMENT = 10,          /**< Vorkasse unbekannter Anbieter */
   LOCAL_PICKUP_CASH_PAYMENT = 11,   /**< Selbstabholung - Barzahlung */
   INSTANT_BANK_TRANSFER = 12,       /**< Sofortüberweisung */
   INVOICE_PREPAYMENT_RESERVED = 13, /**< Offene Rechnung - Vorkasse vorbehalten */
   CHECK_PREPAYMENT = 14,            /**< Scheck - Vorkasse */
 };
 Q_ENUM_NS(AntiquaCRM::PaymentMethod)

 /**
  * @brief Kunden Vertrauensebene festlegen!
  * @section customers
  */
 enum CustomerTrustLevel {
   NO_TRUST_LEVEL = 0, /**< Keine Vertrauensebene festgelegt */
   ON_TIME = 1,        /**< Zahlt Pünktlich */
   RELIABLE = 2,       /**< Zahlt Zuverlässig */
   WITH_DELAY = 3,     /**< Zahlt mit Verspätung */
   PREPAYMENT = 4,     /**< Nur mit Vorauszahlung */
   NO_DELIVERY = 5     /**< Keine Lieferung */
 };
 Q_ENUM_NS(AntiquaCRM::CustomerTrustLevel)

 /**
  * @brief Geschlechter Definition
  * @section customers
  */
 enum Gender {
  NO_GENDER = 0, /**< Ohne Angabe */
  MALE = 1,      /**< Männlich */
  FEMALE = 2,    /**< Weiblich */
  VARIOUS = 3    /**< Diverse */
 };
 Q_ENUM_NS(AntiquaCRM::Gender)

 /**
  * @brief Inventory Gruppen
  * Umsetzung für Dienstleister kategorien.
  * @section inventory
  */
 enum ArticleType {
  UNKNOWN = 0,   /**< nicht festgelegt */
  BOOK = 1,      /**< Bücher */
  MEDIA = 2,     /**< Film & Tonträger */
  PRINTS = 3,    /**< Drucke & Stiche */
  OTHER = 4      /**< Diverse */
 };
 Q_ENUM_NS(AntiquaCRM::ArticleType)

 /**
  * @brief Zustandsbeschreibung für Artikel
  * @section inventory
  */
 enum Condition {
  NO_CONDITION = 0, /**< Ohne Angabe */
  FINE = 1,         /**< Sehr gut, fast neuwertig! */
  GOOD = 2,         /**< Leichte Gebrauchsspuren */
  SATISFYING = 3,   /**< Deutliche Gebrauchsspuren */
  SUFFICIENT = 4    /**< Stark abgenutzt */
 };
 Q_ENUM_NS(AntiquaCRM::Condition)

 /**
  * @brief Postleitzahlen Eintrag
  * @section widgets
  */
 struct ANTIQUACRM_LIBRARY PostalCode {
   QString plz;
   QString location;
   QString state;
 };

 struct ANTIQUACRM_LIBRARY APluginConfig {
   QString hostname = QString();
   QString querypath = QString();
   QString username = QString();
   QString apiKey = QString();
   int port = 443;
   int history = -7;
 };

};

Q_DECLARE_METATYPE(AntiquaCRM::PostalCode)

#endif // ANTIQUACRM_GLOBAL_H

/** @} */
