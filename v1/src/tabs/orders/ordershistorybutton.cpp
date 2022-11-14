// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordershistorybutton.h"

#include <AntiquaCRM>
#include <QIcon>
#include <QMenu>

OrdersHistoryButton::OrdersHistoryButton(QWidget *parent)
    : QPushButton{parent} {
  setObjectName("orders_history_button");
  setIcon(QIcon("://icons/user_identity.png"));
  setText(tr("Advanced Queries"));

  QIcon ico = QIcon("://icons/user_identity.png");
  QMenu *m_menu = new QMenu(this);
  ac_default = m_menu->addAction(ico, tr("Default view"));
  ac_hasClosed = m_menu->addAction(ico, tr("Show completed orders."));
  ac_noPayment =
      m_menu->addAction(ico, tr("Show orders with missing payment."));
  ac_noDeliver = m_menu->addAction(ico, tr("Show orders without delivery."));
  setMenu(m_menu);

  connect(ac_default, SIGNAL(triggered()), SIGNAL(sendDefaultView()));
  connect(ac_hasClosed, SIGNAL(triggered()), SLOT(setQueryClosedOrders()));
  connect(ac_noPayment, SIGNAL(triggered()), SLOT(setQueryNoPayments()));
  connect(ac_noDeliver, SIGNAL(triggered()), SLOT(setQueryNoDelivery()));
}

void OrdersHistoryButton::setQueryClosedOrders() {
  emit sendOrderStatusAction(AntiquaCRM::OrderStatus::COMPLETED);
}

void OrdersHistoryButton::setQueryNoPayments() {
  emit sendOrderPaymentAction(AntiquaCRM::PaymentStatus::WAIT_FOR_PAYMENT);
}

void OrdersHistoryButton::setQueryNoDelivery() {
  emit sendOrderStatusAction(AntiquaCRM::OrderStatus::DELIVERED);
}
