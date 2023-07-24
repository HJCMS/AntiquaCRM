// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "salesinfo.h"

SalesInfo::SalesInfo(QWidget *parent) : QWidget{parent} {
  setContentsMargins(5, 0, 5, 0);

  int _row = 0;
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 2, 0, 2);
  layout->setColumnStretch(1, 1);

  QLabel *lb = new QLabel(this);
  lb->setText(tr("Displays the current payment and order status Information."));
  lb->setStyleSheet("QLabel {font-weight: bold;}");
  layout->addWidget(lb, _row++, 0, 1, 2, Qt::AlignLeft);

  layout->addWidget(infoCell(tr("Ordered at")), _row, 0, 1, 1);
  m_since = new AntiquaCRM::DateTimeInfo(this);
  layout->addWidget(m_since, _row++, 1, 1, 1);

  layout->addWidget(infoCell(tr("Payment Method")), _row, 0, 1, 1);
  m_method = new AntiquaCRM::SelectPaymentMethod(this);
  m_method->setReadOnly(true);
  layout->addWidget(m_method, _row++, 1, 1, 1);

  layout->addWidget(infoCell(tr("Paid with PayPal")), _row, 0, 1, 1);
  m_orderPayed = new QLabel(this);
  m_orderPayed->setText(tr("Without disclosures"));
  layout->addWidget(m_orderPayed, _row++, 1, 1, 1);

  layout->addWidget(infoCell(tr("Payment Confirmed")), _row, 0, 1, 1);
  m_confirmed = new AntiquaCRM::DateTimeInfo(this);
  m_confirmed->setObjectName("o_payment_confirmed");
  layout->addWidget(m_confirmed, _row++, 1, 1, 1);

  layout->setRowStretch(_row++, 1);
  setLayout(layout);
}

QLabel *SalesInfo::infoCell(const QString &str) {
  QLabel *m = new QLabel(this);
  m->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  m->setText(str + ":");
  return m;
}

const QDateTime SalesInfo::convertDateTime(const QString &str) {
  QDateTime _dt = QDateTime::fromString(str, Qt::ISODate);
  _dt.setTimeSpec(Qt::OffsetFromUTC);
  return _dt;
}

void SalesInfo::setPaymentMethod(int i) {
  AntiquaCRM::PaymentMethod e = static_cast<AntiquaCRM::PaymentMethod>(i);
  m_method->setValue(e);
}

void SalesInfo::setInfo(const QJsonObject &object) {
  if (object.isEmpty())
    return;

  if (object.contains("o_since")) {
    QString o_s = object.value("o_since").toString();
    m_since->setValue(convertDateTime(o_s));
  }

  if (object.contains("o_payment_method")) {
    setPaymentMethod(object.value("o_payment_method").toInt());
  }

  if (object.contains("o_payment_paypal_txn_id")) {
    QString _oppti = object.value("o_payment_paypal_txn_id").toString();
    m_orderPayed->setText(_oppti);
  }

  if (object.contains("o_payment_confirmed")) {
    QString _opc = object.value("o_payment_confirmed").toString();
    m_confirmed->setValue(convertDateTime(_opc));
  } else {
    m_confirmed->setEnabled(false);
    m_confirmed->setVisible(false);
  }
}
