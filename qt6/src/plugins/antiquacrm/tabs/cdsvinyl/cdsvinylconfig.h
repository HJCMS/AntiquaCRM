// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CDSVINYL_CONFIG_H
#define ANTIQUACRM_PLUGIN_CDSVINYL_CONFIG_H

#include <QtGlobal>
#include <QString>

/**
 * @ingroup ProviderPlugin
 * @brief library export definition
 */
#ifdef CDsVinyl_EXPORTS
# define ANTIQUACRM_CDSVINYL_PLUGIN Q_DECL_EXPORT
#else
# define ANTIQUACRM_CDSVINYL_PLUGIN Q_DECL_IMPORT
#endif

/**
 * @brief CDs and Vinyl tab plugin Macros
 * @ingroup TabsPlugin
 * @{
 */
#ifndef CDSVINYL_INTERFACE_NAME
#define CDSVINYL_INTERFACE_NAME QString("cdsvinyl")
#endif

#ifndef CDSVINYL_INVENTORY_TABID
#define CDSVINYL_INVENTORY_TABID "cdsvinyl_tab"
#endif

#ifndef CDSVINYL_CONFIG_POINTER
#define CDSVINYL_CONFIG_POINTER QString("CONFIG_CDVINYL_TAB")
#endif

#ifndef CDSVINYL_TABLE_NAME
#define CDSVINYL_TABLE_NAME QString("inventory_cdvinyl")
#endif

#ifndef CDSVINYL_INDEX_COLUMN
#define CDSVINYL_INDEX_COLUMN QString("cv_id")
#endif

#ifndef CDSVINYL_SQL_EDITOR_PATTERN
#define CDSVINYL_SQL_EDITOR_PATTERN "^cv_[a-z_]+[0-9]?\\b$"
#endif

/**
 * @}
 */

#endif // ANTIQUACRM_PLUGIN_CDSVINYL_CONFIG_H
