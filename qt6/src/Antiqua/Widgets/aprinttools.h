// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_INPUT_APRINTTOOLS_H
#define ANTIQUACRM_INPUT_APRINTTOOLS_H

#include <AGlobal>
#include <QPrinter>
#include <QPrinterInfo>
#include <QtGui>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY APrintTools {
private:
  const QPageSize pageSize;

protected:
  struct PrinterInfo {
    QPrinterInfo dinA4; /**< @brief Primary printer info */
    QPrinterInfo dinA6; /**< @brief Secundary printer info */
  };

  struct PageMargins {
    qreal top = 10;    /**< @brief Header margin top  */
    qreal left = 20;   /**< @brief Document margin left */
    qreal right = 10;  /**< @brief Document margin right */
    qreal address = 6; /**< @brief Address margin in Letter window */
    qreal subject = 6; /**< @brief Subject margin top to Letter window */
  };

  /**
   * @brief 1mm to point
   * 1 mm = 2.8452755906 point (printer's)
   * 1 point (printer's) = 0.3514598035 mm
   */
  const qreal points = 2.8452755906;

  /**
   * @brief Convert Millimeter to Points
   * @param millimeter
   */
  qreal getPoints(int millimeter) const;

  /**
   * @brief General Linestyle and for folds and paper punches!
   */
  const QPen linePen(const QColor &color = Qt::gray) const;

  /**
   * @brief Default fonts Pen
   */
  const QPen fontPen(const QColor &color = Qt::black) const;

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
   * @brief Letter Recipient Window (DIN 5008B) Position.
   * @param left - start position from left in millimeter
   */
  const QRectF letterWindow(qreal left = 25.0) const;

public:
  explicit APrintTools(QPageSize::PageSizeId id);

  /**
   * @brief Borderless Page Layout for this Document
   * @note We use a DIN A4 Portrait Layout.
   */
  const QPageLayout pageLayout() const;

  /**
   * @brief Brush for visible Borders
   */
  const QBrush borderBrush(const QColor &color = Qt::gray) const;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_INPUT_APRINTTOOLS_H
