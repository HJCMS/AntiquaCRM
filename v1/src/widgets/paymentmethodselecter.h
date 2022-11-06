// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PAYMENTMETHODSELECTER_H
#define ANTIQUACRM_PAYMENTMETHODSELECTER_H

#include <AGlobal>
#include <AntiquaInputEdit>
#include <QMap>
#include <QObject>
#include <QPair>
#include <QWidget>

class PaymentMethodSelecter final : public InputEdit {
  Q_OBJECT

private:
  QMap<int, QPair<QString, AntiquaCRM::PaymentMethod>> p_map;
  AntiquaComboBox *m_box;
  void loadDataset();

private Q_SLOTS:
  void dataChanged(int);

Q_SIGNALS:
  void sendPaymentMethodChanged(AntiquaCRM::PaymentMethod);

public Q_SLOTS:
  Q_INVOKABLE void reset();
  void setValue(const QVariant &);
  void setFocus();

public:
  explicit PaymentMethodSelecter(QWidget *parent = nullptr);
  void setProperties(const QSqlField &field);
  const QVariant value();
  bool isValid();
  void setInfo(const QString &);
  const QString info();
  const QString notes();
};

#endif // ANTIQUACRM_PAYMENTMETHODSELECTER_H
