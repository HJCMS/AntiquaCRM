// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_PRINT_DIALOG_H
#define ANTIQUACRM_WIDGETS_PRINT_DIALOG_H

#include <AntiquaCRM>
#include <QObject>
#include <QPrinter>
#include <QScrollArea>
#include <QtWidgets>

namespace AntiquaCRM {

class APrintingPage;

/**
 * @class APrintDialog
 * @brief Abstract Printing Dialog
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
   * @param - Message
   */
  void sendStatusMessage(const QString &);

public:
  explicit APrintDialog(QWidget *parent = nullptr);

  /**
   * @brief prevent dialog without options
   * @return QDialog::Rejected
   */
  int exec() override final;

  /**
   * @brief execute with options
   * @param options - Json Object
   * @return QDialog::DialogCode
   */
  virtual int exec(const QJsonObject &options) = 0;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_PRINT_DIALOG_H
