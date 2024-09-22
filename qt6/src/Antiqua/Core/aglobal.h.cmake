// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

/**
 * @mainpage @DISPLAYNAME@
 * API Documentation for „AntiquaCRM“ core library.
 */

#ifndef ANTIQUACRM_GLOBAL_H
#define ANTIQUACRM_GLOBAL_H

#include <QObject>
#include <QtGlobal>
#include <QDate>
#include <QString>
#include <QIODevice>
#include <QMetaType>
#include <QByteArray>
#include <QFlags>

/**
 * @ingroup AMacros
 * @brief default executable name
 * @short Required by qApp-&gt;setApplicationName() and QSettings class.
 */
#ifndef ANTIQUACRM_NAME
#define ANTIQUACRM_NAME "@PROJECTNAME@"
#endif

/**
 * @ingroup AMacros
 * @brief Application Domain in user configuration path
 * @code
 *  ~/.config/{HJCMS_CONFIG_DOMAIN}/{HJCMS_CONFIG_NAME}/{HJCMS_CONFIG_VERSION}
 * @endcode
 */
#ifndef HJCMS_CONFIG_DOMAIN
#define HJCMS_CONFIG_DOMAIN "de.hjcms"
#endif

/**
 * @ingroup AMacros
 * @brief Application name in user configuration path
 */
#ifndef HJCMS_CONFIG_NAME
#define HJCMS_CONFIG_NAME "@PROJECTNAME@"
#endif

/**
 * @ingroup AMacros
 * @brief Application Version in user configuration path
 * @short config version for targeting the location 
 *  of the current user configuration file.
 */
#ifndef HJCMS_CONFIG_VERSION
#define HJCMS_CONFIG_VERSION "@ANTIQUACRM_VERSION_MAJOR@.1"
#endif

/**
 * @ingroup AMacros
 * @brief Configuration Application ID
 */
#ifndef HJCMS_CONFIG_APPID
#define HJCMS_CONFIG_APPID "@ANTIQUACRM_APPID@"
#endif

/**
 * @ingroup AMacros
 * @brief Configuration DNS ID
 */
#ifndef HJCMS_CONFIG_DNSID
#define HJCMS_CONFIG_DNSID "@ANTIQUACRM_DNSID@"
#endif

/**
 * @ingroup AMacros
 * @brief Configuration URL ID
 */
#ifndef HJCMS_CONFIG_URLID
#define HJCMS_CONFIG_URLID "@ANTIQUACRM_URLID@"
#endif

/**
 * @ingroup AMacros
 * @brief required by PostgreSQL, D-Bus, TCP-Socket and Interfaces
 */
#ifndef ANTIQUACRM_CONNECTION_DOMAIN
#define ANTIQUACRM_CONNECTION_DOMAIN "de.hjcms.@PROJECTNAME@"
#endif

/**
 * @ingroup AMacros
 * @brief required by NetworkAccessManager
 * @warning This fixed string is needet for OAuth
 */
#ifndef ANTIQUACRM_USERAGENT
#define ANTIQUACRM_USERAGENT "AntiquaCRM/@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@"
#endif

/**
 * @ingroup AMacros
 * @brief Application Version String
 */
#ifndef ANTIQUACRM_VERSION
#define ANTIQUACRM_VERSION "@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@.@ANTIQUACRM_VERSION_RELEASE@"
#endif

/**
 * @ingroup AMacros
 * @brief Application Display title
 */
#ifndef ANTIQUACRM_DISPLAYNAME
#define ANTIQUACRM_DISPLAYNAME "@DISPLAYNAME@"
#endif

/**
 * @ingroup AMacros
 * @brief Application Window title inkl. Version string
 */
#ifndef ANTIQUACRM_WINDOW_TITLE
#define ANTIQUACRM_WINDOW_TITLE "@DISPLAYNAME@ (v@ANTIQUACRM_VERSION_STRING@)"
#endif

/**
 * @ingroup AMacros
 * @brief Project Homepage url
 */
#ifndef ANTIQUACRM_HOMEPAGE
#define ANTIQUACRM_HOMEPAGE "@HOMEPAGEURL@"
#endif

/**
 * @ingroup AMacros
 * @brief Shared Data location
 */
#ifndef ANTIQUACRM_DATA_TARGET
#define ANTIQUACRM_DATA_TARGET "@ANTIQUACRM_DATA_TARGET@"
#endif

/**
 * @ingroup AMacros
 * @brief Plugins target
 */
#ifndef ANTIQUACRM_PLUGIN_TARGET
#define ANTIQUACRM_PLUGIN_TARGET "@ANTIQUACRM_PLUGIN_TARGET@"
#endif

/**
 * @ingroup AMacros
 * @brief Translations target
 */
