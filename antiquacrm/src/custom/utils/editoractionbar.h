// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef EDITORACTIONBAR_UTILS_H
#define EDITORACTIONBAR_UTILS_H

#include <QFlags>
#include <QObject>
#include <QPushButton>
#include <QWidget>

class EditorActionBar final : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QPushButton *m_restoreBtn;
  QPushButton *m_printDeliveryBtn;
  QPushButton *m_printInvoiceBtn;
  QPushButton *m_printBookCardBtn;
  QPushButton *m_cancelBtn;
  QPushButton *m_saveBtn;
  QPushButton *m_readyBtn;

Q_SIGNALS:
  void s_restoreClicked();
  void s_printDeliveryNote();
  void s_printInvoiceNote();
  void s_printBookCard();
  void s_cancelClicked();
  void s_saveClicked();
  void s_finishClicked();

public Q_SLOTS:
  void setRestoreable(bool);

public:
  explicit EditorActionBar(QWidget *parent = nullptr);
  void viewPrintButton(bool b = false);
  void viewPrintBookCardButton(bool b = false);
  bool isRestoreable();
};

#endif // EDITORACTIONBAR_UTILS_H
