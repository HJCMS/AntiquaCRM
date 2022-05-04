// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

/**
 * @defgroup AntiquaCRM (Global definitions)
 * @{
 */

#ifndef ANTIQUA_GLOBAL_H
#define ANTIQUA_GLOBAL_H

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
#define ANTIQUACRM_CONNECTION_DOMAIN "de.hjcms.@PROJECTNAME@"
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
 * @def ANTIQUACORE_EXPORT
 * @brief library export definition
 */
#include <QtCore/QtGlobal>
#if defined(ANTIQUACORE_LIBRARAY)
# define ANTIQUACORE_EXPORT Q_DECL_EXPORT
#else
# define ANTIQUACORE_EXPORT Q_DECL_IMPORT
#endif

#endif // ANTIQUA_GLOBAL_H

/** @} */
