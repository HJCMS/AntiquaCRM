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
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class InvoiceLetter
 * @brief Invoice Painting Device
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY InvoiceLetter final : public QWidget {
  Q_OBJECT

private:
  AntiquaCRM::APrintingFormat *format;
  const QStaticText addressBlock(const QStringList &) const;
  void paintEvent(QPaintEvent *) override;

public:
  explicit InvoiceLetter(AntiquaCRM::APrintingFormat *formatting,
                         QWidget *parent = nullptr);
};

/**
 * @class PrintInvoice
 * @brief Printing Invoices Dialog
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY PrintInvoice final : public APrintDialog {
  Q_OBJECT

private:
  InvoiceLetter *device;

  bool generateDocument(QPrinter *printer) override;

private Q_SLOTS:
  void openPrintDialog() override;

public:
  explicit PrintInvoice(QWidget *parent = nullptr);
  int exec(const QStringList &options) override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_PRINT_INVOICE_H
