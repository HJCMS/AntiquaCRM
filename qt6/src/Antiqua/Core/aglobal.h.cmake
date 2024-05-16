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

#include <QtGlobal>
#include <QDate>
#include <QString>
#include <QIODevice>

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
#ifdef ANTIQUACRM_SHARED_LIBRARY
# define ANTIQUACRM_LIBRARY Q_DECL_EXPORT
#else
# define ANTIQUACRM_LIBRARY Q_DECL_IMPORT
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
}
Q_DECLARE_METATYPE(AntiquaCRM::PostalCode)

/**
 * @short Including predefined enumerations
 */
#ifndef ANTIQUACRM_AENUMERATIONS_H
#include "aenumerations.h"
#endif

/**
 * @short Including predefined flags
 */
#ifndef ANTIQUACRM_AFLAGS_H
#include "aflags.h"
#endif

#endif // ANTIQUACRM_GLOBAL_H
