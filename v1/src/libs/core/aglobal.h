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
 * @brief OrganizationDomain is required by QSettings
 * @note All Configurations stored in:
 *  @li Linux/User:  ~/.config/de.hjcms/antiquacrm/
 *  @li Windows: C:/Users/<USER>/AppData/Local/de.hjcms/antiquacrm/
 */
#ifndef HJCMS_CONFIG_DOMAIN
#define HJCMS_CONFIG_DOMAIN "de.hjcms"
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
 * @brief required by QNetworkAccessManager and cUrl
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
#define ANTIQUACRM_DATA_TARGET ""
#endif

/**
 * @def ANTIQUACRM_PLUGIN_TARGET
 * @brief Plugins target
 */
#ifndef ANTIQUACRM_PLUGIN_TARGET
#define ANTIQUACRM_PLUGIN_TARGET ""
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

#ifndef ANTIQUACRM_QUERY_PASTDAYS
#define ANTIQUACRM_QUERY_PASTDAYS -4
#endif

namespace AntiquaCRM {
  Q_NAMESPACE

  enum Notify {
    NOTICE = 1,
    WARNING = 2,
    FATAL = 3
  };
  Q_ENUM_NS(AntiquaCRM::Notify);

  enum PaymentStatus {
    ORDER_STATUS_NOT_SET = 0,
    ORDER_WAIT_FOR_PAYMENT = 1,
    ORDER_READY_FOR_SHIPMENT = 2,
    ORDER_SHIPPED_WAIT_FOR_PAYMENT = 3,
    ORDER_SHIPPED_AND_PAID = 4,
    ORDER_BUYER_NO_REACTION = 5
  };
  Q_ENUM_NS(AntiquaCRM::PaymentStatus);

};

#endif // ANTIQUACRM_GLOBAL_H

/** @} */
