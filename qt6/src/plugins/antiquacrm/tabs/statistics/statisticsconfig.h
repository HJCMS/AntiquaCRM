// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#pragma once

#include <QtCore/QtGlobal>

/**
 * @ingroup ProviderPlugin
 * @brief library export definition
 */
#ifdef Statistics_EXPORTS
#  define ANTIQUACRM_STATISTICS_PLUGIN Q_DECL_EXPORT
#else
#  define ANTIQUACRM_STATISTICS_PLUGIN Q_DECL_IMPORT
#endif
