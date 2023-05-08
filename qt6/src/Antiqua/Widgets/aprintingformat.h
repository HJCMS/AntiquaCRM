// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_PRINT_FORMAT_H
#define ANTIQUACRM_WIDGETS_PRINT_FORMAT_H

#include <ASettings>
#include <QObject>
#include <QtGui>

namespace AntiquaCRM {

/**
 * @class APrintingFormat
 * @brief Printer formats
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY APrintingFormat final : public QObject {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *cfg;

  /**
   * @brief page margins
   */
  qreal marginLeft;
  qreal marginRight;
  qreal marginSubject;
  qreal marginBody;
  qreal marginRecipient;

  /**
   * 1 mm = 2.8452755906 point (printer's)
   * 1 point (printer's) = 0.3514598035 mm
   */
  const qreal points = 2.8452755906;

  void initConfiguration();

  /**
   * @brief Company data @ref initConfiguration()
   */
  QHash<QString, QString> p_companyData;

public:
  explicit APrintingFormat(AntiquaCRM::ASettings *config);
  virtual ~APrintingFormat();

  /**
   * @brief layout of a page in a paged document
   */
  const QPageLayout pageLayout(QPageSize::PageSizeId id = QPageSize::A4) const;

  /**
   * @brief General Linestyle and for folds and paper punches!
   */
  const QPen penStyle() const;

  /**
   * @brief Table border color
   */
  const QBrush borderBrush() const;

  /**
   * @brief Standard text format for document content
   */
  const QTextCharFormat charFormat(const QFont &f, bool bolded = false);

  /**
   * @brief Text Block align right
   */
  const QTextBlockFormat alignRight();

  /**
   * @brief Text Block align Center
   */
  const QTextBlockFormat alignCenter();

  /**
   * @brief Table formats
   */
  const QTextTableFormat tableFormat();
  const QTextTableFormat inlineTableFormat();

  /**
   * @brief Table cell formats
   */
  const QTextTableCellFormat recipientCellFormat();

  /**
   * @brief Table cell formats
   * Only Qt::AlignTop and Qt::AlignBottom enumarations accepted!
   * If not one of this Alignments, border is disabled!
   * @param border - Set border with Vertical Alignments
   */
  const QTextTableCellFormat cellFormat(Qt::Alignment border = Qt::AlignTop);

  /**
   * @brief Company data from Configuration
   * @param key
   */
  const QString companyData(const QString &key);

  /**
   * @brief Key list for add Fonts in printer configuration.
   */
  const QStringList fontKeys() const;

  /**
   * @brief query font by configuration key
   * @note to find the right key see fontKeys()
   */
  const QFont getFont(const QString &) const;

  /**
   * @brief letter heading watermark transparency
   */
  qreal watermarkOpacity() const;

  /**
   * @brief letter heading watermark
   */
  const QImage watermark() const;

  /**
   * @brief get DIN A4 Rect in mm
   * @note without margins!
   */
  const QRectF letterRect() const;

  /**
   * @brief get DIN A4 Rect in pixel
   * @note without margins!
   */
  const QRectF pointsRect() const;

  qreal getPoints(int mm) const;

  /**
   * @brief get Letter Window rect for DIN 5008B
   */
  const QRectF letterWindowRect() const;

  /**
   * @brief Border left in points
   */
  qreal borderLeft() const;

  /**
   * @brief Border right from left in points
   */
  qreal borderRight() const;

  /**
   * @brief get Painting width() minus margins in points!
   */
  qreal inlineFrameWidth() const;

  qreal recipientPadding() const;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_PRINT_FORMAT_H
