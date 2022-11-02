// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PHONEEDIT_H
#define ANTIQUACRM_PHONEEDIT_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QRegExp>
#include <QRegExpValidator>
#include <QRegularExpression>

class PhoneEdit final : public InputEdit {
  Q_OBJECT

private:
  AntiquaLineEdit *m_edit;
  QRegExpValidator *m_validator;
  void loadDataset() {};

private Q_SLOTS:
  void dataChanged(const QString &);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit PhoneEdit(QWidget *parent = nullptr);
  static const QRegExp rePattern();
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_PHONEEDIT_H
