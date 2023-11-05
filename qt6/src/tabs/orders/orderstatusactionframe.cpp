// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderstatusactionframe.h"
#include "aenumerations.h"

#include <QLayout>

OrderStatusActionFrame::OrderStatusActionFrame(QWidget *parent)
    : QFrame{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  QString _info; // whats this text Qt::RichText

  // Auftragsstatus
  o_order_status = new AntiquaCRM::SelectOrderStatus(this);
  o_order_status->setObjectName("o_order_status");
  o_order_status->setBuddyLabel(tr("Order status"));
  o_order_status->setValue(AntiquaCRM::OrderStatus::OPEN);
  _info = tr("<b>Set order status for the current order.</b>"
             "<p>An order has different states:</p>"
             "<ul><li>Open - This order has currently no status!</li>"
             "<li>Created - The order process has been accepted.</li>"
             "<li>Fetchet - The order is packed and awaiting delivery.</li>"
             "<li>Delivery - The package is in transit.</li>"
             "<li>Delivered - The package has arrived at the recipient.</li>"
             "<li>Canceled - Will cancel the order.</li></ul>");
  o_order_status->setWhatsThisText(_info);
  layout->addWidget(o_order_status);

  // Zahlungsstatus
  o_payment_status = new AntiquaCRM::SelectOrderPayment(this);
  o_payment_status->setObjectName("o_payment_status");
  o_payment_status->setBuddyLabel(tr("Payment status"));
  o_payment_status->setValue(AntiquaCRM::OrderPayment::NOTPAID);
  _info = tr("<b>Sets the payment status for the current order.</b><br>"
             "<i>This selector only provides information about the various "
             "payment statuses of this order.</i><ul><li>Open - Waiting for "
             "payment.</li><li>Payed - The recipient has already paid for this "
             "order.</li><li>Reminded - Mark this order as, the recipient was "
             "reminded.</li><li>Admonished - Mark this order as, the recipient "
             "has been admonished.</li><li>Refunding - The order includes a "
             "refund.</li><li>Collection process - This order containing a "
             "collection transaction.</li></ul>");
  o_payment_status->setWhatsThisText(_info);
  layout->addWidget(o_payment_status);
  setLayout(layout);

  connect(o_order_status, SIGNAL(sendStatusChanged(AntiquaCRM::OrderStatus)),
          SLOT(orderStatusChanged(AntiquaCRM::OrderStatus)));

  connect(o_payment_status, SIGNAL(sendStatusChanged(AntiquaCRM::OrderPayment)),
          SLOT(orderPaymentChanged(AntiquaCRM::OrderPayment)));
}

void OrderStatusActionFrame::orderStatusChanged(AntiquaCRM::OrderStatus s) {
  QString _notify;
  if (s == AntiquaCRM::OrderStatus::OPEN)
    return;

  if (s == AntiquaCRM::OrderStatus::CANCELED) {
    _notify = tr("Payment reminder selected");
    switch (getOrderPayment()) {
    case (AntiquaCRM::OrderPayment::PAYED):
    case (AntiquaCRM::OrderPayment::REMIND):
    case (AntiquaCRM::OrderPayment::ADMONISH): {
      _notify = tr("<b>You cannot cancel this order!</b><p>"
                   "This order has already been paid.</p><p>"
                   "Please issue a refund or if delivery is not made, refund "
                   "the money and cancel later!</p>");
      emit sendNoticeMessage(_notify);
      o_order_status->setReject();
      return;
    } break;

    default:
      break;
    }
  }

  emit sendOrderStatus(s);
  if (_notify.length() > 3)
    emit sendNotifyStatus(_notify);
}

void OrderStatusActionFrame::orderPaymentChanged(AntiquaCRM::OrderPayment s) {
  QString _notify;
  switch (s) {
  case (AntiquaCRM::OrderPayment::PAYED):
  case (AntiquaCRM::OrderPayment::REMIND):
  case (AntiquaCRM::OrderPayment::ADMONISH):
  case (AntiquaCRM::OrderPayment::RETURN):
  case (AntiquaCRM::OrderPayment::COLLPROC): {
    if (getOrderStatus() == AntiquaCRM::OrderStatus::CANCELED) {
      _notify = tr("<b>You cannot set this Status.</b><p>"
                   "This order was already canceled!</p>");
      emit sendNoticeMessage(_notify);
      o_payment_status->setReject();
      return;
    }
  } break;

  default:
    break;
  }
  emit sendOrderPayment(s);
  if (_notify.length() > 3)
    emit sendNotifyStatus(_notify);
}

void OrderStatusActionFrame::setOrderStatus(AntiquaCRM::OrderStatus s) {
  o_order_status->setValue(s);
}

void OrderStatusActionFrame::setOrderPayment(AntiquaCRM::OrderPayment p) {
  o_payment_status->setValue(p);
}

void OrderStatusActionFrame::setStatusProtection(bool b) {
  o_payment_status->setReadOnly(b);
  o_order_status->setReadOnly(b);
}

bool OrderStatusActionFrame::currentOrderStatus() {
  return (getOrderPayment() == AntiquaCRM::OrderPayment::PAYED &&
          getOrderStatus() == AntiquaCRM::OrderStatus::DELIVERED);
}

AntiquaCRM::OrderStatus OrderStatusActionFrame::getOrderStatus() {
  int _id = o_order_status->getValue().toInt();
  return static_cast<AntiquaCRM::OrderStatus>(_id);
}

AntiquaCRM::OrderPayment OrderStatusActionFrame::getOrderPayment() {
  int _id = o_payment_status->getValue().toInt();
  return static_cast<AntiquaCRM::OrderPayment>(_id);
}
