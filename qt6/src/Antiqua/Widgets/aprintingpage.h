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
#include <QTextEdit>
#include <QWidget>
#include <QtGui>

namespace AntiquaCRM {

/**
 * @class APrintingPage
 * @brief TextEdit Widgets with Watermark and Textformatings
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY APrintingPage : public QTextEdit {
  Q_OBJECT

protected:
  AntiquaCRM::ASettings *cfg;

  /**
   * @brief page margins
   */
  qreal marginLeft;
  qreal marginRight;
  qreal marginSubject;
  qreal marginBody;
  qreal addressIndent;

  /**
   * 1 mm = 2.8452755906 point (printer's)
   * 1 point (printer's) = 0.3514598035 mm
   */
  const qreal points = 2.8452755906;

  /**
   * @brief Company data @ref initConfiguration()
   */
  QHash<QString, QString> p_companyData;

  /**
   * @brief QTextDocument rootFrame
   */
  QTextFrame *mainFrame;

  /**
   * @brief load configuration settings and company data
   */
  void initConfiguration();

  void paintEvent(QPaintEvent *) override;

  /**
   * @brief layout of a page in a paged document
   */
  virtual const QPageLayout pageLayout() const = 0;

public:
  explicit APrintingPage(QWidget *parent = nullptr);
  virtual ~APrintingPage();

  /**
   * @brief initial dcoument letter heading
   */
  void setLetterHeading();

  /**
   * @brief add reciepient address with customerId
   * @param cutomer - Id
   */
  QTextTable *recipientAddress(const QString &subject);

  /**
   * @brief query Customer data
   * @param cId - Customer Id
   */
  const QMap<QString, QVariant> queryCustomerData(qint64 cId);

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
  const QTextTableCellFormat addressCellFormat();

  /**
   * @brief Table cell formats
   * Only Qt::AlignTop and Qt::AlignBottom enumarations accepted!
   * If not one of this Alignments, border is disabled!
   * @param border - Set border with Vertical Alignments
   */
  const QTextTableCellFormat
  cellFormat(Qt::Alignment border = Qt::AlignBaseline);

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
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_PRINT_FORMAT_H
