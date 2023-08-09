// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRINTING_FORMAT_H
#define ANTIQUACRM_PRINTING_FORMAT_H

#include <AntiquaCRM>
#include <AntiquaPrintSupport>
#include <QPainter>
#include <QTextEdit>

namespace AntiquaCRM {

/**
 * @class APrintingPage
 * @brief TextEdit Widgets with Watermark and Textformatings
 *
 * @ingroup PrinterWidgets
 */
class ANTIQUACRM_LIBRARY APrintingPage : public QWidget, public APrintTools {
  Q_OBJECT

private:
  /**
   * @brief Company data, loaded by initConfiguration()
   */
  QHash<QString, QString> p_companyData;

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
   * @brief Inserts Document footer
   */
  void paintFooter(QPainter &painter);

  /**
   * @brief paint watermark and borders
   *
   * if you want display page borders define PRINTPAGE_DEBUG=1
   */
  virtual void paintEvent(QPaintEvent *) override final;

protected:
  /**
   * @brief System settings
   * @sa initConfiguration
   */
  AntiquaCRM::ASettings *cfg;

  /**
   * @brief Query Sql settings
   * @sa initConfiguration
   */
  AntiquaCRM::ASqlCore *m_sql = nullptr;

  /**
   * @brief a TextEdit widget for content data
   */
  APrintingBody *m_body = nullptr;

  /**
   * @brief Default Font
   * @sa initConfiguration
   */
  QFont normalFont;

  /**
   * @brief Opacity for Header attachment
   * @sa initConfiguration
   */
  qreal watermark_opacity = 1.0;

  /**
   * @brief Primary Content data ...
   * @note Required by all Painting functions!
   *
   * Most important data set in this class.
   * Inherits all Configuration fields, text bodies and article data.
   * e.g.: Article list, VAT and prices.
   *
   * @sa setContentData
   */
  QJsonObject contentData;

  /**
   * @brief Begin Painting
   */
  const QLineF footerLine() const;

  /**
   * @brief Points from left Margin to left Border
   * @code
   *  getPoints(margin.left)
   * @endcode
   */
  qreal borderLeft() const;

  /**
   * @brief Points from left Margin to right Border
   * @code
   *  pagePoints().width() - getPoints(margin.right)
   * @endcode
   */
  qreal borderRight() const;

  /**
   * @brief Inline frame Width
   * @code
   *   borderRight() - borderLeft()
   * @endcode
   */
  qreal inlineWidth() const;

  /**
   * @brief Paint Document Body
   * @param painter
   */
  virtual void paintContent(QPainter &painter) = 0;

public:
  /**
   * @param parent - parent object
   * @param id - current Printer page size id
   */
  explicit APrintingPage(QWidget *parent = nullptr,
                         QPageSize::PageSizeId id = QPageSize::A4);

  virtual ~APrintingPage();

  /**
   * @brief Set required Body objects
   */
  virtual void setBodyLayout() = 0;

  /**
   * @brief Set required Content data fpr Page layout!
   */
  virtual bool setContentData(QJsonObject &data) = 0;

  /**
   * @brief SQL-Query Customer data
   * @param cid - Customer Id
   */
  const QMap<QString, QVariant> queryCustomerData(qint64 cid);

  /**
   * @brief Company data from Configuration
   * @param key
   */
  const QString companyData(const QString &key);

  /**
   * @brief query font by configuration key
   * @note to find the right key see fontKeys()
   */
  const QFont getFont(const QString &) const;

  /**
   * @brief letter heading watermark
   */
  const QImage watermark() const;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRINTING_FORMAT_H
