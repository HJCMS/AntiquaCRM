// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PAYMENTMETHODSELECTER_H
#define ANTIQUACRM_PAYMENTMETHODSELECTER_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QMap>
#include <QPair>

/**
 * @brief The PaymentMethodSelecter class
 * @ingroup widgets
 */
class PaymentMethodSelecter final : public InputEdit {
  Q_OBJECT

private:
  QMap<int, QPair<QString, AntiquaCRM::PaymentMethod>> p_map;
  AntiquaComboBox *m_box;
  void loadDataset() override;

private Q_SLOTS:
  void dataChanged(int);

Q_SIGNALS:
  void sendPaymentMethodChanged(AntiquaCRM::PaymentMethod);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;
  void setReadOnly(bool b = false);

public:
  explicit PaymentMethodSelecter(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;

  static const QMap<AntiquaCRM::PaymentMethod,QString> getMethodes();
};

#endif // ANTIQUACRM_PAYMENTMETHODSELECTER_H
