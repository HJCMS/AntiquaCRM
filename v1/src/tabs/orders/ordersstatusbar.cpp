// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordersstatusbar.h"
#include "ordershistorybutton.h"

OrdersStatusBar::OrdersStatusBar(QWidget *parent) : TabStatusBar{parent} {
  setObjectName("orders_status_bar");

  QHBoxLayout *layout = new QHBoxLayout(m_frame);
  layout->setContentsMargins(0, 0, 0, 0);

  btn_history = new OrdersHistoryButton(this);
  layout->addWidget(btn_history);

  m_frame->setLayout(layout);

  connect(btn_history, SIGNAL(sendDefaultView()), SIGNAL(sendDefaultView()));
  connect(btn_history, SIGNAL(sendHistoryAction(int)),
          SLOT(setHistoryAction(int)));
}

void OrdersStatusBar::setHistoryAction(int action) {
  QString sqlQuery;
  QString year("date_part('year',o_since)=date_part('year',CURRENT_DATE)");

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
    sqlQuery = "(o_payment_status=" + status;
    status = QString::number(AntiquaCRM::OrderPayment::ADMONISH);
    sqlQuery.append(" OR o_payment_status=" + status);
    status = QString::number(AntiquaCRM::OrderStatus::DELIVERED);
    sqlQuery.append(") AND o_order_status=" + status);
    break;

  case OrdersHistoryButton::HistoryQuery::FILTER_COMPLETED:
    status = QString::number(AntiquaCRM::OrderPayment::PAYED);
    sqlQuery = "o_payment_status=" + status;
    status = QString::number(AntiquaCRM::OrderStatus::DELIVERED);
    sqlQuery.append(" AND o_order_status=" + status);
    break;

  default:
    sqlQuery = "o_order_status=0 AND o_payment_status=0";
    break;
  };

  sqlQuery.append(" AND ");
  sqlQuery.append(year);

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << sqlQuery;
#endif

  emit sendHistoryQuery(sqlQuery);
}
