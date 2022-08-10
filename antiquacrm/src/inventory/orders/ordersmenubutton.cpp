// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordersmenubutton.h"
#include "myicontheme.h"
#include "orderstatements.h"

#include <QDebug>
#include <QMenu>

OrdersMenuButton::OrdersMenuButton(QWidget *parent) : QPushButton{parent} {
  setObjectName("orders_menu_button");
  setIcon(myIcon("identity"));
  setText(tr("Advanced Queries"));

  QIcon ico = myIcon("autostart");
  QMenu *m_menu = new QMenu(this);
  ac_default = m_menu->addAction(ico, tr("Default view"));
  ac_hasClosed = m_menu->addAction(ico, tr("Show completed orders."));
  ac_noPayment = m_menu->addAction(ico, tr("Show orders with missing payment."));
  ac_noDeliver = m_menu->addAction(ico, tr("Show orders without delivery."));
  setMenu(m_menu);

  connect(ac_default, SIGNAL(triggered()), this, SIGNAL(sendDefaultView()));
  connect(ac_hasClosed, SIGNAL(triggered()), this, SLOT(setQueryClosedOrders()));
  connect(ac_noPayment, SIGNAL(triggered()), this, SLOT(setQueryNoPayments()));
  connect(ac_noDeliver, SIGNAL(triggered()), this, SLOT(setQueryNoDelivery()));
}

void OrdersMenuButton::setQueryClosedOrders() {
  QString q("a.o_order_status=");
  q.append(QString::number(ORDER_STATUS_CLOSED));
  emit sendCustomQuery(q);
}

void OrdersMenuButton::setQueryNoPayments() {
  QString q("a.o_payment_status=false");
  emit sendCustomQuery(q);
}

void OrdersMenuButton::setQueryNoDelivery() {
  QString q("a.o_order_status<");
  q.append(QString::number(ORDER_STATUS_DELIVERED));
  emit sendCustomQuery(q);
}
