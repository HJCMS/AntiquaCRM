// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_ANTIQUAICON_H
#define ANTIQUACRM_WIDGETS_ANTIQUAICON_H

#include <AGlobal>
#include <QIcon>
#include <QResource>
#include <QString>
#ifdef ANTIQUA_DEVELOPMENT
#  include <QDebug>
#endif

namespace AntiquaCRM
{

/**
 * @brief Icon from Theme or Application Resource file ...
 * @param name  - Iconname
 * @warning The QResource must initialed in application first!
 *
 * Load qrc://icons/&lt;image&gt; from icon QResource.
 *
 * @ingroup IconTheme
 */
inline const QIcon qrcIcon(const QString& name) {
  return QIcon(":/icons/" + name + ".png");
}

/**
 * @brief Icon from Theme or Application Resource file ...
 * @param name  - Iconname
 *
 * @note The QResource must initialed in application first!
 *
 * @ingroup IconTheme
 */
inline const QIcon antiquaIcon(const QString& name) {
  if (name.length() < 2)
    return qrcIcon("unknown");

  return QIcon::fromTheme(name, qrcIcon(name));
}

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_ANTIQUAICON_H
