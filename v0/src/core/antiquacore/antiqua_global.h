// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

/**
 * @defgroup AntiquaCRM (Global definitions)
 * @{
 */

#ifndef ANTIQUA_GLOBAL_H
#define ANTIQUA_GLOBAL_H

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
#define ANTIQUACRM_USERAGENT "AntiquaCRM/0.5"
#endif

/**
 * @def ANTIQUACRM_VERSION
 * @brief versioning
 */
#ifndef ANTIQUACRM_VERSION
#define ANTIQUACRM_VERSION "0.5.5"
#endif

/**
 * @def ANTIQUACRM_DISPLAYNAME
 * @brief Window title
 */
#ifndef ANTIQUACRM_DISPLAYNAME
#define ANTIQUACRM_DISPLAYNAME "Antiqua CRM"
#endif

#ifndef ANTIQUACRM_WINDOW_TITLE
#define ANTIQUACRM_WINDOW_TITLE "Antiqua CRM (v0.5.5)"
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
#define ANTIQUACRM_DATA_TARGET "/usr/local/antiquacrm"
#endif

/**
 * @def ANTIQUACRM_PLUGIN_TARGET
 * @brief Plugins target
 */
#ifndef ANTIQUACRM_PLUGIN_TARGET
#define ANTIQUACRM_PLUGIN_TARGET "/usr/local/antiquacrm/plugins/antiqua"
#endif

/**
 * @def ANTIQUACORE_EXPORT
 * @brief library export definition
 */
#if defined(ANTIQUACORE_LIBRARAY)
# define ANTIQUACORE_EXPORT Q_DECL_EXPORT
#else
# define ANTIQUACORE_EXPORT Q_DECL_IMPORT
#endif

/**
 * @def ANTIQUA_DATETIME_FORMAT
 * @ingroup Antiqua Plugin Interface
 * @brief Das vom System vorgegebene Datumsformat ist ISO 8601!
 * Soll verhindern das bei der Umwandlung von einem String, die Konvertierung nicht fehlschlägt!
 * @code
 *  QJsonObject import;
 *  QJsonObject obj = jsonArray[i].toObject();
 *  QDateTime d = QDateTime::fromString(obj["datetime"].toString(), "yyyy-MM-dd hh:mm:ss");
 *  import.insert("datetime", QJsonValue(d.toString(ANTIQUA_DATETIME_FORMAT)));
 * @endcode
 */
#ifndef ANTIQUA_DATETIME_FORMAT
#define ANTIQUA_DATETIME_FORMAT Qt::ISODate
#endif

#ifndef ANTIQUA_QUERY_PASTDAYS
#define ANTIQUA_QUERY_PASTDAYS -4
#endif

namespace Antiqua {
  Q_NAMESPACE

  enum ErrorStatus {
    NOTICE = 1,
    WARNING = 2,
    FATAL = 3
  };
  Q_ENUM_NS(Antiqua::ErrorStatus);

  enum PaymentStatus {
    ORDER_STATUS_NOT_SET = 0,
    ORDER_WAIT_FOR_PAYMENT = 1,
    ORDER_READY_FOR_SHIPMENT = 2,
    ORDER_SHIPPED_WAIT_FOR_PAYMENT = 3,
    ORDER_SHIPPED_AND_PAID = 4,
    ORDER_BUYER_NO_REACTION = 5
  };
  Q_ENUM_NS(Antiqua::PaymentStatus);

};

#endif // ANTIQUA_GLOBAL_H

/** @} */
