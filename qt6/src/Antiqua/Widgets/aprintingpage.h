// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_PRINT_FORMAT_H
#define ANTIQUACRM_WIDGETS_PRINT_FORMAT_H

#include <AntiquaCRM>
#include <QPrinterInfo>
#include <QRegularExpression>
#include <QTextEdit>
#include <QWidget>
#include <QtGui>

namespace AntiquaCRM {

/**
 * @class APrintingPage
 * @brief TextEdit Widgets with Watermark and Textformatings
 * @ingroup PrinterWidgets
 */
class ANTIQUACRM_LIBRARY APrintingPage : public QTextEdit {
  Q_OBJECT

private:
  struct PrinterInfo {
    QPrinterInfo dinA4; /**< @brief Primary printer info */
    QPrinterInfo dinA6; /**< @brief Secundary printer info */
  };

  /**
   * @brief printer info class
   * Called with @ref getPrinterInfo.
   * Initialed in @ref initConfiguration()
   */
  PrinterInfo p_printerInfo;

  struct PageMargins {
    qreal top = 10;    /**< @brief Header margin top  */
    qreal left = 20;   /**< @brief Document margin left */
    qreal right = 10;  /**< @brief Document margin right */
    qreal address = 6; /**< @brief Address margin in Letter window */
    qreal subject = 6; /**< @brief Subject margin top to Letter window */
  };

  const QPen linePen = QPen(Qt::gray);
  const QPen fontPen = QPen(Qt::black);

  const QString toRichText(const QString &) const;

  /**
   * @brief load configuration settings and company data
   * @note This function is called in constructer
   */
  void initConfiguration();

  /**
   * @brief Inserts Document Header title and Attachment
   */
  void paintHeader(QPainter &painter);

  /**
   * @brief Inserts Document Recipient and Letter Window
   */
  void paintAddressBox(QPainter &painter);

  /**
   * @brief Inserts Document Identities
   */
  void paintIdentities(QPainter &painter);

  /**
   * @brief Inserts Document Subject and Date
   */
  void paintSubject(QPainter &painter);

  /**
   * @brief Inserts Document footer
   */
  void paintFooter(QPainter &painter);

  /**
   * @brief paint watermark and borders
   * @short define DEBUG_DISPLAY_BORDERS before load this class
   * if you want display painted borders
   */
  virtual void paintEvent(QPaintEvent *) override final;

protected:
  AntiquaCRM::ASqlCore *m_sql = nullptr;

  /**
   * @brief Fixed page size
   */
  const QPageSize::PageSizeId p_pageSizeId;

  /**
   * @brief System settings
   */
  AntiquaCRM::ASettings *cfg;

  /**
   * @brief current page margins
   * Initialed in initConfiguration()
   */
  PageMargins margin;

  /**
   * @brief Company data, loaded by initConfiguration()
   */
  QHash<QString, QString> p_companyData;

  /**
   * @brief Primary Content data to fill
   */
  QJsonObject p_content_data;

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
   * @brief get Page pressure Area Rect in points
   * @note without margins!
   */
  const QRectF pointsRect() const;

  /**
   * @brief get Painting width() minus margins in points!
   */
  const QRectF paintArea() const;

  /**
   * @brief Letter Recipient Window (DIN 5008B) Position.
   * @param left - start position from left in millimeter
   */
  const QRectF letterWindow(qreal left = 25.0) const;

  /**
   * @brief Border left in points
   */
  qreal borderLeft() const;

  /**
   * @brief Border right from left in points
   */
  qreal borderRight() const;

public:
  /**
   * @param parent - parent object
   * @param id - current Printer page size id
   */
  explicit APrintingPage(QWidget *parent = nullptr,
                         QPageSize::PageSizeId id = QPageSize::A4);

  virtual ~APrintingPage();

  /**
   * @brief Borderless Page Layout for this Document
   * @note We use a DIN A4 Portrait Layout.
   */
  const QPageLayout pageLayout() const;

  /**
   * @brief Create Document Body
   * @param oid = Order Id
   * @param cid = Customer Id
   */
  virtual void setBody(qint64 oid, qint64 cid) = 0;

  /**
   * @brief SQL-Query Customer data
   * @param cid - Customer Id
   */
  const QMap<QString, QVariant> queryCustomerData(qint64 cid);

  /**
   * @brief Call Printer Info for PageSize Id
   * @param id - Default: Din A4 Portrait
   */
  const QPrinterInfo getPrinterInfo(QPageSize::PageSizeId id = QPageSize::A4);

  /**
   * @brief General Linestyle and for folds and paper punches!
   */
  const QPen penStyle() const;

  /**
   * @brief Brush for visible Borders
   */
  const QBrush borderBrush() const;

  /**
   * @brief Standard text format
   */
  const QTextCharFormat charFormat(const QFont &f, bool bolded = false);

  /**
   * @brief Body Text Block align left
   */
  const QTextBlockFormat bodyText();

  /**
   * @brief Text Block align right
   */
  const QTextBlockFormat alignRight();

  /**
   * @brief Text Block align Center
   */
  const QTextBlockFormat alignCenter();

  /**
   * @brief Create Centered Cell Format
   */
  const QTextCharFormat verticalCenter();

  /**
   * @brief Text Table formats
   */
  const QTextTableFormat tableFormat();
  const QTextTableFormat inlineTableFormat();

  /**
   * @brief Text Table Cell formats
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

  bool setContentData(const QJsonObject &data);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_PRINT_FORMAT_H