#ifndef ANTIQUACRM_TRANSLATION_TARGET
#define ANTIQUACRM_TRANSLATION_TARGET "@ANTIQUACRM_TRANSLATION_TARGET@"
#endif

/**
 * @ingroup AMacros
 * @brief Expiry time for the socket connections
 */
#ifndef ANTIQUACRM_SOCKET_TIMEOUT
#ifdef Q_OS_WIN
#define ANTIQUACRM_SOCKET_TIMEOUT 3000
#else
#define ANTIQUACRM_SOCKET_TIMEOUT 1500
#endif
#endif

/**
 * @ingroup AMacros
 * @brief Standard mode for socket connections
 */
#ifndef ANTIQUACRM_TRANSMITTER_MODE
#define ANTIQUACRM_TRANSMITTER_MODE QIODevice::WriteOnly
#endif

/**
 * @ingroup AMacros
 * @brief library export definition
 */
#ifndef ANTIQUACRM_SHARED_STATIC
# ifdef ANTIQUACRM_SHARED_LIBRARY
#   define ANTIQUACRM_LIBRARY Q_DECL_EXPORT
# else
#   define ANTIQUACRM_LIBRARY Q_DECL_IMPORT
# endif
#else
# define ANTIQUACRM_LIBRARY
#endif

/**
 * @ingroup AMacros
 * @brief Default initialisation Date
 * 
 * Used in DateEdit and DateTimeEdit Widgets for global fallback detection.
 */
#ifndef ANTIQUACRM_DEFAULT_STARTDATE
#define ANTIQUACRM_DEFAULT_STARTDATE QDate(1900, 01, 01)
#endif

/**
 * @ingroup AMacros
 * @brief Import Format for PGSQL::CURRENT_TIMESTAMP
 * @note AntiquaCRM not using Time Zones in SQL Tables!
 */
#ifndef ANTIQUACRM_TIMESTAMP_IMPORT
#define ANTIQUACRM_TIMESTAMP_IMPORT "dd.MM.yyyy hh:mm:ss.zzz"
#endif

/**
 * @ingroup AMacros
 * @brief The date format specified by the system is ISO-8601!
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
 * @ingroup AMacros
 * @brief Date and time information for the UI display.
 */
#ifndef ANTIQUACRM_DATETIME_TOOLTIP
#define ANTIQUACRM_DATETIME_TOOLTIP "dd.MM.yyyy hh:mm:ss"
#endif

/**
 * @ingroup AMacros
 * @brief Long date time display format
 */
#ifndef ANTIQUACRM_DATETIME_DISPLAY
#define ANTIQUACRM_DATETIME_DISPLAY "dddd dd. MMMM yyyy hh:mm"
#endif

/**
 * @ingroup AMacros
 * @brief Normal date time display format
 */
#ifndef ANTIQUACRM_DATE_DISPLAY
#define ANTIQUACRM_DATE_DISPLAY "dddd dd. MMMM yyyy"
#endif

/**
 * @ingroup AMacros
 * @brief Short date time display format
 */
#ifndef ANTIQUACRM_SHORT_DATE_DISPLAY
#define ANTIQUACRM_SHORT_DATE_DISPLAY "dd MMM yyyy"
#endif

/**
 * @ingroup AMacros
 * @brief Default Charset for Database Connections, Application files, is utf8!
 */
#ifndef ANTIQUACRM_CHARSET
#define ANTIQUACRM_CHARSET QByteArray("UTF8")
#endif

/**
 * @ingroup AMacros
 * @brief Predefined configuration parameters for path settings.
 * @code
 *   AntiquaCRM::ASettings *m_config = new AntiquaCRM::ASettings(this);
 *   m_config->beginGroup(ANTIQUACRM_ARCHIVE_CONFIG_DIRS);
 *   qDebug() << m_config->value(ANTIQUACRM_ARCHIVE_IMAGES);
 *   m_config->endGroup();
 * @endcode
 */
#ifndef ANTIQUACRM_ARCHIVE_CONFIG_DIRS
#define ANTIQUACRM_ARCHIVE_CONFIG_DIRS "dirs"
#define ANTIQUACRM_ARCHIVE_IMAGES "archive_images"
#define ANTIQUACRM_ARCHIVE_DELIVERY "archive_delivery"
#define ANTIQUACRM_ARCHIVE_CARDS "archive_cards"
#define ANTIQUACRM_ARCHIVE_INVOICES "archive_invoices"
#define ANTIQUACRM_ARCHIVE_REMINDERS "archive_reminder"
#define ANTIQUACRM_ARCHIVE_REPORTS "archive_reports"
#define ANTIQUACRM_ARCHIVE_IMPORT "import_folder"
#endif

