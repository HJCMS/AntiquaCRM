// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordersstatusbar.h"
#include "ordershistorybutton.h"

OrdersStatusBar::OrdersStatusBar(QWidget *parent) : TabStatusBar{parent} {
  setObjectName("orders_status_bar");
  defaultViewButton();
  btn_history = new OrdersHistoryButton(this);
  addButton(btn_history);
  connect(btn_history, SIGNAL(sendDefaultView()), SIGNAL(sendDefaultView()));
  connect(btn_history, SIGNAL(sendHistoryAction(int)),
          SLOT(setHistoryAction(int)));
}

void OrdersStatusBar::setHistoryAction(int action) {
  QString sqlQuery;
  QString status;
  switch (static_cast<OrdersHistoryButton::HistoryQuery>(action)) {
  // Zeige alle Nicht bezahlten
  case OrdersHistoryButton::HistoryQuery::FILTER_DEFAULT:
    status = QString::number(AntiquaCRM::OrderStatus::DELIVERED);
    sqlQuery = "o_order_status<" + status;
    break;

  // Zeige "nicht Bezahlt" und "nicht Storniert"
  case OrdersHistoryButton::HistoryQuery::FILTER_NOT_PAID:
    status = QString::number(AntiquaCRM::OrderPayment::PAYED);
    sqlQuery = "o_payment_status!=" + status;
    status = QString::number(AntiquaCRM::OrderStatus::CANCELED);
    sqlQuery.append(" AND o_order_status !=" + status);
    break;

  // Zeige geliefert und Nicht bezahlt
  case OrdersHistoryButton::HistoryQuery::FILTER_DELIVERED_NOT_PAID:
    status = QString::number(AntiquaCRM::OrderPayment::PAYED);
    sqlQuery = "o_payment_status!=" + status;
    status = QString::number(AntiquaCRM::OrderStatus::DELIVERED);
    sqlQuery.append(" AND o_order_status=" + status);
    break;

  // Zeige geliefert mit Zahlungserinnerung oder Mahnung
  case OrdersHistoryButton::HistoryQuery::FILTER_PAYMENT_REMINDED:
    status = QString::number(AntiquaCRM::OrderPayment::REMIND);
    sqlQuery = "o_payment_status IN (" + status;
    status = QString::number(AntiquaCRM::OrderPayment::ADMONISH);
    sqlQuery.append("," + status);
    status = QString::number(AntiquaCRM::OrderPayment::COLLPROC);
    sqlQuery.append("," + status + ")");
    status = QString::number(AntiquaCRM::OrderStatus::DELIVERED);
    sqlQuery.append(" AND o_order_status=" + status);
    break;

  case OrdersHistoryButton::HistoryQuery::FILTER_COMPLETED:
    status = QString::number(AntiquaCRM::OrderPayment::PAYED);
    sqlQuery = "o_payment_status=" + status;
    status = QString::number(AntiquaCRM::OrderStatus::DELIVERED);
    sqlQuery.append(" AND o_order_status=" + status);
    sqlQuery.append(" AND ");
    sqlQuery.append(searchYear());
    break;

  case OrdersHistoryButton::HistoryQuery::FILTER_CANCELED:
    status = QString::number(AntiquaCRM::OrderStatus::CANCELED);
    sqlQuery.append("o_order_status=" + status);
    sqlQuery.append(" AND ");
    sqlQuery.append(searchYear());
    break;

  default:
    sqlQuery = "o_order_status=0 AND o_payment_status=0";
    break;
  };
  // qDebug() << Q_FUNC_INFO << sqlQuery;
  emit sendHistoryQuery(sqlQuery);
}

const QString OrdersStatusBar::searchYear(int y) {
  QString year = QString::number(y);
  return QString("date_part('year',o_since)=" + year);
}
