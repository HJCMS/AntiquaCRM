// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "purchasepaymentinfo.h"
#include "paymentorderupdate.h"
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
  layout->addWidget(m_paymentConfirmed, row, 1, 1, 1);

  m_updateAction = new PaymentOrderUpdate(this);
  m_updateAction->setObjectName("o_order_status_update");
  m_updateAction->setToolTip(tr("Update Order status"));
  layout->addWidget(m_updateAction, row++, 2, 1, 2);

  QLabel *info_shipping = new QLabel(tr("Shipping costs") + ":", this);
  layout->addWidget(info_shipping, row, 0, 1, 1, Qt::AlignRight);

  m_deliveryCost = new PLineRead(this);
  m_deliveryCost->setObjectName("o_delivery_cost");
  m_deliveryCost->setPlaceholderText(tr("Shipping costs"));
  m_deliveryCost->setToolTip(tr("transportation costs"));
  layout->addWidget(m_deliveryCost, row++, 1, 1, 1, Qt::AlignLeft);

  QLabel *info_iban = new QLabel(tr("IBAN") + ":", this);
  layout->addWidget(info_iban, row, 0, 1, 1, Qt::AlignRight);

  c_iban = new PLineRead(this);
  c_iban->setObjectName("c_iban");
  c_iban->setPlaceholderText(tr("IBAN"));
  c_iban->setToolTip(tr("IBAN"));
  layout->addWidget(c_iban, row++, 1, 1, 1);

  QLabel *info_swift = new QLabel(tr("SWIFT/BIC") + ":", this);
  layout->addWidget(info_swift, row, 0, 1, 1, Qt::AlignRight);

  c_swift_bic = new PLineRead(this);
  c_swift_bic->setObjectName("c_swift_bic");
  c_swift_bic->setPlaceholderText(tr("SWIFT/BIC"));
  c_swift_bic->setToolTip(tr("SWIFT/BIC"));
  layout->addWidget(c_swift_bic, row++, 1, 1, 1);

  QLabel *info_tax = new QLabel(tr("TAX Id") + ":", this);
  layout->addWidget(info_tax, row, 0, 1, 1, Qt::AlignRight);

  c_tax_id = new PLineRead(this);
  c_tax_id->setObjectName("c_tax_id");
  c_tax_id->setPlaceholderText(tr("Tax Id"));
  c_tax_id->setToolTip(tr("Tax Id"));
  layout->addWidget(c_tax_id, row++, 1, 1, 1);

  layout->setRowStretch(row, 1);
  setLayout(layout);

  connect(m_updateAction, SIGNAL(currentIndexChanged(int)), this,
          SLOT(updateOrderChanged(int)));
}

void PurchasePaymentInfo::updateOrderChanged(int index) {
  switch (index) {
  case (PaymentStatus::ORDER_WAIT_FOR_PAYMENT): {
    emit sendUpdateOrderStatus(Antiqua::PaymentStatus::ORDER_WAIT_FOR_PAYMENT);
    break;
  }
  case (PaymentStatus::ORDER_READY_FOR_SHIPMENT): {
    emit sendUpdateOrderStatus(
        Antiqua::PaymentStatus::ORDER_READY_FOR_SHIPMENT);
    break;
  }
  case (PaymentStatus::ORDER_SHIPPED_WAIT_FOR_PAYMENT): {
    emit sendUpdateOrderStatus(
        Antiqua::PaymentStatus::ORDER_SHIPPED_WAIT_FOR_PAYMENT);
    break;
  }
  case (PaymentStatus::ORDER_SHIPPED_AND_PAID): {
    emit sendUpdateOrderStatus(Antiqua::PaymentStatus::ORDER_SHIPPED_AND_PAID);
    break;
  }
  case (PaymentStatus::ORDER_BUYER_NO_REACTION): {
    emit sendUpdateOrderStatus(Antiqua::PaymentStatus::ORDER_BUYER_NO_REACTION);
    break;
  }
  default:
    qWarning("Unknown Antiqua::PaymentStatus::(%s)", qPrintable(QString::number(index)));
  }
}

void PurchasePaymentInfo::setOrderUpdateActions(
    const QMap<Antiqua::PaymentStatus, QString> &map) {
  m_updateAction->setParams(map);
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