#ifdef ANTIQUACRM_DBUS_ENABLED
#include <QDBusAbstractAdaptor>
#else
/**
 * @ingroup AMacros
 * @brief Macro-fake for Windows
 * @short Redefined macro to prevent errors when compile on windows
 */
#define Q_NOREPLY
#endif

/**
 * @namespace AntiquaCRM
 * @brief AntiquaCRM Core Library namespace
 */
namespace AntiquaCRM {
Q_NAMESPACE

  /**
   * @brief Type specification for the network request interface!
   * @ingroup Enumerations
   */
  enum NetworkQueryType { JSON_QUERY = 0, XML_QUERY = 1 };
  Q_ENUM_NS(NetworkQueryType)

  /**
   * @brief Order processing status
   * @ingroup Enumerations
   */
  enum OrderStatus {
    OPEN = 0,      /**< open */
    STARTED = 1,   /**< order accepted */
    FETCHET = 2,   /**< ready for pickup */
    DELIVERY = 3,  /**< delivery is on the way */
    DELIVERED = 4, /**< package has been delivered */
    COMPLETED = 5, /**< @deprecated delivery completed */
    CANCELED = 6   /**< canceled */
  };
  Q_ENUM_NS(OrderStatus)

  /**
   * @brief Order payment status
   * @ingroup Enumerations
   */
  enum OrderPayment {
    NOTPAID = 0,  /**< waiting for payment */
    PAYED = 1,    /**< paid */
    REMIND = 2,   /**< remindet */
    ADMONISH = 3, /**< admonish */
    RETURN = 4,   /**< returns */
    COLLPROC = 5  /**< collection procedures */
  };
  Q_ENUM_NS(OrderPayment)

  /**
   * @brief Order status of the service provider
   * @note Used in the order system!
   * @ingroup Enumerations
   */
  enum ProviderPaymentStatus {
    STATUS_NOT_SET = 0,           /**< No status set */
    WAIT_FOR_PAYMENT = 1,         /**< Warte auf Zahlung */
    SHIPMENT_CREATED = 2,         /**< Fertig zum Versand */
    SHIPPED_WAIT_FOR_PAYMENT = 3, /**< Geliefert warte auf Zahlung */
    SHIPPED_AND_PAID = 4,         /**< Bezahlt und versendet */
    BUYER_NO_REACTION = 5,        /**< Keine Reaktion des Käufers */
    ORDER_CANCELED = 6            /**< Käufer hat die Bestellung storniert */
  };
  Q_ENUM_NS(ProviderPaymentStatus)

  /**
   * @brief Payment method of the order
   *
   * Used in the order system to show the payment method.
   * The implementation of this specification is up to the plugin integration!
   *
   * @ingroup Enumerations
   */
  enum PaymentMethod {
    PAYMENT_NOT_SET = 0,              /**< unspecified */
    BANK_PREPAYMENT = 1,              /**< bank transfer (prepayment) */
    DELIVER_WITH_INVOICE = 2,         /**< delivery with open invoice */
    DIRECT_DEBIT_PREPAYMENT = 3,      /**< direct debit payment in advance */
    CREDIT_CARD_PREPAYMENT = 4,       /**< Credit card in advance */
    CASH_ON_DELIVERY = 5,             /**< Cash on delivery payment */
    PAYPAL_PREPAYMENT = 6,            /**< PayPal in advance */
    SKRILL_PREPAYMENT = 7,            /**< Skrill in advance */
    GIROPAY_PREPAYMENT = 8,           /**< GiroPay in advance */
    GOOGLEPAY_PREPAYMENT = 9,         /**< Google Pay in advance */
    UNKNOWN_PREPAYMENT = 10,          /**< Payment in advance from an unknown */
    LOCAL_PICKUP_CASH_PAYMENT = 11,   /**< Pickup - cash payment */
    INSTANT_BANK_TRANSFER = 12,       /**< Instant bank transfer */
    INVOICE_PREPAYMENT_RESERVED = 13, /**< open - Advance payment reserved */
    CHECK_PREPAYMENT = 14,            /**< Check - payment in advance */
  };
  Q_ENUM_NS(PaymentMethod)

  /**
   * @brief Set customer trust level!
   * @ingroup Enumerations
   */
  enum CustomerTrustLevel {
    NO_TRUST_LEVEL = 0, /**< unspecified */
    ON_TIME = 1,        /**< Pays on time */
    RELIABLE = 2,       /**< Pays reliably */
    WITH_DELAY = 3,     /**< Pay late */
    PREPAYMENT = 4,     /**< Only with prepayment */
    NO_DELIVERY = 5     /**< No delivery */
  };
  Q_ENUM_NS(CustomerTrustLevel)

