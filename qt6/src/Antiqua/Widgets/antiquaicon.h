// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_ANTIQUAICON_H
#define ANTIQUACRM_WIDGETS_ANTIQUAICON_H

#include <AGlobal>
#include <QIcon>
#include <QResource>
#include <QString>

namespace AntiquaCRM {

/**
 * @brief Icon from Theme or Application Resource file ...
 * @param name  - Iconname
 * @warning The QResource must initialed in application first!
 *
 * Load qrc://icons/&lt;image&gt; from icon QResource.
 *
 * @ingroup IconTheme
 */
ANTIQUACRM_LIBRARY inline const QIcon qrcIcon(const QString &name) {
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
ANTIQUACRM_LIBRARY inline const QIcon antiquaIcon(const QString &name) {
  /*
    if (!name.contains("antiqua") && !QIcon::hasThemeIcon(name)) {
      qWarning("No Theme Icon:'%s'", qPrintable(name));
    }
  */
  // Load from Unix Icon theme
  return QIcon::fromTheme(name, qrcIcon(name));
}

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_ANTIQUAICON_H
