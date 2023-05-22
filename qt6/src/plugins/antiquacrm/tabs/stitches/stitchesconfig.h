// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_STITCHESCONFIG_H
#define ANTIQUACRM_PLUGIN_STITCHESCONFIG_H

#include <QString>

/**
 * @brief Prints & Stitches tab plugin Macros
 * @ingroup TabsPlugin
 * @{
 */
#ifndef STITCHES_INTERFACE_NAME
#define STITCHES_INTERFACE_NAME "stitches"
#endif

#ifndef STITCHES_CONFIG_POINTER
#define STITCHES_CONFIG_POINTER QString("CONFIG_PRINTS_STITCHES")
#endif

#ifndef STITCHES_SQL_TABLE_NAME
#define STITCHES_SQL_TABLE_NAME "inventory_prints"
#endif

#ifndef STITCHES_SQL_FIELD_PREFIX
#define STITCHES_SQL_FIELD_PREFIX "ip"
#endif

#ifndef STITCHES_SQL_EDITOR_PATTERN
#define STITCHES_SQL_EDITOR_PATTERN "^ip_[a-z_]+\\b$"
#endif

#ifndef STITCHES_ALWAYS_ADD_ONLOAD
#define STITCHES_ALWAYS_ADD_ONLOAD true
#endif

/**
 * @}
 */

#endif
