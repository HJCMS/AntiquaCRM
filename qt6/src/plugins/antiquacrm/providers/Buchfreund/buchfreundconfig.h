// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef BUCHFREUND_PLUGIN_CONFIG_H
#define BUCHFREUND_PLUGIN_CONFIG_H

#include <QtGlobal>
#include <QString>

/**
 * @brief Buchfreund Provider plugin Macros
 */
#ifdef Buchfreund_EXPORTS
# define BUCHFREUND_PLUGIN Q_DECL_EXPORT
#else
# define BUCHFREUND_PLUGIN Q_DECL_IMPORT
#endif

#ifndef BUCHFREUND_CONFIG_POINTER
#define BUCHFREUND_CONFIG_POINTER QString("CONFIG_BUCHFREUND")
#endif

#endif // BUCHFREUND_PLUGIN_CONFIG_H
