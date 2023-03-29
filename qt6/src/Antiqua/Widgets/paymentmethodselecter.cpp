// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "paymentmethodselecter.h"

#include <QDebug>

namespace AntiquaCRM {

PaymentMethodSelecter::PaymentMethodSelecter(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void PaymentMethodSelecter::valueChanged(int index) {
  if (index < 1)
    return;

  int _mt = m_edit->itemData(index, Qt::UserRole).toInt();
  emit sendPaymentMethodChanged(static_cast<AntiquaCRM::PaymentMethod>(_mt));
}

void PaymentMethodSelecter::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::Int));
  _f.setDefaultValue(0);
  setRestrictions(_f);

  QMapIterator<AntiquaCRM::PaymentMethod, QString> it(getMethodes());
  int c = 0;
  while (it.hasNext()) {
    it.next();
    m_edit->insertItem(c++, it.value(), it.key());
  }

  setWindowModified(false);
}

void PaymentMethodSelecter::setValue(const QVariant &value) {
  if (value.metaType().id() != QMetaType::Int) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Reject:" << value;
#endif
    return;
  }

  AntiquaCRM::PaymentMethod _method =
      static_cast<AntiquaCRM::PaymentMethod>(value.toInt());
  int _index = m_edit->findData(_method, Qt::UserRole);
  if (_index > 0)
    m_edit->setCurrentIndex(_index);
}

void PaymentMethodSelecter::setFocus() {
  m_edit->setFocus();
  m_edit->showPopup();
}

void PaymentMethodSelecter::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void PaymentMethodSelecter::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void PaymentMethodSelecter::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void PaymentMethodSelecter::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool PaymentMethodSelecter::isValid() {
  if (isRequired() && m_edit->currentIndex() > 0)
    return false;

  return true;
}

const QVariant PaymentMethodSelecter::getValue() {
  int _index = m_edit->currentIndex();
  int _method = m_edit->itemData(_index, Qt::UserRole).toInt();
  return static_cast<AntiquaCRM::PaymentMethod>(_method);
}

const QString PaymentMethodSelecter::popUpHints() {
  return tr("a Valid payment method is required.");
}

const QString PaymentMethodSelecter::statusHints() { return popUpHints(); }

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

} // namespace AntiquaCRM
