// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef BOOKLOOKER_PLUGIN_CONFIG_H
#define BOOKLOOKER_PLUGIN_CONFIG_H

#include <QtGlobal>
#include <QString>

/**
 * @ingroup ProviderPlugin
 * @brief library export definition
 */
#ifdef Booklooker_EXPORTS
# define BOOKLOOKER_PLUGIN Q_DECL_EXPORT
#else
# define BOOKLOOKER_PLUGIN Q_DECL_IMPORT
#endif

/**
 * @brief BookLooker Provider plugin Macros
 * @ingroup BookLooker
 * @{
 */
#ifndef BOOKLOOKER_CONFIG_POINTER
#define BOOKLOOKER_CONFIG_POINTER QString("CONFIG_BOOKLOOKER")
#endif

/**
 * @}
 */

#endif // BOOKLOOKER_PLUGIN_CONFIG_H
