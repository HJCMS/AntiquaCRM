// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_POSTALCODEEDIT_H
#define ANTIQUACRM_POSTALCODEEDIT_H

#include <QComboBox>
#include <QCompleter>
#include <QLabel>
#include <QLineEdit>
#include <QWidget>

#include "inputedit.h"

class PostalCodeModel;

class PostalCodeEdit : public InputEdit {
  Q_OBJECT

private:
  QLabel *m_label;
  QLineEdit *m_postalcode;
  QComboBox *m_countries;

  QCompleter *m_completer;
  PostalCodeModel *m_completerModel;

  void loadDataset();

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit PostalCodeEdit(QWidget *parent = nullptr);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_POSTALCODEEDIT_H
