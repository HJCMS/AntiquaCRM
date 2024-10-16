// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_STITCHESCONFIG_H
#define ANTIQUACRM_PLUGIN_STITCHESCONFIG_H

#include <QString>
#include <QtGlobal>

/**
 * @ingroup ProviderPlugin
 * @brief library export definition
 */
#ifdef Stitches_EXPORTS
#  define ANTIQUACRM_STITCHES_PLUGIN Q_DECL_EXPORT
#else
#  define ANTIQUACRM_STITCHES_PLUGIN Q_DECL_IMPORT
#endif

/**
 * @brief Prints & Stitches tab plugin Macros
 * @ingroup TabsPlugin
 * @{
 */
#ifndef STITCHES_INTERFACE_NAME
#  define STITCHES_INTERFACE_NAME QString("stitches")
#endif

#ifndef STITCHES_CONFIG_POINTER
#  define STITCHES_CONFIG_POINTER QString("CONFIG_PRINTS_TAB")
#endif

#ifndef STITCHES_TABLE_NAME
#  define STITCHES_TABLE_NAME QString("inventory_prints")
#endif

#ifndef STITCHES_INDEX_COLUMN
#  define STITCHES_INDEX_COLUMN QString("ip_id")
#endif

#ifndef STITCHES_SQL_EDITOR_PATTERN
#  define STITCHES_SQL_EDITOR_PATTERN "^ip_[a-z_]+[0-9]?\\b$"
#endif

/**
 * @}
 */

#endif
