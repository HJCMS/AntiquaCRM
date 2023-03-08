// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

/**
 * @defgroup core AntiquaCRM Core Library
 * @brief AntiquaCRM core components
 * @{
 */

#ifndef ANTIQUACRM_GLOBAL_H
#define ANTIQUACRM_GLOBAL_H

#include <QtGlobal>
#include <QObject>
#include <QString>
#include <QIODevice>
#ifdef ANTIQUACRM_DBUS_ENABLED
#include <QDBusAbstractAdaptor>
#else
/**
 * @brief QDBus Macro - fake for windows
 */
#define Q_NOREPLY
#endif

/**
 * @brief executable name
 * @note required by QCoreApplication and QSettings
 */
#ifndef ANTIQUACRM_NAME
#define ANTIQUACRM_NAME "@PROJECTNAME@"
#endif

/**
 * @brief Required by ASettings
 * @code
 *  ~/.config/{HJCMS_CONFIG_DOMAIN}/{HJCMS_CONFIG_NAME}/{HJCMS_CONFIG_VERSION}
 * @endcode
 */
#ifndef HJCMS_CONFIG_DOMAIN
#define HJCMS_CONFIG_DOMAIN "de.hjcms"
#endif

/**
 * @brief Configuration Namespace
 */
#ifndef HJCMS_CONFIG_NAME
#define HJCMS_CONFIG_NAME "@PROJECTNAME@"
#endif

/**
 * @brief Configuration Application ID
 */
#ifndef HJCMS_CONFIG_APPID
#define HJCMS_CONFIG_APPID "@ANTIQUACRM_APPID@"
#endif

/**
 * @brief Configuration DNS ID
 */
#ifndef HJCMS_CONFIG_DNSID
#define HJCMS_CONFIG_DNSID "@ANTIQUACRM_DNSID@"
#endif

/**
 * @brief Configuration URL ID
 */
#ifndef HJCMS_CONFIG_URLID
#define HJCMS_CONFIG_URLID "@ANTIQUACRM_URLID@"
#endif

/**
 * @brief Config version
 */
#ifndef HJCMS_CONFIG_VERSION
#define HJCMS_CONFIG_VERSION "@ANTIQUACRM_VERSION_MAJOR@.0"
#endif

/**
 * @brief required by PostgreSQL, D-Bus and TCP-Socket
 */
#ifndef ANTIQUACRM_CONNECTION_DOMAIN
#define ANTIQUACRM_CONNECTION_DOMAIN "de.hjcms.@PROJECTNAME@"
#endif

/**
 * @brief required by NetworkAccessManager
 * @warning This fixed string is needet for OAuth
 */
#ifndef ANTIQUACRM_USERAGENT
#define ANTIQUACRM_USERAGENT "AntiquaCRM/@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@"
#endif

/**
 * @brief versioning
 */
#ifndef ANTIQUACRM_VERSION
#define ANTIQUACRM_VERSION "@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@.@ANTIQUACRM_VERSION_RELEASE@"
#endif

/**
 * @brief Window title
 */
#ifndef ANTIQUACRM_DISPLAYNAME
#define ANTIQUACRM_DISPLAYNAME "@DISPLAYNAME@"
#endif

/**
 * @brief Complete AntiquaCRM Window title with version
 */
#ifndef ANTIQUACRM_WINDOW_TITLE
#define ANTIQUACRM_WINDOW_TITLE "@DISPLAYNAME@ (v@ANTIQUACRM_VERSION_STRING@)"
#endif

/**
 * @brief Support link
 */
#ifndef ANTIQUACRM_HOMEPAGE
#define ANTIQUACRM_HOMEPAGE "@HOMEPAGEURL@"
#endif

/**
 * @brief Plugin Interface Description
 */
#ifndef ANTIQUACRM_INTERFACE
#define ANTIQUACRM_INTERFACE "de.hjcms.antiquacrm.ProviderInterface"
#endif

/**
 * @brief Shared Data location
 */
#ifndef ANTIQUACRM_DATA_TARGET
#define ANTIQUACRM_DATA_TARGET "@ANTIQUACRM_DATA_TARGET@"
#endif

/**
 * @brief Plugins target
 */
#ifndef ANTIQUACRM_PLUGIN_TARGET
#define ANTIQUACRM_PLUGIN_TARGET "@ANTIQUACRM_PLUGIN_TARGET@"
#endif

/**
 * @brief Translations target
 */
#ifndef ANTIQUACRM_TRANSLATION_TARGET
#define ANTIQUACRM_TRANSLATION_TARGET "@ANTIQUACRM_TRANSLATION_TARGET@"
#endif

/**
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
 * @brief Standard Modus für Verbindungen
 */
#ifndef ANTIQUACRM_TRANSMITTER_MODE
#define ANTIQUACRM_TRANSMITTER_MODE QIODevice::WriteOnly
#endif

/**
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

/**
 * @brief Long Date time Display format
 */
#ifndef ANTIQUACRM_DATETIME_DISPLAY
#define ANTIQUACRM_DATETIME_DISPLAY "dddd dd. MMMM yyyy hh:mm"
#endif

/**
 * @brief Normal Date time Display format
 */
#ifndef ANTIQUACRM_DATE_DISPLAY
#define ANTIQUACRM_DATE_DISPLAY "dddd dd. MMMM yyyy"
#endif

/**
 * @brief Short Date time Display format
 */
#ifndef ANTIQUACRM_SHORT_DATE_DISPLAY
#define ANTIQUACRM_SHORT_DATE_DISPLAY "dd MMM yyyy"
#endif

/**
 * @brief Dienstleisteranfragen der letzten ... Tage anzeigen.
 */
#ifndef ANTIQUACRM_QUERY_PASTDAYS
#define ANTIQUACRM_QUERY_PASTDAYS -4
#endif

