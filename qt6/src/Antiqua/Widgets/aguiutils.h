// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_INPUT_UTILS_H
#define ANTIQUACRM_INPUT_UTILS_H

#include <AGlobal>
#include <QIcon>
#include <QFileInfo>
#include <QResource>

/**
 * @brief Initial AntiquaCRM Icons Resources
 * @ingroup EditWidgets
 */
ANTIQUACRM_LIBRARY inline void initAntiquaIconsResource() {
#ifdef ANTIQUACRM_RESOURCE_PATH
  QFileInfo _qrc(ANTIQUACRM_RESOURCE_PATH);
  if (_qrc.isReadable()) {
    QResource::registerResource(_qrc.filePath());
  } else {
    qWarning("Invalid Icons Resource file path!");
  }
#else
  qWarning("Missing Icons Resources!");
#endif
}

namespace AntiquaCRM {

/**
 * @class AGuiTools
 * @brief A set of Utils using QtGui and QtWidgets
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY AGuiUtils final {
public:
  /**
   * @brief Internal Antiqua Widget Theme Icons
   * Use QIcon::fromTheme and fallback to internal QRessource.
   * @param name
   */
  static const QIcon getIcon(const QString &name);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_INPUT_UTILS_H
