// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CUSTOMERSCONFIG_H
#define ANTIQUACRM_PLUGIN_CUSTOMERSCONFIG_H

#include <QString>

/**
 * @brief CUSTOMERS tab plugin Macros
 * @ingroup TabsPlugin
 * @{
 */
#ifndef CUSTOMERS_INTERFACE_NAME
#define CUSTOMERS_INTERFACE_NAME "customers"
#endif

#ifndef CUSTOMERS_INTERFACE_TABID
#define CUSTOMERS_INTERFACE_TABID "customers_tab"
#endif

#ifndef CUSTOMERS_CONFIG_POINTER
#define CUSTOMERS_CONFIG_POINTER QString("CONFIG_CUSTOMERS_TAB")
#endif

#ifndef CUSTOMERS_SQL_TABLE_NAME
#define CUSTOMERS_SQL_TABLE_NAME "customers"
#endif

#ifndef CUSTOMERS_SQL_FIELD_PREFIX
#define CUSTOMERS_SQL_FIELD_PREFIX "c"
#endif

#ifndef CUSTOMERS_SQL_EDITOR_PATTERN
#define CUSTOMERS_SQL_EDITOR_PATTERN "^c_[a-z_]+([0-9]+)?\\b$"
#endif

#ifndef CUSTOMERS_ALWAYS_ADD_ONLOAD
#define CUSTOMERS_ALWAYS_ADD_ONLOAD true
#endif

/**
 * @}
 */

#endif
