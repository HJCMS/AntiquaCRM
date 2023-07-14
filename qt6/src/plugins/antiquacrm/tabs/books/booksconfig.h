// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_BOOKSCONFIG_H
#define ANTIQUACRM_PLUGIN_BOOKSCONFIG_H

#include <QString>

/**
 * @brief Books tab plugin Macros
 * @ingroup TabsPlugin
 * @{
 */
#ifndef BOOKS_INTERFACE_NAME
#define BOOKS_INTERFACE_NAME "books"
#endif

#ifndef BOOKS_INTERFACE_TABID
#define BOOKS_INTERFACE_TABID "books_tab"
#endif

#ifndef BOOKS_CONFIG_POINTER
#define BOOKS_CONFIG_POINTER QString("CONFIG_BOOK_TAB")
#endif

#ifndef BOOKS_SQL_TABLE_NAME
#define BOOKS_SQL_TABLE_NAME "inventory_books"
#endif

#ifndef BOOKS_SQL_FIELD_PREFIX
#define BOOKS_SQL_FIELD_PREFIX "ib"
#endif

#ifndef BOOKS_SQL_EDITOR_PATTERN
#define BOOKS_SQL_EDITOR_PATTERN "^ib_[a-z_]+\\b$"
#endif

#ifndef BOOKS_ALWAYS_ADD_ONLOAD
#define BOOKS_ALWAYS_ADD_ONLOAD true
#endif

/**
 * @}
 */

#endif
