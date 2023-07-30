// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
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
  struct PageMargins {
    qreal top = 10;    /**< @brief Header margin top  */
    qreal left = 20;   /**< @brief Document margin left */
    qreal right = 10;  /**< @brief Document margin right */
    qreal address = 6; /**< @brief Address margin in Letter window */
    qreal subject = 6; /**< @brief Subject margin top to Letter window */
  };

  /**
   * @brief current page margins
   * Initialed in @ref APrintingPage::initConfiguration()
   */
  PageMargins margin;

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
            QTextOption::WrapMode mode = QTextOption::ManualWrap) const;

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
