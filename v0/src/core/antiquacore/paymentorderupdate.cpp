// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "paymentorderupdate.h"

namespace Antiqua {

PaymentOrderUpdate::PaymentOrderUpdate(QWidget *parent) : QComboBox{parent} {
  // ORDER_STATUS_NOT_SET = 0
  insertItem(ORDER_STATUS_NOT_SET, tr("Provider Status - no disclosures"),
             ORDER_STATUS_NOT_SET);
  // ORDER_WAIT_FOR_PAYMENT = 1
  insertItem(ORDER_WAIT_FOR_PAYMENT, tr("Wait for Order Payment"),
             ORDER_WAIT_FOR_PAYMENT);
  // ORDER_READY_FOR_SHIPMENT = 2
  insertItem(ORDER_READY_FOR_SHIPMENT, tr("Order is ready for Shipment"),
             ORDER_READY_FOR_SHIPMENT);
  // ORDER_SHIPPED_WAIT_FOR_PAYMENT = 3
  insertItem(ORDER_SHIPPED_WAIT_FOR_PAYMENT,
             tr("Order shipped, wait for Payment."),
             ORDER_SHIPPED_WAIT_FOR_PAYMENT);
  // ORDER_SHIPPED_AND_PAID = 4
  insertItem(ORDER_SHIPPED_AND_PAID, tr("Order Shipped and Paid"),
             ORDER_SHIPPED_AND_PAID);
  // ORDER_BUYER_NO_REACTION = 5
  insertItem(ORDER_BUYER_NO_REACTION, tr("Order Buyer no reaction."),
             ORDER_BUYER_NO_REACTION);
}

void PaymentOrderUpdate::overrideUserRoles(
    const QMap<Antiqua::PaymentStatus, QString> &map) {
  QMapIterator<Antiqua::PaymentStatus, QString> it(map);
  while (it.hasNext()) {
    it.next();
    setItemData(it.key(), it.value(), Qt::UserRole);
  }
}

}; // namespace Antiqua
