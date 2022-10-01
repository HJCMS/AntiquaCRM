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
#define ANTIQUACRM_NAME "antiquacrm"
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
#define HJCMS_CONFIG_NAME "antiquacrm"
#endif

#ifndef HJCMS_CONFIG_VERSION
#define HJCMS_CONFIG_VERSION "1.0"
#endif

/**
 * @def ANTIQUACRM_CONNECTION_DOMAIN
 * @brief required by PostgreSQL, D-Bus and TCP-Socket
 */
#ifndef ANTIQUACRM_CONNECTION_DOMAIN
#define ANTIQUACRM_CONNECTION_DOMAIN "de.hjcms.antiquacrm"
#endif

/**
 * @def ANTIQUACRM_USERAGENT
 * @brief required by NetworkAccessManager
 * @warning This fixed string is needet for OAuth
 */
#ifndef ANTIQUACRM_USERAGENT
#define ANTIQUACRM_USERAGENT "AntiquaCRM/1.0"
#endif

/**
 * @def ANTIQUACRM_VERSION
 * @brief versioning
 */
#ifndef ANTIQUACRM_VERSION
#define ANTIQUACRM_VERSION "1.0.0"
#endif

/**
 * @def ANTIQUACRM_DISPLAYNAME
 * @brief Window title
 */
#ifndef ANTIQUACRM_DISPLAYNAME
#define ANTIQUACRM_DISPLAYNAME "Antiqua CRM"
#endif

#ifndef ANTIQUACRM_WINDOW_TITLE
#define ANTIQUACRM_WINDOW_TITLE "Antiqua CRM (v1.0.0)"
#endif

/**
 * @def ANTIQUACRM_HOMEPAGE
 * @brief Support link
 */
#ifndef ANTIQUACRM_HOMEPAGE
#define ANTIQUACRM_HOMEPAGE "https://www.hjcms.de"
#endif

/**
 * @def ANTIQUACRM_DATA_TARGET
 * @brief Shared Data location
 */
#ifndef ANTIQUACRM_DATA_TARGET
#define ANTIQUACRM_DATA_TARGET "/home/heinemann/Developement/antiqua/v1/src/data"
#endif

/**
 * @def ANTIQUACRM_PLUGIN_TARGET
 * @brief Plugins target
 */
#ifndef ANTIQUACRM_PLUGIN_TARGET
#define ANTIQUACRM_PLUGIN_TARGET "/home/heinemann/Developement/antiqua/build-v1-Desktop_Qt_5_15_2_GCC_64bit-Debug/src/plugins"
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
   NOTICE = 1,
   WARNING = 2,
   FATAL = 3
  };
  Q_ENUM_NS(AntiquaCRM::Message);

  enum OrderStatus {
   ORDER_OPEN = 0x0,
   ORDER_STARTED = 0x1,
   ORDER_FETCHET = 0x2,
   ORDER_DELIVERED = 0x3,
   ORDER_REMINDET = 0x4,
   ORDER_COMPLETED = 0x5,
   ORDER_CANCELED = 0x6,
   ORDER_RETURNING = 0x7
  };
  Q_ENUM_NS(AntiquaCRM::OrderStatus);

  enum PaymentStatus {
   ORDER_STATUS_NOT_SET = 0x0,
   ORDER_WAIT_FOR_PAYMENT = 0x1,
   ORDER_READY_FOR_SHIPMENT = 0x2,
   ORDER_SHIPPED_WAIT_FOR_PAYMENT = 0x3,
   ORDER_SHIPPED_AND_PAID = 0x4,
   ORDER_BUYER_NO_REACTION = 0x5
  };
  Q_ENUM_NS(AntiquaCRM::PaymentStatus);

};

#endif // ANTIQUACRM_GLOBAL_H

/** @} */
