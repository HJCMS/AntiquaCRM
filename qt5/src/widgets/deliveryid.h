// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_DELIVERYID_H
#define ANTIQUACRM_DELIVERYID_H

#include <AntiquaInputEdit>
#include <QObject>
#include <QWidget>

/**
 * @brief The DeliveryId class
 * @ingroup widgets
 */
class DeliveryId final : public InputEdit {
  Q_OBJECT

private:
  AntiquaLineEdit *m_edit;
  void loadDataset() override;

private Q_SLOTS:
  void inputChanged(const QString &);

public Q_SLOTS:
  void setValue(const QVariant &) override;
  Q_INVOKABLE void reset() override;
  void setFocus() override;

public:
  explicit DeliveryId(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QVariant value() override;
  bool isValid() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_DELIVERYID_H
