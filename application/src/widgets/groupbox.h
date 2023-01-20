// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_GROUPBOX_H
#define ANTIQUACRM_GROUPBOX_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QGroupBox>

class GroupBox final : public InputEdit {
  Q_OBJECT

private:
  void loadDataset() {};

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  QGroupBox *box;
  explicit GroupBox(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_GROUPBOX_H
