// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectpaymentmethod.h"
#include "antiquaicon.h"

#include <QIcon>

namespace AntiquaCRM {

SelectPaymentMethod::SelectPaymentMethod(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void SelectPaymentMethod::valueChanged(int index) {
  if (index < 1)
    return;

  int _mt = m_edit->itemData(index, Qt::UserRole).toInt();
  emit sendPaymentMethodChanged(static_cast<AntiquaCRM::PaymentMethod>(_mt));
  emit sendInputChanged();
}

void SelectPaymentMethod::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setDefaultValue(AntiquaCRM::PaymentMethod::PAYMENT_NOT_SET);
  setRestrictions(_f);

  int c = 1;
  m_edit->setWithoutDisclosures(AntiquaCRM::PaymentMethod::PAYMENT_NOT_SET);
  m_edit->insertItem(c++, tr("Bank transfer in advance"),
                     AntiquaCRM::PaymentMethod::BANK_PREPAYMENT);
  m_edit->insertItem(c++, tr("Delivery with open invoice"),
                     AntiquaCRM::PaymentMethod::DELIVER_WITH_INVOICE);
  m_edit->insertItem(c++, tr("Direct debit payment in advance"),
                     AntiquaCRM::PaymentMethod::DIRECT_DEBIT_PREPAYMENT);
  m_edit->insertItem(c++, tr("Credit card in advance"),
                     AntiquaCRM::PaymentMethod::CREDIT_CARD_PREPAYMENT);
  m_edit->insertItem(c++, tr("Cash on delivery payment"),
                     AntiquaCRM::PaymentMethod::CASH_ON_DELIVERY);
  m_edit->insertItem(c++, tr("PayPal prepayment"),
                     AntiquaCRM::PaymentMethod::PAYPAL_PREPAYMENT);
  m_edit->insertItem(c++, tr("Skrill prepayment"),
                     AntiquaCRM::PaymentMethod::SKRILL_PREPAYMENT);
  m_edit->insertItem(c++, tr("GiroPay prepayment"),
                     AntiquaCRM::PaymentMethod::GIROPAY_PREPAYMENT);
  m_edit->insertItem(c++, tr("GooglePay prepayment"),
                     AntiquaCRM::PaymentMethod::GOOGLEPAY_PREPAYMENT);
  m_edit->insertItem(c++, tr("Payment in advance from an unknown provider"),
                     AntiquaCRM::PaymentMethod::UNKNOWN_PREPAYMENT);
  m_edit->insertItem(c++, tr("Pickup - cash payment"),
                     AntiquaCRM::PaymentMethod::LOCAL_PICKUP_CASH_PAYMENT);
  m_edit->insertItem(c++, tr("Instant bank transfer"),
                     AntiquaCRM::PaymentMethod::INSTANT_BANK_TRANSFER);
  m_edit->insertItem(c++, tr("Open account - advance payment reserved"),
                     AntiquaCRM::PaymentMethod::INVOICE_PREPAYMENT_RESERVED);
  m_edit->insertItem(c++, tr("Check - payment in advance"),
                     AntiquaCRM::PaymentMethod::CHECK_PREPAYMENT);

  const QIcon _icon = AntiquaCRM::antiquaIcon("view-loan-asset");
  for (int i = 1; i < m_edit->count(); i++) {
    m_edit->setItemIcon(i, _icon);
  }

  setWindowModified(false);
}

void SelectPaymentMethod::setValue(const QVariant &value) {
  if (value.metaType().id() != QMetaType::Int) {
#ifdef ANTIQUA_DEVELOPMENT
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

void SelectPaymentMethod::setFocus() {
  m_edit->setFocus();
  m_edit->showPopup();
}

void SelectPaymentMethod::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectPaymentMethod::setReadOnly(bool b) { m_edit->setEnabled(!b); }

void SelectPaymentMethod::setRestrictions(const QSqlField &field) {
  setRequired((field.requiredStatus() == QSqlField::Required));
}

void SelectPaymentMethod::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void SelectPaymentMethod::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SelectPaymentMethod::isValid() {
  if (isRequired() && m_edit->currentIndex() > 0)
    return false;

  return true;
}

const QMetaType SelectPaymentMethod::getType() const {
  return QMetaType(QMetaType::Int);
}

const QVariant SelectPaymentMethod::getValue() {
  int _index = m_edit->currentIndex();
  int _method = m_edit->itemData(_index, Qt::UserRole).toInt();
  return static_cast<AntiquaCRM::PaymentMethod>(_method);
}

const QString SelectPaymentMethod::popUpHints() {
  return tr("a Valid payment method is required.");
}

const QString SelectPaymentMethod::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
