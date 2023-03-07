// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SHOWPAYMENTMETHOD_H
#define ANTIQUACRM_SHOWPAYMENTMETHOD_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QLabel>
#include <QMap>

/**
 * @brief The ShowPaymentMethod class
 * @ingroup widgets
 */
class ShowPaymentMethod final : public InputEdit {
  Q_OBJECT

private:
  QLabel *m_text;
  AntiquaCRM::PaymentMethod p_changed = AntiquaCRM::PAYMENT_NOT_SET;
  QMap<AntiquaCRM::PaymentMethod, QString> p_data;
  void loadDataset() override;

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset() override;
  void setValue(const QVariant &) override;
  void setFocus() override;

public:
  explicit ShowPaymentMethod(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field) override;
  const QVariant value() override;
  bool isValid() override;
  void setInfo(const QString &) override;
  const QString info() override;
  const QString notes() override;
};

#endif // ANTIQUACRM_SHOWPAYMENTMETHOD_H
