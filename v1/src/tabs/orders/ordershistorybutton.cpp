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
  ac_noPayment = m_menu->addAction(ico, tr("Orders without payment."));
  ac_delivered = m_menu->addAction(ico, tr("Delivered and not completed."));
  ac_remindet = m_menu->addAction(ico, tr("Reminder orders."));
  ac_hasClosed = m_menu->addAction(ico, tr("Completed orders."));
  setMenu(m_menu);

  connect(ac_default, SIGNAL(triggered()), SIGNAL(sendDefaultView()));
  connect(ac_hasClosed, SIGNAL(triggered()), SLOT(setQueryClosedOrders()));
  connect(ac_noPayment, SIGNAL(triggered()), SLOT(setQueryNoPayments()));
  connect(ac_delivered, SIGNAL(triggered()), SLOT(setQueryDelivered()));
  connect(ac_remindet, SIGNAL(triggered()), SLOT(setQueryRemindet()));
}

void OrdersHistoryButton::setQueryClosedOrders() {
  emit sendOrderStatusAction(AntiquaCRM::OrderStatus::COMPLETED);
}

void OrdersHistoryButton::setQueryNoPayments() {
  emit sendOrderPaymentAction(AntiquaCRM::OrderStatus::STARTED);
}

void OrdersHistoryButton::setQueryDelivered() {
  emit sendOrderStatusAction(AntiquaCRM::OrderStatus::DELIVERY);
}

void OrdersHistoryButton::setQueryRemindet() {
  emit sendOrderStatusAction(AntiquaCRM::OrderStatus::REMINDET);
}
