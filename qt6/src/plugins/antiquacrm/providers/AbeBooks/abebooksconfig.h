// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ABEBOOKS_PLUGIN_ABEBOOKS_CONFIG_H
#define ABEBOOKS_PLUGIN_ABEBOOKS_CONFIG_H

#include <QtGlobal>
#include <QString>

/**
 * @ingroup ProviderPlugin
 * @brief library export definition
 */
#ifdef AbeBooks_EXPORTS
# define ABEBOOKS_PLUGIN Q_DECL_EXPORT
#else
# define ABEBOOKS_PLUGIN Q_DECL_IMPORT
#endif

/**
 * @brief AbeBooks Provider plugin Macros
 * @ingroup ProviderPlugin
 * @{
 */

#ifndef ABEBOOKS_CONFIG_POINTER
#define ABEBOOKS_CONFIG_POINTER QString("CONFIG_ABEBOOKS")
#endif

/**
 * @}
 */

#endif
