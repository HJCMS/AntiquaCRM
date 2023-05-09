// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_PRINT_INVOICE_H
#define ANTIQUACRM_WIDGETS_PRINT_INVOICE_H

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
class ANTIQUACRM_LIBRARY InvoicePage final
    : public AntiquaCRM::APrintingPage {
  Q_OBJECT

private:
  const QPageLayout pageLayout() const override;

public:
  explicit InvoicePage(QWidget *parent = nullptr);
  void setLetterSubject(const QString &);
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

  bool generateDocument(QPrinter *printer) override;

private Q_SLOTS:
  void openPrintDialog() override;

public:
  explicit PrintInvoice(QWidget *parent = nullptr);
  int exec(const QJsonObject &options) override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_PRINT_INVOICE_H
