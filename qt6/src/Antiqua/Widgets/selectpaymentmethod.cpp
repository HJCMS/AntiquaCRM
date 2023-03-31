// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectpaymentmethod.h"

#include <QIcon>

namespace AntiquaCRM {

SelectPaymentMethod::SelectPaymentMethod(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
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
}

void SelectPaymentMethod::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::Int));
  _f.setDefaultValue(0);
  setRestrictions(_f);

  int c = 1;
  m_edit->setWithoutDisclosures(AntiquaCRM::PAYMENT_NOT_SET);
  m_edit->insertItem(c++, tr("Bank transfer in advance"),
                     AntiquaCRM::BANK_PREPAYMENT);
  m_edit->insertItem(c++, tr("Delivery with open invoice"),
                     AntiquaCRM::DELIVER_WITH_INVOICE);
  m_edit->insertItem(c++, tr("Direct debit payment in advance"),
                     AntiquaCRM::DIRECT_DEBIT_PREPAYMENT);
  m_edit->insertItem(c++, tr("Credit card in advance"),
                     AntiquaCRM::CREDIT_CARD_PREPAYMENT);
  m_edit->insertItem(c++, tr("Cash on delivery payment"),
                     AntiquaCRM::CASH_ON_DELIVERY);
  m_edit->insertItem(c++, tr("PayPal prepayment"),
                     AntiquaCRM::PAYPAL_PREPAYMENT);
  m_edit->insertItem(c++, tr("Skrill prepayment"),
                     AntiquaCRM::SKRILL_PREPAYMENT);
  m_edit->insertItem(c++, tr("GiroPay prepayment"),
                     AntiquaCRM::GIROPAY_PREPAYMENT);
  m_edit->insertItem(c++, tr("GooglePay prepayment"),
                     AntiquaCRM::GOOGLEPAY_PREPAYMENT);
  m_edit->insertItem(c++, tr("Payment in advance from an unknown provider"),
                     AntiquaCRM::UNKNOWN_PREPAYMENT);
  m_edit->insertItem(c++, tr("Pickup - cash payment"),
                     AntiquaCRM::LOCAL_PICKUP_CASH_PAYMENT);
  m_edit->insertItem(c++, tr("Instant bank transfer"),
                     AntiquaCRM::INSTANT_BANK_TRANSFER);
  m_edit->insertItem(c++, tr("Open account - advance payment reserved"),
                     AntiquaCRM::INVOICE_PREPAYMENT_RESERVED);
  m_edit->insertItem(c++, tr("Check - payment in advance"),
                     AntiquaCRM::CHECK_PREPAYMENT);

  const QIcon _icon = AGuiUtils::getIcon("view-loan-asset");
  for (int i = 1; i < m_edit->count(); i++) {
    m_edit->setItemIcon(i, _icon);
  }

  setWindowModified(false);
}

void SelectPaymentMethod::setValue(const QVariant &value) {
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
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
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
