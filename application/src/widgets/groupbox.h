// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_GROUPBOX_H
#define ANTIQUACRM_GROUPBOX_H

#include <AntiquaInputEdit>
#include <QGroupBox>

class GroupBox final : public InputEdit {
  Q_OBJECT

private:
  void loadDataset() override{};

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  QGroupBox *box;
  explicit GroupBox(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_GROUPBOX_H
