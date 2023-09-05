// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRINTING_INVOICE_H
#define ANTIQUACRM_PRINTING_INVOICE_H

#include <AntiquaPrintDialog>
#include <QObject>
#include <QPageSize>
#include <QTextEdit>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class InvoicePage
 * @brief Invoice Painting Device
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY InvoicePage final : public AntiquaCRM::APrintingPage {
  Q_OBJECT

private:
  QJsonObject config;
  QTextTable *m_articles = nullptr;
  double summary = 0.00;
  void paintContent(QPainter &painter) override;
  virtual void setBodyLayout() override;
  void setArticleCell(int row, const QVariant &value);
  void setDescripeCell(int row, const QVariant &value);
  void setQuantityCell(int row, const QVariant &value);
  int addArticleRows(int row, const QSqlQuery &result);

public:
  explicit InvoicePage(QWidget *parent = nullptr);
  bool setContentData(QJsonObject &data) override;
};

/**
 * @class PrintInvoice
 * @brief Printing Invoices Dialog
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY PrintInvoice final : public APrintDialog {
  Q_OBJECT

private:
  InvoicePage *page;

private Q_SLOTS:
  void renderPage(QPrinter *printer) override;
  void createPDF() override;
  void openPrintDialog() override;

public:
  explicit PrintInvoice(QWidget *parent = nullptr);
  int exec(const QJsonObject &options, bool pdfbtn = true) override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRINTING_INVOICE_H
