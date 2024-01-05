// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_VARIOUS_CONFIG_H
#define ANTIQUACRM_PLUGIN_VARIOUS_CONFIG_H

#include <QString>

#ifndef VARIOUS_INTERFACE_NAME
#define VARIOUS_INTERFACE_NAME "various"
#endif

#ifndef VARIOUS_INTERFACE_TABID
#define VARIOUS_INTERFACE_TABID "various_tab"
#endif

#ifndef VARIOUS_CONFIG_POINTER
#define VARIOUS_CONFIG_POINTER QString("CONFIG_VARIOUS_TAB")
#endif

#ifndef VARIOUS_SQL_TABLE_NAME
#define VARIOUS_SQL_TABLE_NAME "inventory_various"
#endif

#ifndef VARIOUS_SQL_FIELD_PREFIX
#define VARIOUS_SQL_FIELD_PREFIX "va"
#endif

#ifndef VARIOUS_SQL_EDITOR_PATTERN
#define VARIOUS_SQL_EDITOR_PATTERN "^va_[a-z_]+[0-9]?\\b$"
#endif

#ifndef VARIOUS_ALWAYS_ADD_ONLOAD
#define VARIOUS_ALWAYS_ADD_ONLOAD false
#endif

#endif // ANTIQUACRM_PLUGIN_VARIOUS_CONFIG_H
