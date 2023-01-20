// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_EMAILEDIT_H
#define ANTIQUACRM_EMAILEDIT_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QRegExpValidator>

class EMailEdit final : public InputEdit {
  Q_OBJECT

private:
  AntiquaLineEdit *m_edit;
  QRegExpValidator *m_validator;
  void loadDataset() {};
  bool validate(const QString &mail) const;

private Q_SLOTS:
  void dataChanged(const QString &);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit EMailEdit(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_EMAILEDIT_H