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
  ac_hasClosed = m_menu->addAction(ico, tr("Show completed orders."));
  ac_noPayment =
      m_menu->addAction(ico, tr("Show orders with missing payment."));
  ac_noDeliver = m_menu->addAction(ico, tr("Show orders without delivery."));
  setMenu(m_menu);

  connect(ac_hasClosed, SIGNAL(triggerd()), this, SLOT());
  connect(ac_noPayment, SIGNAL(triggerd()), this, SLOT());
  connect(ac_noDeliver, SIGNAL(triggerd()), this, SLOT());
}

void OrdersMenuButton::setQueryClosedOrders() {
  QString q("");
  qDebug() << advancedOrdersQuery(q);
}

void OrdersMenuButton::setQueryNoPayments() {
  QString q("");
  qDebug() << advancedOrdersQuery(q);
}

void OrdersMenuButton::setQueryNoDelivery() {
  QString q("");
  qDebug() << advancedOrdersQuery(q);
}
