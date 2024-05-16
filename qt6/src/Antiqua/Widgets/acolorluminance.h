// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_INPUT_COLORLUMINANCE_H
#define ANTIQUACRM_INPUT_COLORLUMINANCE_H

#include <AGlobal>
#include <QApplication>
#include <QColor>
#include <QObject>
#include <QPalette>
#include <QString>

namespace AntiquaCRM {

/**
 * @class AColorLuminance
 * @brief Get Relative Luminance for Application Stylesheets.
 * This is a helper class for correcting some color stylesheet rendering.
 *
 * It will intalialed at application start from QApplication to fix some styles.
 * See also the Color Success Criterias from WCAG21:
 * https://www.w3.org/WAI/WCAG21/Understanding/contrast-minimum.html#dfn-contrast-ratio
 *
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY AColorLuminance final : public QObject {
  Q_OBJECT

private:
  /**
   * @brief QPalette from QApplication
   */
  const QPalette p_palette;

public:
  /**
   * @param parent - This parent Widget is required for QPalette
   */
  explicit AColorLuminance(QApplication *parent = nullptr);

  /**
   * @brief calculate luminance from color
   * @param color - diff color
   */
  static double luminance(const QColor &color);

  /**
   * @brief check luminance contrast
   * The Contrast ratios can range from 1 to 21.
   * If contrast ratio smaller than 12 it returns false!
   *
   * @param fg - Foreground Color
   * @param bg - Background Color
   */
  bool check(const QColor &fg, const QColor &bg) const;

  /**
   * @brief Check Foreground to Background luminance.
   * @param fgColor - foreground Color
   */
  bool checkForeground(const QColor &fgColor);

  /**
   * @brief Check Text color to Background luminance.
   * @param bgColor - foreground Color
   */
  bool checkBackground(const QColor &bgColor);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_INPUT_COLORLUMINANCE_H
