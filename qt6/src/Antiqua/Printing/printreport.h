// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRINTING_REPORT_H
#define ANTIQUACRM_PRINTING_REPORT_H

#include <AntiquaPrintDialog>
#include <QJsonObject>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM {

/**
 * @class PrintReport
 * @brief Printing monthly report
 *
 * @ingroup PrinterWidgets
 */
class ANTIQUACRM_LIBRARY PrintReport final : public AntiquaCRM::APrintDialog {
  Q_OBJECT

private:
  APrintingBody *page;

private Q_SLOTS:
  void renderPage(QPrinter *printer) override;
  void createPDF() override;
  void openPrintDialog() override;

public:
  explicit PrintReport(QWidget *parent = nullptr);
  int exec(const QJsonObject &opts, bool pdfbtn = true) override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRINTING_REPORT_H
