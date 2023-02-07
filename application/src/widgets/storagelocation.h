// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_STORAGELOCATION_H
#define ANTIQUACRM_STORAGELOCATION_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QLineEdit>

class StorageLocation final : public InputEdit {
  Q_OBJECT

private:
  AntiquaComboBox *m_box;
  QLineEdit *m_search;

private Q_SLOTS:
  void dataChanged(int);
  void filterChanged(const QString &);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  explicit StorageLocation(QWidget *parent = nullptr);
  void loadDataset() override;
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_STORAGELOCATION_H
