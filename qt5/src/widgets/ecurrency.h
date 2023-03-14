// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_ECURRENCY_H
#define ANTIQUACRM_ECURRENCY_H

#include <AntiquaInputEdit>
#include <QLabel>
#include <QLineEdit>
#include <QObject>

/**
 * @brief The Ecurrency class
 * @ingroup widgets
 */
class Ecurrency final : public InputEdit {
  Q_OBJECT

private:
  QLineEdit *m_lineEdit;
  AntiquaComboBox *m_box;
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
