// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRINTING_DIALOG_H
#define ANTIQUACRM_PRINTING_DIALOG_H

#include <AntiquaCRM>
#include <AntiquaPrintSupport>
#include <QPushButton>
#include <QScrollArea>
#include <QtWidgets>

namespace AntiquaCRM {

class APrintingPage;

/**
 * @class APrintDialog
 * @brief Abstract Printing Dialog
 *
 * @ingroup PrinterWidgets
 */
class ANTIQUACRM_LIBRARY APrintDialog : public QDialog {
  Q_OBJECT

private:
  QDialogButtonBox *m_buttonBox;
  QStatusBar *m_statusBar;

protected:
  AntiquaCRM::ASettings *config;
  QPageLayout pageLayout;
  QString pdfFileName;
  QPushButton *btn_pdf;
  QScrollArea *viewPort;

  void setPrintingPage(AntiquaCRM::APrintingPage *page);

protected Q_SLOTS:
  /**
   * @brief render current page
   * @param printer - initialed printer dialog
   */
  virtual void renderPage(QPrinter *printer) = 0;

  /**
   * @brief create PDF ...
   */
  virtual void createPDF() = 0;

  /**
   * @brief call printer dialog
   */
  virtual void openPrintDialog() = 0;

public Q_SLOTS:
  /**
   * @brief status bar messanger
   * @param message - body
   */
  void sendStatusMessage(const QString &message);

public:
  /**
   * @param parent - parent object
   */
  explicit APrintDialog(QWidget *parent = nullptr);

  /**
   * @brief Call Printer Info for PageSize Id
   * @param id - Default: Din A4 Portrait
   */
  const QPrinterInfo getPrinterInfo(QPageSize::PageSizeId id = QPageSize::A4);

  /**
   * @brief prevent dialog without options
   * @return QDialog::Rejected
   */
  int exec() override final;

  /**
   * @brief execute with options
   * @param options - Json Object
   * @param pdfbtn - Enable PDF Export Button
   * @return QDialog::DialogCode
   */
  virtual int exec(const QJsonObject &options, bool pdfbtn = true) = 0;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRINTING_DIALOG_H
