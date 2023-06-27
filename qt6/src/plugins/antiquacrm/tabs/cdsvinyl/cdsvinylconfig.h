// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CDSVINYL_CONFIG_H
#define ANTIQUACRM_PLUGIN_CDSVINYL_CONFIG_H

#include <QString>

/**
 * @brief CDs and Vinyl tab plugin Macros
 * @ingroup TabsPlugin
 * @{
 */
#ifndef CDSVINYL_INTERFACE_NAME
#define CDSVINYL_INTERFACE_NAME QString("cdsvinyl")
#endif

#ifndef CDSVINYL_CONFIG_POINTER
#define CDSVINYL_CONFIG_POINTER QString("CONFIG_PRINTS_CDSVINYL")
#endif

#ifndef CDSVINYL_TABLE_NAME
#define CDSVINYL_TABLE_NAME QString("inventory_cdvinyl")
#endif

#ifndef CDSVINYL_INDEX_COLUMN
#define CDSVINYL_INDEX_COLUMN QString("cv_id")
#endif

#ifndef CDSVINYL_SQL_EDITOR_PATTERN
#define CDSVINYL_SQL_EDITOR_PATTERN "^cv_[a-z_]+\\b$"
#endif

/**
 * @}
 */

#endif // ANTIQUACRM_PLUGIN_CDSVINYL_CONFIG_H
