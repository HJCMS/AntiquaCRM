// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef EDITORACTIONBAR_UTILS_H
#define EDITORACTIONBAR_UTILS_H

#include <QFlags>
#include <QObject>
#include <QPushButton>
#include <QWidget>

#include "printerbutton.h"

class EditorActionBar final : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QPushButton *m_restoreBtn;
  PrinterButton *m_printerButton;
  QPushButton *m_cancelBtn;
  QPushButton *m_saveBtn;
  QPushButton *m_readyBtn;

Q_SIGNALS:
  void s_restoreClicked();
  void s_printDeliveryNote();
  void s_printInvoiceNote();
  void s_printPaymentReminder();
  void s_printBookCard();
  void s_cancelClicked();
  void s_saveClicked();
  void s_finishClicked();

public Q_SLOTS:
  void setRestoreable(bool);

public:
  explicit EditorActionBar(QWidget *parent = nullptr);
  void setPrinterButtons(PrinterButton::Buttons buttons);
  bool isRestoreable();
};

#endif // EDITORACTIONBAR_UTILS_H
