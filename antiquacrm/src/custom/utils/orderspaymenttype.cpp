// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderspaymenttype.h"

#include <QDebug>
#include <QHBoxLayout>

OrdersPaymentTypes::OrdersPaymentTypes() {
  insert(0, QObject::tr("Without disclosures"));
  /** @brief  1: Banküberweisung (Vorkasse) */
  insert(1, QObject::tr("Bank transfer (prepayment)"));
  /** @brief  2: Offene Rechnung */
  insert(2, QObject::tr("open account"));
  /** @brief  3: Lastschrift (Vorkasse) */
  insert(3, QObject::tr("direct debit (prepayment)"));
  /** @brief  4: Kreditkarte (Vorkasse) */
  insert(4, QObject::tr("Credit card (prepayment)"));
  /** @brief  5: Nachnahme */
  insert(5, QObject::tr("cash on delivery"));
  /** @brief  6: PayPal (Vorkasse) */
  insert(6, QObject::tr("PayPal (prepayment)"));
  /** @brief  7: OFFEN - Nicht gesetzt */
  insert(7, QObject::tr("Unknown"));
  /** @brief  8: Skrill (Vorkasse) */
  insert(8, QObject::tr("Skrill (prepayment)"));
  /** @brief  9: Selbstabholung - Barzahlung */
  insert(9, QObject::tr("Pickup - cash payment"));
  /** @brief  10: Sofortüberweisung */
  insert(10, QObject::tr("Instant bank transfer"));
  /** @brief  11: Offene Rechnung (Vorkasse vorbehalten) */
  insert(11, QObject::tr("Invoice (prepayment reserved)"));
  /** @brief  12: Scheck */
  insert(12, QObject::tr("Check"));
};

OrdersPaymentTypeSelect::OrdersPaymentTypeSelect(QWidget *parent)
    : QComboBox{parent} {

  OrdersPaymentTypes items;
  if (items.size() > 0) {
    QMapIterator<int, QString> it(items);
    while (it.hasNext()) {
      addItem(it.value(), it.key());
    }
  }
}

OrdersPaymentType::OrdersPaymentType(QWidget *parent) : UtilsMain{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_box = new OrdersPaymentTypeSelect(this);
  layout->addWidget(m_box);

  setLayout(layout);
  setRequired(true);
  setModified(false);
  connect(m_box, SIGNAL(currentIndexChanged(int)), this,
          SLOT(typeChanged(int)));
}

void OrdersPaymentType::typeChanged(int id) { setModified(true); }

void OrdersPaymentType::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void OrdersPaymentType::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void OrdersPaymentType::setValue(const QVariant &val) {
  // TODO String
  int index = m_box->findData(val.toInt(), Qt::UserRole);
  if (index > 0) {
    m_box->setCurrentIndex(index);
    setModified(true);
  }
}

const QVariant OrdersPaymentType::value() {
  int index = m_box->currentIndex();
  return m_box->itemData(index, Qt::UserRole);
}

bool OrdersPaymentType::isValid() {
  if (isRequired() && m_box->currentIndex() == 0)
    return false;

  return true;
}

void OrdersPaymentType::setInfo(const QString &info) {
  m_box->setToolTip(info);
}

const QString OrdersPaymentType::info() { return m_box->toolTip(); }

const QString OrdersPaymentType::notes() {
  return tr("a Valid Payment type is required!");
}
