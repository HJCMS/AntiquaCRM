// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COMPLETERDIALOG_H
#define COMPLETERDIALOG_H

#include <QDialog>
#include <QObject>

class CompleterView;

class CompleterDialog : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  CompleterView *m_tableView;

public:
  enum Filter { CONDITION, DESIGNATION };
  explicit CompleterDialog(
      QWidget *parent = nullptr,
      CompleterDialog::Filter filter = CompleterDialog::CONDITION);
};

#endif // COMPLETERDIALOG_H
