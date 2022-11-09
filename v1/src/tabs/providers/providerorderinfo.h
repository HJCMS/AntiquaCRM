// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDERORDERINFO_H
#define ANTIQUACRM_PROVIDERORDERINFO_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>
#include <QJsonObject>

class ProviderOrderInfo : public QWidget {
  Q_OBJECT

private:
  AntiquaDateInfo *m_since;
  PaymentMethodSelecter *m_paymentMethod;
  LineEdit *m_orderPayed;
  LineEdit *m_paymentConfirmed;
  PaymentStatusSelecter *m_orderStatus;

public:
  explicit ProviderOrderInfo(QWidget *parent = nullptr);
  void setOrderInfo(const QJsonObject &info);
};

#endif // ANTIQUACRM_PROVIDERORDERINFO_H
