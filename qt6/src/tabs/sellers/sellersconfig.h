// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_SELLERSCONFIG_H
#define ANTIQUACRM_PLUGIN_SELLERSCONFIG_H

#include <QString>

/**
 * @brief Sellers tab plugin Macros
 * @ingroup TabsPlugin
 * @{
 */
#ifndef SELLERS_INTERFACE_NAME
#define SELLERS_INTERFACE_NAME "sellers"
#endif

#ifndef SELLERS_INTERFACE_TABID
#define SELLERS_INTERFACE_TABID "sellers_tab"
#endif

#ifndef SELLERS_CONFIG_POINTER
#define SELLERS_CONFIG_POINTER QString("CONFIG_SELLERS_TAB")
#endif

#ifndef SELLERS_SQL_TABLE_NAME
#define SELLERS_SQL_TABLE_NAME QString("provider_orders")
#endif

#ifndef SELLERS_SQL_FIELD_PREFIX
#define SELLERS_SQL_FIELD_PREFIX "pr"
#endif

/**
 * @}
 */

#endif