/**
 * @brief Default Charset for Database Connections, Application files, is utf8!
 */
#ifndef ANTIQUACRM_TEXTCODEC
#define ANTIQUACRM_TEXTCODEC "UTF8"
#endif

/**
 * @namespace AntiquaCRM
 * @brief Antiqua CRM Namespace
 */
namespace AntiquaCRM {
 Q_NAMESPACE

 /**
  * @defgroup Enumerations AntiquaCRM Enumerations
  * @brief Enumerations in AntiquaCRM Namespace
  */

 /**
  * @ingroup Enumerations
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
  * @ingroup Enumerations
  * @brief Typenangabe für die Netzwerkanfragen Schnittstelle!
  */
 enum NetworkQueryType { JSON_QUERY = 0, XML_QUERY = 1 };
 Q_ENUM_NS(AntiquaCRM::NetworkQueryType)

 /**
  * @ingroup Enumerations
  * @brief Auftrag-Bearbeitungsstatus
  */
 enum OrderStatus {
   OPEN = 0,      /**< Offen */
   STARTED = 1,   /**< Auftrag angenommen */
   FETCHET = 2,   /**< Bereit zur Abholung */
   DELIVERY = 3,  /**< Lieferung ist Unterwegs */
   DELIVERED = 4, /**< Paket wurde geliefert */
   COMPLETED = 5, /**< @deprecated Lieferung abgeschlossen */
   CANCELED = 6   /**< Storniert */
 };
 Q_ENUM_NS(AntiquaCRM::OrderStatus)

 /**
  * @ingroup Enumerations
  * @brief Auftrag-Zahlungsstatus
  */
 enum OrderPayment {
   NOTPAID = 0,  /**< Warte auf Zahlung */
   PAYED = 1,    /**< Bezahlt */
   REMIND = 2,   /**< Erinnert */
   ADMONISH = 3, /**< Mahnen */
   RETURN = 4,   /**< Retour */
   COLLPROC = 5  /**< Collection procedures/Inkassoverfahren */
 };
 Q_ENUM_NS(AntiquaCRM::OrderPayment)

 /**
  * @ingroup Enumerations
  * @brief Bestellstatus des Dienstleisters
  * Wird im Auftragssystem verwendet!
  */
 enum ProviderPaymentStatus {
   STATUS_NOT_SET = 0,           /**< Kein Status fesgelegt */
   WAIT_FOR_PAYMENT = 1,         /**< Warte auf Zahlung */
   SHIPMENT_CREATED = 2,         /**< Fertig zum Versand */
   SHIPPED_WAIT_FOR_PAYMENT = 3, /**< Geliefert warte auf Zahlung */
   SHIPPED_AND_PAID = 4,         /**< Bezahlt und versendet */
   BUYER_NO_REACTION = 5,        /**< Keine Reaktion des Käufers */
   ORDER_CANCELED = 6            /**< Käufer hat die Bestellung storniert */
 };
 Q_ENUM_NS(AntiquaCRM::ProviderPaymentStatus)

 /**
  * @ingroup Enumerations
  * @brief Zahlungsart der Bestellung
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
  * @ingroup Enumerations
  * @brief Kunden Vertrauensebene festlegen!
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
  * @ingroup Enumerations
  * @brief Geschlechter Definition
  */
 enum Gender {
  NO_GENDER = 0, /**< Ohne Angabe */
  MALE = 1,      /**< Männlich */
  FEMALE = 2,    /**< Weiblich */
  VARIOUS = 3    /**< Diverse */
 };
 Q_ENUM_NS(AntiquaCRM::Gender)

 /**
  * @ingroup Enumerations
  * @brief Inventory Gruppen
  * Umsetzung für Dienstleister kategorien.
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
  * @ingroup Enumerations
  * @brief SalesTax
  * Rechnungsmöglichkeiten bei der Umsatzsteuerangabe (UStG §14)
  */
 enum SalesTax {
  TAX_NOT = 0,  /**< without sales tax */
  TAX_INCL = 1, /**< including sales tax */
  TAX_WITH = 2  /**< with sales tax  */
 };
 Q_ENUM_NS(AntiquaCRM::SalesTax)

 /**
  * @ingroup Enumerations
  * @brief MediaType
  * Umsetzung Musik Medien
  */
 enum MediaType {
  MEDIA_NO_TYPE = 0,            /**< nicht festgelegt */
  MEDIA_DISC_COMPACT = 1,       /**< Compact Disc */
  MEDIA_DISC_MINI = 2,          /**< Mini Compact Disc */
  MEDIA_DISC_VIDEO = 3,         /**< Video Compact Disc */
  MEDIA_VINYL_SINGLE = 4,       /**< Vinyl single record */
  MEDIA_VINYL_LONGPLAY = 5,     /**< Vinyl longplay record */
  MEDIA_VINYL_MULTI_GROOVE = 6, /**< Vinyl Multi Groove */
  MEDIA_VINYL_COLORED = 7,      /**< Vinyl colored record */
  MEDIA_VINYL_VIDEO = 8,        /**< Vinyl video record */
  MEDIA_VINYL_DISC = 9          /**< Vinyl record and DISC */
 };
 Q_ENUM_NS(AntiquaCRM::MediaType)

 /**
  * @ingroup Enumerations
  * @brief Zustandsbeschreibung für Artikel
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
  * @ingroup core
  * @class PostalCode
  * @brief PostalCode entries
  */
 struct ANTIQUACRM_LIBRARY PostalCode {
   QString plz;
   QString location;
   QString state;
 };

};

Q_DECLARE_METATYPE(AntiquaCRM::PostalCode)

#endif // ANTIQUACRM_GLOBAL_H

/**
 * @}
 */
