// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerorderinfo.h"

#include <QDebug>
#include <QLabel>
#include <QLayout>

ProviderOrderInfo::ProviderOrderInfo(QWidget *parent) : QWidget{parent} {
  setContentsMargins(5, 0, 5, 0);

  int r = 0;
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 2, 0, 2);

  QLabel *lb = new QLabel(this);
  lb->setText(tr("Displays the current payment information."));
  layout->addWidget(lb, r++, 0, 1, 2, Qt::AlignLeft);

  m_since = new AntiquaDateInfo(this);
  m_since->setObjectName("o_since");
  m_since->setInfo(tr("Ordered at"));
  layout->addWidget(m_since, r++, 0, 1, 2, Qt::AlignLeft);

  m_paymentMethod = new PaymentMethodSelecter(this);
  m_paymentMethod->setObjectName("o_payment_method");
  m_paymentMethod->setInfo(tr("Payment Method"));
  layout->addWidget(m_paymentMethod, r++, 0, 1, 2, Qt::AlignLeft);

  m_orderPayed = new LineEdit(this);
  m_orderPayed->setObjectName("o_payment_paypal_txn_id");
  m_orderPayed->setInfo(tr("Payed"));
  layout->addWidget(m_orderPayed, r, 0, 1, 1);

  m_paymentConfirmed = new LineEdit(this);
  m_paymentConfirmed->setObjectName("o_payment_confirmed");
  m_paymentConfirmed->setInfo(tr("Confirmed"));
  layout->addWidget(m_paymentConfirmed, r++, 1, 1, 1);

  m_orderStatus = new PaymentStatusSelecter(this);
  m_orderStatus->setObjectName("o_provider_order_status");
  m_orderStatus->setInfo(tr("Order status"));
  m_orderStatus->setEnabled(false);
  layout->addWidget(m_orderStatus, r++, 0, 1, 2, Qt::AlignLeft);

  layout->setRowStretch(r++, 1);
  setLayout(layout);
}

void ProviderOrderInfo::setOrderInfo(const QJsonObject &info) {
  if (info.isEmpty())
    return;

  foreach (QString k, info.keys()) {
    InputEdit *inp = findChild<InputEdit *>(k);
    if (inp != nullptr)
      inp->setValue(info.value(k));
  }
}
