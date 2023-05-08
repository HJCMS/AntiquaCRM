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

class APrintingFormat;

/**
 * @class APrintDialog
 * @brief Primary Printing Dialog
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY APrintDialog : public QDialog {
  Q_OBJECT

private:
  QToolBar *m_toolBar;
  QDialogButtonBox *m_buttonBox;
  QStatusBar *m_statusBar;

protected:
  AntiquaCRM::ASettings *config;

  QScrollArea *viewPort;

  AntiquaCRM::APrintingFormat *initFormatting();

  void setPrintingPage(QWidget *page);

  virtual bool generateDocument(QPrinter *printer) = 0;

protected Q_SLOTS:
  virtual void openPrintDialog() = 0;

public:
  explicit APrintDialog(QWidget *parent = nullptr);

  /**
   * @brief prevent dialog without options
   * @return QDialog::Rejected
   */
  int exec() override final;

  /**
   * @brief execute with options
   * @param options
   * @return QDialog::DialogCode
   */
  virtual int exec(const QStringList &options) = 0;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_PRINT_DIALOG_H
