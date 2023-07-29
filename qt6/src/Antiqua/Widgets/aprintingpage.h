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
#include <AntiquaPrintSupport>
#include <QPainter>
#include <QTextEdit>
#include <QTextFrame>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class APrintingPage
 * @brief TextEdit Widgets with Watermark and Textformatings
 * @ingroup PrinterWidgets
 */
class ANTIQUACRM_LIBRARY APrintingPage : public QTextEdit, public APrintTools {
  Q_OBJECT

private:
  /**
   * @brief printer info class
   * Called with @ref getPrinterInfo.
   * Initialed in @ref initConfiguration()
   */
  PrinterInfo p_printerInfo;

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
  /**
   * @brief Query Sql settings
   */
  AntiquaCRM::ASqlCore *m_sql = nullptr;

  /**
   * @brief System settings
   */
  AntiquaCRM::ASettings *cfg;

  /**
   * @brief QTextDocument rootFrame
   * Initialed and configured in constructor
   */
  QTextFrame *rootFrame;

  /**
   * @brief get Painting width() minus margins in points!
   */
  const QRectF paintArea() const;

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
  const QTextTableCellFormat cellFormat(Qt::Alignment border = Qt::AlignBaseline);

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
