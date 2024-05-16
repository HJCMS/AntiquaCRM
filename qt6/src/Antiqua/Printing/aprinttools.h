// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRINTING_APRINTTOOLS_H
#define ANTIQUACRM_PRINTING_APRINTTOOLS_H

#include <AGlobal>
#include <QPrinter>
#include <QPrinterInfo>
#include <QtGui>

namespace AntiquaCRM {

/**
 * @class APrintTools
 * @brief Utilities for APrintingPage
 *
 * @ingroup PrinterWidgets
 */
class ANTIQUACRM_LIBRARY APrintTools {
private:
  /**
   * @brief Current PageSize
   */
  const QPageSize pageSize;

  /**
   * @brief 1mm to point
   * 1 mm = 2.8452755906 point (printer's)
   * 1 point (printer's) = 0.3514598035 mm
   */
  const qreal points = 2.8452755906;

protected:
  /**
   * @brief current page margins
   * Initialed in @sa APrintingPage::initConfiguration()
   */
  QMarginsF margin = QMarginsF(0, 0, 0, 0);

  /**
   * @brief Convert Millimeter to Points
   *
   * 1 mm = 2.8452755906 point (printer's)
   * 1 point (printer's) = 0.3514598035 mm
   *
   * @param millimeter
   */
  qreal getPoints(int millimeter) const;

  /**
   * @brief Default Static text
   */
  const QStaticText
  textBlock(Qt::Alignment align = Qt::AlignLeft,
            QTextOption::WrapMode mode = QTextOption::ManualWrap,
            Qt::TextFormat format = Qt::RichText) const;

  /**
   * @brief Convert linebreaks to html br
   */
  const QString toRichText(const QString &) const;

  /**
   * @brief get Page pressure Area Rect in points
   * @note without margins!
   */
  const QRectF pagePoints() const;

  /**
   * @brief Letter Recipient Window (DIN 5008) Position.
   * @param left - start position from left in millimeter
   */
  const QRectF letterWindow(qreal left = 25.0) const;

  /**
   * @brief Letter Recipient Address zone (DIN 5008) Rect.
   *
   * A Recipient address zone is 80mm width and 40mm height and must inside of
   * Letter Window rectangle.
   *
   * @sa APrintTools::letterWindow()
   */
  const QRectF addressZone() const;

public:
  explicit APrintTools(QPageSize::PageSizeId id);

  /**
   * @brief Borderless Page Layout for this Document
   * @note We use a DIN A4 Portrait Layout.
   */
  const QPageLayout pageLayout() const;

  /**
   * @brief Default painter drawText option
   * @param align - Alignment
   * @param mode  - WrapMode
   */
  const QTextOption textOption(Qt::Alignment align = Qt::AlignLeft,
                               QTextOption::WrapMode mode = QTextOption::NoWrap);

  /**
   * @brief General Linestyle and for folds and paper punches!
   */
  const QPen linePen(const QColor &color = Qt::gray) const;

  /**
   * @brief Default fonts Pen
   */
  const QPen fontPen(const QColor &color = Qt::black) const;

  /**
   * @brief get Font height in Points
   * @param font
   */
  qreal fontHeight(const QFont &font) const;

  /**
   * @brief Brush for visible Borders
   */
  const QBrush borderBrush(const QColor &color = Qt::gray) const;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRINTING_APRINTTOOLS_H
