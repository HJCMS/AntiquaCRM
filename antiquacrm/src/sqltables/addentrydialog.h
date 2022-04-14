// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ADDENTRYDIALOG_H
#define ADDENTRYDIALOG_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtWidgets/QDialog>
#include <QtWidgets/QWidget>

class StrLineEdit;

class AddEntryDialog : public QDialog {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  StrLineEdit *m_lineEdit;

public:
  explicit AddEntryDialog(QWidget *parent = nullptr);
  const QVariant value();
};

#endif // ADDENTRYDIALOG_H
