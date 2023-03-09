// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

/**
 * @defgroup amacros AntiquaCRM Macros
 * @brief AntiquaCRM core macros
 */

#ifndef ANTIQUACRM_GLOBAL_H
#define ANTIQUACRM_GLOBAL_H

#include <QtGlobal>
#include <QString>
#include <QIODevice>

/**
 * @ingroup amacros
 * @brief default executable name
 * @short Required by qApp-&gt;setApplicationName() and QSettings class.
 */
#ifndef ANTIQUACRM_NAME
#define ANTIQUACRM_NAME "@PROJECTNAME@"
#endif

/**
 * @ingroup amacros
 * @brief Application Domain in user configuration path
 * @code
 *  ~/.config/{HJCMS_CONFIG_DOMAIN}/{HJCMS_CONFIG_NAME}/{HJCMS_CONFIG_VERSION}
 * @endcode
 */
#ifndef HJCMS_CONFIG_DOMAIN
#define HJCMS_CONFIG_DOMAIN "de.hjcms"
#endif

/**
 * @ingroup amacros
 * @brief Application name in user configuration path
 */
#ifndef HJCMS_CONFIG_NAME
#define HJCMS_CONFIG_NAME "@PROJECTNAME@"
#endif

/**
 * @ingroup amacros
 * @brief Application Version in user configuration path
 * @short config version for targeting the location 
 *  of the current user configuration file.
 */
#ifndef HJCMS_CONFIG_VERSION
#define HJCMS_CONFIG_VERSION "@ANTIQUACRM_VERSION_MAJOR@.0"
#endif

/**
 * @ingroup amacros
 * @brief Configuration Application ID
 */
#ifndef HJCMS_CONFIG_APPID
#define HJCMS_CONFIG_APPID "@ANTIQUACRM_APPID@"
#endif

/**
 * @ingroup amacros
 * @brief Configuration DNS ID
 */
#ifndef HJCMS_CONFIG_DNSID
#define HJCMS_CONFIG_DNSID "@ANTIQUACRM_DNSID@"
#endif

/**
 * @ingroup amacros
 * @brief Configuration URL ID
 */
#ifndef HJCMS_CONFIG_URLID
#define HJCMS_CONFIG_URLID "@ANTIQUACRM_URLID@"
#endif

/**
 * @ingroup amacros
 * @brief required by PostgreSQL, D-Bus and TCP-Socket
 */
#ifndef ANTIQUACRM_CONNECTION_DOMAIN
#define ANTIQUACRM_CONNECTION_DOMAIN "de.hjcms.@PROJECTNAME@"
#endif

/**
 * @ingroup amacros
 * @brief required by NetworkAccessManager
 * @warning This fixed string is needet for OAuth
 */
#ifndef ANTIQUACRM_USERAGENT
#define ANTIQUACRM_USERAGENT "AntiquaCRM/@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@"
#endif

/**
 * @ingroup amacros
 * @brief Application Version String
 */
#ifndef ANTIQUACRM_VERSION
#define ANTIQUACRM_VERSION "@ANTIQUACRM_VERSION_MAJOR@.@ANTIQUACRM_VERSION_MINOR@.@ANTIQUACRM_VERSION_RELEASE@"
#endif

/**
 * @ingroup amacros
 * @brief Application Display title
 */
#ifndef ANTIQUACRM_DISPLAYNAME
#define ANTIQUACRM_DISPLAYNAME "@DISPLAYNAME@"
#endif

/**
 * @ingroup amacros
 * @brief Application Window title inkl. Version string
 */
#ifndef ANTIQUACRM_WINDOW_TITLE
#define ANTIQUACRM_WINDOW_TITLE "@DISPLAYNAME@ (v@ANTIQUACRM_VERSION_STRING@)"
#endif

/**
 * @ingroup amacros
 * @brief Project Homepage url
 */
#ifndef ANTIQUACRM_HOMEPAGE
#define ANTIQUACRM_HOMEPAGE "@HOMEPAGEURL@"
#endif

