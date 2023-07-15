// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDERSCONFIG_H
#define ANTIQUACRM_PLUGIN_ORDERSCONFIG_H

#include <QString>

/**
 * @brief Orders tab plugin Macros
 * @ingroup TabsPlugin
 * @{
 */
#ifndef ORDERS_INTERFACE_NAME
#define ORDERS_INTERFACE_NAME "orders"
#endif

#ifndef ORDERS_INTERFACE_TABID
#define ORDERS_INTERFACE_TABID "orders_tab"
#endif

#ifndef ORDERS_CONFIG_POINTER
#define ORDERS_CONFIG_POINTER QString("CONFIG_ORDERS_TAB")
#endif

#ifndef ORDERS_SQL_TABLE_NAME
#define ORDERS_SQL_TABLE_NAME "inventory_orders"
#endif

#ifndef ORDERS_SQL_FIELD_PREFIX
#define ORDERS_SQL_FIELD_PREFIX "o"
#endif

#ifndef ORDERS_SQL_EDITOR_PATTERN
#define ORDERS_SQL_EDITOR_PATTERN "^o_[a-z_]+\\b$"
#endif

/**
 * @}
 */

#endif
