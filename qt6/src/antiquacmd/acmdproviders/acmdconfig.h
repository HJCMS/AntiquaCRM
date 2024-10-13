// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACMD_ANTIQUACMD_CONFIG_H
#define ANTIQUACMD_ANTIQUACMD_CONFIG_H

/**
 * @ingroup AMacros
 * @brief library export definition
 */
#ifdef ACMD_PLUGIN_EXPORT
# define ACMDPLUGIN_LIBRARY Q_DECL_EXPORT
#else
# define ACMDPLUGIN_LIBRARY Q_DECL_IMPORT
#endif

#endif // ANTIQUACMD_ANTIQUACMD_CONFIG_H
