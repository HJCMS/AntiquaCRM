// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordershistorybutton.h"

#include <AntiquaCRM>
#include <QIcon>

OrdersHistoryButton::OrdersHistoryButton(QWidget *parent)
    : QPushButton{parent} {
  setObjectName("orders_history_button");
  setIcon(QIcon("://icons/user_identity.png"));
  setText(tr("Advanced Queries"));

  QIcon ico = QIcon("://icons/user_identity.png");
  m_menu = new QMenu(this);
  /** OrdersHistoryButton::FILTER_DEFAULT */
  ac_default = m_menu->addAction(ico, tr("Default view"));
  ac_default->setStatusTip(tr("Current orders in progress to delivered."));
  /** OrdersHistoryButton::FILTER_NOT_PAID */
  ac_noPayment = m_menu->addAction(ico, tr("Orders without payment."));
  ac_noPayment->setStatusTip(tr("Current orders without a payment."));
  /** OrdersHistoryButton::FILTER_DELIVERED_NOT_PAID */
  ac_delivered = m_menu->addAction(ico, tr("Delivered and not payed."));
  ac_delivered->setStatusTip(tr("Delivered and waiting for payment."));
  /** OrdersHistoryButton::FILTER_PAYMENT_REMINDED */
  ac_remindet =
      m_menu->addAction(ico, tr("Reminded, admonished - Payment Collection."));
  ac_remindet->setStatusTip(
      tr("Without payment with reminder or Payment Collection."));
  /** OrdersHistoryButton::FILTER_COMPLETED */
  ac_hasClosed = m_menu->addAction(ico, tr("Completed orders."));
  ac_hasClosed->setStatusTip(tr("Completed orders from this Year."));
  /** OrdersHistoryButton::FILTER_CANCELED */
  ac_canceled = m_menu->addAction(ico, tr("Canceled orders."));
  ac_canceled->setStatusTip(tr("Canceled orders from this Year."));
  setMenu(m_menu);

  connect(ac_default, SIGNAL(triggered()), SLOT(setDefaultView()));
  connect(ac_noPayment, SIGNAL(triggered()), SLOT(setQueryNoPayments()));
  connect(ac_delivered, SIGNAL(triggered()), SLOT(setQueryDelivered()));
  connect(ac_remindet, SIGNAL(triggered()), SLOT(setQueryRemindet()));
  connect(ac_hasClosed, SIGNAL(triggered()), SLOT(setQueryClosedOrders()));
  connect(ac_canceled, SIGNAL(triggered()), SLOT(setQueryCanceled()));
}

void OrdersHistoryButton::setDefaultView() {
  emit sendHistoryAction(OrdersHistoryButton::FILTER_DEFAULT);
}

void OrdersHistoryButton::setQueryNoPayments() {
  emit sendHistoryAction(OrdersHistoryButton::FILTER_NOT_PAID);
}

void OrdersHistoryButton::setQueryDelivered() {
  emit sendHistoryAction(OrdersHistoryButton::FILTER_DELIVERED_NOT_PAID);
}

void OrdersHistoryButton::setQueryRemindet() {
  emit sendHistoryAction(OrdersHistoryButton::FILTER_PAYMENT_REMINDED);
}

void OrdersHistoryButton::setQueryClosedOrders() {
  emit sendHistoryAction(OrdersHistoryButton::FILTER_COMPLETED);
}

void OrdersHistoryButton::setQueryCanceled() {
  emit sendHistoryAction(OrdersHistoryButton::FILTER_CANCELED);
}

const QMap<OrdersHistoryButton::HistoryQuery, QString>
OrdersHistoryButton::entries() {
  QMap<OrdersHistoryButton::HistoryQuery, QString> m;
  m.insert(OrdersHistoryButton::FILTER_DEFAULT, tr("Default view"));
  m.insert(OrdersHistoryButton::FILTER_NOT_PAID, tr("Orders without payment."));
  m.insert(OrdersHistoryButton::FILTER_DELIVERED_NOT_PAID,
           tr("Delivered and not payed."));
  m.insert(OrdersHistoryButton::FILTER_PAYMENT_REMINDED,
           tr("Reminded, admonished - Payment Collection."));
  m.insert(OrdersHistoryButton::FILTER_COMPLETED, tr("Completed orders."));
  m.insert(OrdersHistoryButton::FILTER_CANCELED, tr("Canceled orders."));
  return m;
}
