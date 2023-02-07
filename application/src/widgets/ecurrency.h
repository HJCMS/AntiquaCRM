// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ECURRENCY_H
#define ANTIQUACRM_ECURRENCY_H

#include <AntiquaInputEdit>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QObject>

class Ecurrency final : public InputEdit {
  Q_OBJECT

private:
  QLineEdit *m_lineEdit;
  QComboBox *m_comboBox;
  void loadDataset() override;

private Q_SLOTS:
  void itemChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setFocus() override;
  void setValue(const QVariant &) override;

public:
  explicit Ecurrency(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ECURRENCY_UTILS_H
