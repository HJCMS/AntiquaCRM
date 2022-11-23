// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "paymentmethodselecter.h"

#include <QDebug>

PaymentMethodSelecter::PaymentMethodSelecter(QWidget *parent)
    : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_layout->addWidget(m_box);
  loadDataset();
  setModified(false);
  setRequired(false);
  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(dataChanged(int)));
}

void PaymentMethodSelecter::loadDataset() {
  QMapIterator<AntiquaCRM::PaymentMethod, QString> it(getMethodes());
  int c = 0;
  while (it.hasNext()) {
    it.next();
    m_box->insertItem(c++, it.value(), it.key());
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
  AntiquaCRM::PaymentMethod status =
      static_cast<AntiquaCRM::PaymentMethod>(val.toInt());
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

const QMap<AntiquaCRM::PaymentMethod, QString>
PaymentMethodSelecter::getMethodes() {
  QMap<AntiquaCRM::PaymentMethod, QString> map;
  map.insert(AntiquaCRM::PAYMENT_NOT_SET, tr("No selection available"));
  map.insert(AntiquaCRM::BANK_PREPAYMENT, tr("Bank transfer in advance"));
  map.insert(AntiquaCRM::DELIVER_WITH_INVOICE,
             tr("Delivery with open invoice"));
  map.insert(AntiquaCRM::DIRECT_DEBIT_PREPAYMENT,
             tr("Direct debit payment in advance"));
  map.insert(AntiquaCRM::CREDIT_CARD_PREPAYMENT, tr("Credit card in advance"));
  map.insert(AntiquaCRM::CASH_ON_DELIVERY, tr("Cash on delivery payment"));
  map.insert(AntiquaCRM::PAYPAL_PREPAYMENT, tr("PayPal prepayment"));
  map.insert(AntiquaCRM::SKRILL_PREPAYMENT, tr("Skrill prepayment"));
  map.insert(AntiquaCRM::GIROPAY_PREPAYMENT, tr("GiroPay prepayment"));
  map.insert(AntiquaCRM::GOOGLEPAY_PREPAYMENT, tr("GooglePay prepayment"));
  map.insert(AntiquaCRM::UNKNOWN_PREPAYMENT,
             tr("Payment in advance from an unknown provider"));
  map.insert(AntiquaCRM::LOCAL_PICKUP_CASH_PAYMENT,
             tr("Pickup - cash payment"));
  map.insert(AntiquaCRM::INSTANT_BANK_TRANSFER, tr("Instant bank transfer"));
  map.insert(AntiquaCRM::INVOICE_PREPAYMENT_RESERVED,
             tr("Open account - advance payment reserved"));
  map.insert(AntiquaCRM::CHECK_PREPAYMENT, tr("Check - payment in advance"));
  return map;
}
