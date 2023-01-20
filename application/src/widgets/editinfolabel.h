// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_EDITINFOLABEL_H
#define ANTIQUACRM_EDITINFOLABEL_H

#include <AGlobal>
#include <AntiquaInputEdit>

class EditInfoLabel final : public InputEdit {
  Q_OBJECT

private:
  void loadDataset();

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit EditInfoLabel(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_EDITINFOLABEL_H