  /**
   * @brief gender definition
   * @ingroup Enumerations
   */
  enum Gender {
    NO_GENDER = 0, /**< unspecified */
    MALE = 1,      /**< male */
    FEMALE = 2,    /**< female */
    VARIOUS = 3    /**< various */
  };
  Q_ENUM_NS(Gender)

  /**
   * @brief Article type finder...
   *
   * By default this "ArticleType" is used to find the right tax categories.
   *
   * The Enumeration is identical with PgSQL Database table enum_article_type.
   * @code
   *  \d+ enum_article_type
   * @endcode
   *
   * @ingroup Enumerations
   */
  enum ArticleType {
    UNKNOWN = 0, /**< unspecified */
    BOOK = 1,    /**< books */
    MEDIA = 2,   /**< media - CD,Vinyl,DVD */
    PRINTS = 3,  /**< prints and stitches */
    OTHER = 4    /**< other or various */
  };
  Q_ENUM_NS(ArticleType)

  /**
   * @brief TAX type
   *
   * Invoicing options for sales tax information (UStG §14)
   *
   * @ingroup Enumerations
   */
  enum SalesTax {
    TAX_NOT = 0,  /**< without sales tax */
    TAX_INCL = 1, /**< including sales tax */
    TAX_WITH = 2  /**< with sales tax  */
  };
  Q_ENUM_NS(SalesTax)

  /**
   * @brief Media type
   *
   * Implementation of media type information
   *
   * @ingroup Enumerations
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
  Q_ENUM_NS(MediaType)

  /**
   * @brief Item condition description
   * @ingroup Enumerations
   */
  enum Condition {
    NO_CONDITION = 0, /**< unspecified */
    FINE = 1,         /**< Very good, almost new! */
    GOOD = 2,         /**< Slight signs of wear */
    SATISFYING = 3,   /**< Significant signs of use */
    SUFFICIENT = 4    /**< Heavily worn */
  };
  Q_ENUM_NS(Condition)

  /**
   * @brief Enumeration to know, how to store configs.
   * @ingroup Enumerations
   */
  enum ConfigType {
    CONFIG_SYSTEM = 0,  /**< store in user config */
    CONFIG_DATABASE = 1 /**< store in database */
  };
  Q_ENUM_NS(ConfigType);

  /**
   * @brief Used by Tabs filter selectors
   *
   * Defines some search categories for tables searches.
   *
   * @ingroup Enumerations
   */
  enum SearchBarFilter {
    SBF_TITLES_AUTHORS = 1,  /**< @brief title and authors */
    SBF_TITLES_KEYWORDS = 2, /**< @brief title and keywords */
    SBF_AUTHORS_ARTISTS = 4, /**< @brief authors and artists search */
    SBF_ISBN_GTIN = 8,       /**< @brief ISBN/GTIN search */
    SBF_ARTICLE_IDS = 16,    /**< @brief article number search */
    SBF_STORAGES = 32,       /**< @brief storage location search */
    SBF_PUBLISHERS = 64,     /**< @brief publishers search */
    SBF_KEYWORDS = 128       /**< @brief keywords search */
  };
  Q_ENUM_NS(SearchBarFilter)

  /**
   * @ingroup CoreLibrary
   * @class PostalCode
   * @brief PostalCode entries
   */
  struct ANTIQUACRM_LIBRARY PostalCode {
    QString plz;
    QString location;
    QString state;
    QString country;
  };

  /**
   * @brief Mailler button groups
   * @ingroup Flags
   */
  enum MailGroup {
    NO_MAIL_GROUP = 0x0,      /**< no button set */
    MAIL_ORDER_GROUP = 0x1,   /**< orders section */
    MAIL_CUSTOMER_GROUP = 0x2 /**< customers section */
  };
  Q_DECLARE_FLAGS(MailGroups, MailGroup);

  /**
   * @brief Printer buttons
   * @ingroup Flags
   */
  enum PrinterGroup {
    PRINT_NOBUTTON = 0x0, /**< no button set */
    PRINT_DELIVERY = 0x1, /**< enable delivery button */
    PRINT_INVOICE = 0x2,  /**< enable invoice button */
    PRINT_REMINDER = 0x4, /**< enable reminder button */
    PRINT_BOOKCARD = 0x8, /**< enable bookcard button */
    PRINT_REFUND = 0x16,   /**< enable refunding button */
    PRINT_ADMONITION = 0x32  /**< enable invoice warning */
  };
  Q_DECLARE_FLAGS(PrinterGroups, PrinterGroup);
}

Q_DECLARE_METATYPE(AntiquaCRM::PostalCode)

Q_DECLARE_OPERATORS_FOR_FLAGS(AntiquaCRM::MailGroups);

Q_DECLARE_OPERATORS_FOR_FLAGS(AntiquaCRM::PrinterGroups);

#endif // ANTIQUACRM_GLOBAL_H
