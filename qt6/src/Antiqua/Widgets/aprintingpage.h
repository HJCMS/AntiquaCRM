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
#include <QPrinterInfo>
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
  const QPageSize::PageSizeId p_pageSizeId;
  AntiquaCRM::ASettings *cfg;

  /**
   * @brief page margins
   */
  struct PageMargins {
    qreal left = 20;   /**< @brief Document margin left */
    qreal right = 10;  /**< @brief Document margin right */
    qreal address = 6; /**< @brief Address margin in Letter window */
    qreal subject = 6; /**< @brief Subject margin top to Letter window */
    qreal body = 10;   /**< @brief Body margin top to Subject */
  };
  PageMargins margin;

  struct PrinterInfo {
    QPrinterInfo dinA4;
    QPrinterInfo dinA6;
  };
  PrinterInfo p_printerInfo;

  /**
   * @brief 1mm to point
   * 1 mm = 2.8452755906 point (printer's)
   * 1 point (printer's) = 0.3514598035 mm
   */
  const qreal points = 2.8452755906;

  /**
   * @brief Company data, loaded by initConfiguration()
   */
  QHash<QString, QString> p_companyData;

  /**
   * @brief QTextDocument rootFrame
   * Initialed and configured in constructor
   */
  QTextFrame *rootFrame;

  /**
   * @brief Letter Heading table with address and Subject
   */
  QTextTable *headingTable;

  /**
   * @brief load configuration settings and company data
   */
  void initConfiguration();

  void paintEvent(QPaintEvent *) override;

public:
  explicit APrintingPage(QWidget *parent = nullptr,
                         QPageSize::PageSizeId id = QPageSize::A4);
  virtual ~APrintingPage();

  /**
   * @brief layout of a page in a paged document
   */
  const QPageLayout pageLayout() const;

  /**
   * @brief initial dcoument letter heading
   */
  void setLetterHeading(const QString &subject);

  /**
   * @brief insert reciepient address
   */
  void setRecipientAddress(const QString &address);

  /**
   * @brief insert letter subject and date
   */
  void setLetterSubject(const QString &subject);

  /**
   * @brief query Customer data
   * @param cId - Customer Id
   */
  const QMap<QString, QVariant> queryCustomerData(qint64 cId);

  const QPrinterInfo getPrinterInfo(QPageSize::PageSizeId id = QPageSize::A4);

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
   * @brief get Page Rect in points
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
  Q_DECL_DEPRECATED qreal inlineFrameWidth() const;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_PRINT_FORMAT_H
