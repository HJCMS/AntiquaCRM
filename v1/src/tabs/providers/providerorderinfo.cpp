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

  layout->addWidget(infoCell(tr("Ordered at")), r, 0, 1, 1);
  m_since = new AntiquaDateInfo(this);
  layout->addWidget(m_since, r++, 1, 1, 1);

  layout->addWidget(infoCell(tr("Payment Method")), r, 0, 1, 1);
  m_paymentMethod = new QLabel(this);
  layout->addWidget(m_paymentMethod, r++, 1, 1, 1);

  layout->addWidget(infoCell(tr("Payed")), r, 0, 1, 1);
  m_orderPayed = new QLabel(this);
  layout->addWidget(m_orderPayed, r++, 1, 1, 1);

  layout->addWidget(infoCell(tr("Confirmed")), r, 0, 1, 1);
  m_paymentConfirmed = new AntiquaDateInfo(this);
  m_paymentConfirmed->setObjectName("o_payment_confirmed");
  layout->addWidget(m_paymentConfirmed, r++, 1, 1, 1);

  layout->addWidget(infoCell(tr("Order status")), r, 0, 1, 1);
  m_orderStatus = new QLabel(this);
  layout->addWidget(m_orderStatus, r++, 1, 1, 1);

  layout->setRowStretch(r++, 1);
  setLayout(layout);
}

QLabel *ProviderOrderInfo::infoCell(const QString &info) {
  QLabel *m = new QLabel(this);
  m->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  m->setText(info + ":");
  return m;
}

const QDateTime ProviderOrderInfo::convertDateTime(const QString &str) {
  QDateTime dt = QDateTime::fromString(str, Qt::ISODate);
  dt.setTimeSpec(Qt::OffsetFromUTC);
  return dt;
}

void ProviderOrderInfo::setPaymentStatus(int i) {
  AntiquaCRM::PaymentStatus ps = static_cast<AntiquaCRM::PaymentStatus>(i);
  QString info = PaymentStatusSelecter::getStatus().value(ps);
  m_orderStatus->setText(info);
}

void ProviderOrderInfo::setPaymentMethod(int i) {
  AntiquaCRM::PaymentMethod pm = static_cast<AntiquaCRM::PaymentMethod>(i);
  QString info = PaymentMethodSelecter::getMethodes().value(pm);
  m_paymentMethod->setText(info);
}

void ProviderOrderInfo::setOrderInfo(const QJsonObject &info) {
  if (info.isEmpty())
    return;

  if (info.contains("o_since")) {
    QString o_s = info.value("o_since").toString();
    m_since->setValue(convertDateTime(o_s));
  }

  if (info.contains("o_payment_method")) {
    setPaymentMethod(info.value("o_payment_method").toInt());
  }

  if (info.contains("o_payment_paypal_txn_id")) {
    QString o_ppti = info.value("o_payment_paypal_txn_id").toString();
    m_orderPayed->setText(o_ppti);
  }

  if (info.contains("o_payment_confirmed")) {
    QString o_pc = info.value("o_payment_confirmed").toString();
    m_paymentConfirmed->setValue(convertDateTime(o_pc));
  } else {
    m_paymentConfirmed->setEnabled(false);
    m_paymentConfirmed->setVisible(false);
  }

  if (info.contains("o_payment_status")) {
    setPaymentStatus(info.value("o_payment_status").toInt());
  }
}
