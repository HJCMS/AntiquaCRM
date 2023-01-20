// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SHOWPAYMENTMETHOD_H
#define ANTIQUACRM_SHOWPAYMENTMETHOD_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QLabel>
#include <QMap>

class ShowPaymentMethod final : public InputEdit {
  Q_OBJECT

private:
  QLabel *m_text;
  AntiquaCRM::PaymentMethod p_changed = AntiquaCRM::PAYMENT_NOT_SET;
  QMap<AntiquaCRM::PaymentMethod, QString> p_data;
  void loadDataset();

private Q_SLOTS:
  void dataChanged(int);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit ShowPaymentMethod(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_SHOWPAYMENTMETHOD_H
