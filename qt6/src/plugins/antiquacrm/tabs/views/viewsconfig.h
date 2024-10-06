// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_VIEWS_CONFIG_H
#define ANTIQUACRM_PLUGIN_VIEWS_CONFIG_H

#include <QString>
#include <QtGlobal>

/**
 * @ingroup ProviderPlugin
 * @brief library export definition
 */
#ifdef Views_EXPORTS
#  define ANTIQUACRM_VIEWS_PLUGIN Q_DECL_EXPORT
#else
#  define ANTIQUACRM_VIEWS_PLUGIN Q_DECL_IMPORT
#endif

#endif // ANTIQUACRM_PLUGIN_VIEWS_CONFIG_H
