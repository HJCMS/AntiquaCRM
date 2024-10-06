// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_REPORTS_CONFIG_H
#define ANTIQUACRM_REPORTS_CONFIG_H

#include <QString>
#include <QtGlobal>

/**
 * @ingroup ProviderPlugin
 * @brief library export definition
 */
#ifdef Reports_EXPORTS
#  define ANTIQUACRM_REPORTS_PLUGIN Q_DECL_EXPORT
#else
#  define ANTIQUACRM_REPORTS_PLUGIN Q_DECL_IMPORT
#endif

#endif // ANTIQUACRM_REPORTS_CONFIG_H
