// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

/**
 * @defgroup AntiquaCRM (Global definitions)
 * @{
 */

#ifndef ANTIQUACRM_GLOBAL_H
#define ANTIQUACRM_GLOBAL_H

#include <QtCore/QtGlobal>
#include <QObject>

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
 * @def ANTIQUACRM_EXPORT
 * @brief library export definition
 */
#ifdef ANTIQUACRM_SHARED_LIBRARAY
# define ANTIQUACRM_LIBRARAY Q_DECL_EXPORT
#else
# define ANTIQUACRM_LIBRARAY Q_DECL_IMPORT
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
 *  import.insert("datetime", QJsonValue(d.toString(ANTIQUACRM_DATETIME_FORMAT)));
 * @endcode
 */
#ifndef ANTIQUACRM_DATETIME_FORMAT
#define ANTIQUACRM_DATETIME_FORMAT Qt::ISODate
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

  enum Message {
   NORMAL = 0x0,
   WARNING = 0x1,
   FATAL = 0x2,
   LOGGING = 0x3
  };
  Q_ENUM_NS(AntiquaCRM::Message);

  enum OrderStatus {
   OPEN = 0x0,
   STARTED = 0x1,
   FETCHET = 0x2,
   DELIVERED = 0x3,
   REMINDET = 0x4,
   COMPLETED = 0x5,
   CANCELED = 0x6,
   RETURNING = 0x7
  };
  Q_ENUM_NS(AntiquaCRM::OrderStatus);

  enum PaymentStatus {
   STATUS_NOT_SET = 0x0,
   WAIT_FOR_PAYMENT = 0x1,
   READY_FOR_SHIPMENT = 0x2,
   SHIPPED_WAIT_FOR_PAYMENT = 0x3,
   SHIPPED_AND_PAID = 0x4,
   BUYER_NO_REACTION = 0x5
  };
  Q_ENUM_NS(AntiquaCRM::PaymentStatus);

};

#endif // ANTIQUACRM_GLOBAL_H

/** @} */