/**
 * @ingroup amacros
 * @brief AntiquaCRM Provider Plugin-Interface „namespace“
 */
#ifndef ANTIQUACRM_INTERFACE
#define ANTIQUACRM_INTERFACE "de.hjcms.antiquacrm.ProviderInterface"
#endif

/**
 * @ingroup amacros
 * @brief Shared Data location
 */
#ifndef ANTIQUACRM_DATA_TARGET
#define ANTIQUACRM_DATA_TARGET "@ANTIQUACRM_DATA_TARGET@"
#endif

/**
 * @ingroup amacros
 * @brief Plugins target
 */
#ifndef ANTIQUACRM_PLUGIN_TARGET
#define ANTIQUACRM_PLUGIN_TARGET "@ANTIQUACRM_PLUGIN_TARGET@"
#endif

/**
 * @ingroup amacros
 * @brief Translations target
 */
#ifndef ANTIQUACRM_TRANSLATION_TARGET
#define ANTIQUACRM_TRANSLATION_TARGET "@ANTIQUACRM_TRANSLATION_TARGET@"
#endif

/**
 * @ingroup amacros
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
 * @ingroup amacros
 * @brief Standard mode for socket connections
 */
#ifndef ANTIQUACRM_TRANSMITTER_MODE
#define ANTIQUACRM_TRANSMITTER_MODE QIODevice::WriteOnly
#endif

/**
 * @ingroup amacros
 * @brief library export definition
 */
#ifdef ANTIQUACRM_SHARED_LIBRARY
# define ANTIQUACRM_LIBRARY Q_DECL_EXPORT
#else
# define ANTIQUACRM_LIBRARY Q_DECL_IMPORT
#endif

/**
 * @ingroup amacros
 * @brief Import Format for PGSQL::CURRENT_TIMESTAMP
 * @note AntiquaCRM not using Time Zones in SQL Tables!
 */
#ifndef ANTIQUACRM_TIMESTAMP_IMPORT
#define ANTIQUACRM_TIMESTAMP_IMPORT "dd.MM.yyyy hh:mm:ss.zzz"
#endif

/**
 * @ingroup amacros
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
 * @ingroup amacros
 * @brief Date and time information for the UI display.
 */
#ifndef ANTIQUACRM_DATETIME_TOOLTIP
#define ANTIQUACRM_DATETIME_TOOLTIP "dd.MM.yyyy hh:mm:ss"
#endif

/**
 * @ingroup amacros
 * @brief Long date time display format
 */
#ifndef ANTIQUACRM_DATETIME_DISPLAY
#define ANTIQUACRM_DATETIME_DISPLAY "dddd dd. MMMM yyyy hh:mm"
#endif

/**
 * @ingroup amacros
 * @brief Normal date time display format
 */
#ifndef ANTIQUACRM_DATE_DISPLAY
#define ANTIQUACRM_DATE_DISPLAY "dddd dd. MMMM yyyy"
#endif

/**
 * @ingroup amacros
 * @brief Short date time display format
 */
#ifndef ANTIQUACRM_SHORT_DATE_DISPLAY
#define ANTIQUACRM_SHORT_DATE_DISPLAY "dd MMM yyyy"
#endif

/**
 * @ingroup amacros
 * @brief Default Charset for Database Connections, Application files, is utf8!
 */
#ifndef ANTIQUACRM_TEXTCODEC
#define ANTIQUACRM_TEXTCODEC "UTF8"
#endif

#ifdef ANTIQUACRM_DBUS_ENABLED
#include <QDBusAbstractAdaptor>
#else
/**
 * @ingroup amacros
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
}
Q_DECLARE_METATYPE(AntiquaCRM::PostalCode)

/**
 * @short Including predefined enumerations
 */
#ifndef ANTIQUACRM_AENUMERATIONS_H
#include "aenumerations.h"
#endif

#endif // ANTIQUACRM_GLOBAL_H
