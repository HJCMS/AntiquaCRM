// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "paymentmethodselecter.h"

#include <QDebug>

PaymentMethodSelecter::PaymentMethodSelecter(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_layout->addWidget(m_box);
  loadDataset();
  setModified(false);
  setRequired(false);
  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(dataChanged(int)));
}

void PaymentMethodSelecter::loadDataset() {
  int c = 0;
  p_map.clear();
  p_map.insert(c++, QPair(tr("No selection available"),
                AntiquaCRM::PAYMENT_NOT_SET));
  p_map.insert(c++, QPair(tr("Bank transfer in advance"),
                AntiquaCRM::BANK_PREPAYMENT));
  p_map.insert(c++, QPair(tr("Delivery with open invoice"),
                AntiquaCRM::DELIVER_WITH_INVOICE));
  p_map.insert(c++, QPair(tr("Direct debit payment in advance"),
                AntiquaCRM::DIRECT_DEBIT_PREPAYMENT));
  p_map.insert(c++, QPair(tr("Credit card in advance"),
                AntiquaCRM::CREDIT_CARD_PREPAYMENT));
  p_map.insert(c++, QPair(tr("Cash on delivery payment"),
                AntiquaCRM::CASH_ON_DELIVERY));
  p_map.insert(c++, QPair(tr("PayPal prepayment"),
                AntiquaCRM::PAYPAL_PREPAYMENT));
  p_map.insert(c++, QPair(tr("Skrill prepayment"),
                AntiquaCRM::SKRILL_PREPAYMENT));
  p_map.insert(c++, QPair(tr("GiroPay prepayment"),
                AntiquaCRM::GIROPAY_PREPAYMENT));
  p_map.insert(c++, QPair(tr("GooglePay prepayment"),
                AntiquaCRM::GOOGLEPAY_PREPAYMENT));
  p_map.insert(c++, QPair(tr("Payment in advance from an unknown provider"),
                AntiquaCRM::UNKNOWN_PREPAYMENT));
  p_map.insert(c++, QPair(tr("Pickup - cash payment"),
                AntiquaCRM::LOCAL_PICKUP_CASH_PAYMENT));
  p_map.insert(c++, QPair(tr("Instant bank transfer"),
                AntiquaCRM::INSTANT_BANK_TRANSFER));
  p_map.insert(c++, QPair(tr("Open account - advance payment reserved"),
                AntiquaCRM::INVOICE_PREPAYMENT_RESERVED));
  p_map.insert(c++, QPair(tr("Check - payment in advance"),
                AntiquaCRM::CHECK_PREPAYMENT));
  for (int i = 0; i < p_map.size(); i++) {
    QPair<QString, AntiquaCRM::PaymentMethod> st = p_map[i];
    m_box->insertItem(i, st.first, st.second);
  }
}

void PaymentMethodSelecter::dataChanged(int index) {
  if (index > 0)
    setModified(true);

  int status = m_box->itemData(index, Qt::UserRole).toInt();
  AntiquaCRM::PaymentMethod st = static_cast<AntiquaCRM::PaymentMethod>(status);
  emit sendPaymentMethodChanged(st);
}

void PaymentMethodSelecter::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void PaymentMethodSelecter::setValue(const QVariant &val) {
  AntiquaCRM::PaymentMethod status = static_cast<AntiquaCRM::PaymentMethod>(val.toInt());
  int index = m_box->findData(status, Qt::UserRole);
  if (index >= 0)
    m_box->setCurrentIndex(index);
}

void PaymentMethodSelecter::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void PaymentMethodSelecter::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant PaymentMethodSelecter::value() {
  int index = m_box->currentIndex();
  int status = m_box->itemData(index, Qt::UserRole).toInt();
  return static_cast<AntiquaCRM::PaymentMethod>(status);
}

bool PaymentMethodSelecter::isValid() {
  if (isRequired() && m_box->currentIndex() > 0)
    return false;

  return true;
}

void PaymentMethodSelecter::setInfo(const QString &info) {
  m_box->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString PaymentMethodSelecter::info() { return m_box->toolTip(); }

const QString PaymentMethodSelecter::notes() {
  return tr("a Valid payment method is required.");
}
