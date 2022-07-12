// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "purchasepaymentinfo.h"
#include <QDebug>
#include <QtWidgets>

namespace Antiqua {

PaymentMethodSelect::PaymentMethodSelect(QWidget *parent)
    : QComboBox{parent}, p_typeSet(PaymentTypeSet()) {
  setInsertPolicy(QComboBox::NoInsert);
  setSizeAdjustPolicy(QComboBox::AdjustToContents);

  QMapIterator<int, QString> it(p_typeSet);
  while (it.hasNext()) {
    it.next();
    addItem(it.value(), it.key());
  }
}

int PaymentMethodSelect::findIndex(int id) {
  return findData(id, Qt::UserRole, Qt::MatchExactly);
}

int PaymentMethodSelect::findIndex(const QString &name) {
  return findData(name, Qt::DisplayRole, Qt::MatchStartsWith);
}

int PaymentMethodSelect::getPaymentMethod(const QString &name) {
  int index = findData(name, Qt::DisplayRole, Qt::MatchStartsWith);
  return itemData(index, Qt::UserRole).toInt();
}

const QString PaymentMethodSelect::getPaymentMethod(int id) {
  QMapIterator<int, QString> it(p_typeSet);
  while (it.hasNext()) {
    it.next();
    if (it.key() == id)
      return it.value();
  }
  return QString();
}

PLineRead::PLineRead(QWidget *parent) : QLineEdit{parent} {
  setReadOnly(true);
  setMinimumWidth(32);
}

PurchasePaymentInfo::PurchasePaymentInfo(QWidget *parent) : QWidget{parent} {
  // TODO
  int row = 0;
  QGridLayout *layout = new QGridLayout(this);
  layout->setObjectName("payment_info_layout");
  layout->setColumnStretch(3, 1);

  QLabel *info_type = new QLabel(tr("Payment method") + ":", this);
  layout->addWidget(info_type, row, 0, 1, 1, Qt::AlignRight);

  o_payment_method = new PaymentMethodSelect(this);
  layout->addWidget(o_payment_method, row, 1, 1, 1);

  m_paymentTransactionId = new PLineRead(this);
  m_paymentTransactionId->setObjectName("o_payment_paypal_txn_id");
  QString txn_info = tr("PayPal Transaction Id");
  m_paymentTransactionId->setPlaceholderText(txn_info);
  m_paymentTransactionId->setToolTip(txn_info);
  m_paymentTransactionId->setMinimumWidth(180);
  layout->addWidget(m_paymentTransactionId, row++, 2, 1, 1);

  QLabel *info_confirmed = new QLabel(this);
  info_confirmed->setText(tr("Payment Confirmed") + ":");
  layout->addWidget(info_confirmed, row, 0, 1, 1, Qt::AlignRight);

  m_paymentConfirmed = new PLineRead(this);
  m_paymentConfirmed->setObjectName("o_payment_confirmed");
  layout->addWidget(m_paymentConfirmed, row++, 1, 1, 1);

  QLabel *info_shipping = new QLabel(tr("Shipping costs") + ":", this);
  layout->addWidget(info_shipping, row, 0, 1, 1, Qt::AlignRight);

  m_deliveryCost = new PLineRead(this);
  m_deliveryCost->setObjectName("o_delivery_cost");
  m_deliveryCost->setPlaceholderText(tr("Shipping costs"));
  m_deliveryCost->setToolTip(tr("transportation costs"));
  layout->addWidget(m_deliveryCost, row++, 1, 1, 1, Qt::AlignLeft);

  layout->setRowStretch(row, 1);
  setLayout(layout);
}

void PurchasePaymentInfo::setData(const QString &objName,
                                  const QVariant &value) {
  if (objName == "o_payment_method") {
    int index = -1;
    if (value.type() != QVariant::String) {
      QString name = o_payment_method->getPaymentMethod(value.toInt());
      index = o_payment_method->findIndex(name);
    } else {
      int id = o_payment_method->getPaymentMethod(value.toString());
      index = o_payment_method->findIndex(id);
    }
    if (index > 0)
      o_payment_method->setCurrentIndex(index);

    return;
  }
  // qDebug() << "PayInfo:" << objName << value;
}

QMap<QString, QVariant> PurchasePaymentInfo::getAll() {
  QMap<QString, QVariant> map;
  QList<PLineRead *> list = findChildren<PLineRead *>(QString());
  for (int i = 0; i < list.count(); i++) {
    PLineRead *r = list[i];
    if (!r->text().isEmpty())
      map.insert(r->objectName(), r->text());
  }
  int index = o_payment_method->currentIndex();
  map.insert("o_payment_method",
             o_payment_method->itemData(index, Qt::UserRole));
  return map;
}

}; // namespace Antiqua
