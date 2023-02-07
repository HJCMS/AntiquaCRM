// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_EDITINFOLABEL_H
#define ANTIQUACRM_EDITINFOLABEL_H

#include <AntiquaInputEdit>

class EditInfoLabel final : public InputEdit {
  Q_OBJECT

private:
  void loadDataset() override;

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  explicit EditInfoLabel(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_EDITINFOLABEL_H
