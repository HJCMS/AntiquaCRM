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
  connect(btn_history, SIGNAL(sendOrderStatusAction(int)),
          SLOT(setHistoryAction(int)));
  connect(btn_history, SIGNAL(sendOrderPaymentAction(int)),
          SLOT(setPaymentAction(int)));
}

void OrdersStatusBar::setHistoryAction(int index) {
  if (index < 0)
    return;

  QString query;
  QString str_index = QString::number(index);
  switch (static_cast<AntiquaCRM::OrderStatus>(index)) {
  case AntiquaCRM::OrderStatus::STARTED: /**< Auftrag angenommen */
    query = "o_order_status=" + str_index;
    break;

  case AntiquaCRM::OrderStatus::FETCHET: /**< Bereit zur Abholung */
    query = "o_order_status=" + str_index;
    break;

  case AntiquaCRM::OrderStatus::DELIVERED: /**< Ausgeliefert */
    query = "o_order_status=" + str_index;
    break;

  case AntiquaCRM::OrderStatus::REMINDET: /**< Erinnerung */
    query = "o_order_status=" + str_index;
    break;

  case AntiquaCRM::OrderStatus::COMPLETED: /**< Abgeschlossen */
    query = "o_order_status=" + str_index;
    break;

  case AntiquaCRM::OrderStatus::CANCELED: /**< Storniert */
    query = "o_order_status=" + str_index;
    break;

  case AntiquaCRM::OrderStatus::RETURNING: /**< Retour */
    query = "o_order_status=" + str_index;
    break;

  default:
    query = "o_order_status=0";
    break;
  };

  QString year("date_part('year',o_since)=date_part('year',CURRENT_DATE)");
  query.append(" AND " + year);
  emit sendHistoryQuery(query);
}

void OrdersStatusBar::setPaymentAction(int index) {
  if (index < 0)
    return;

  QString query;
  switch (static_cast<AntiquaCRM::OrderStatus>(index)) {
  case AntiquaCRM::OrderStatus::STARTED:
    query = "o_payment_status=false AND o_order_status>0";
    query.append(" AND o_order_status<6");
    break;

  default:
    query = "o_payment_status=false";
  };

  QString year("date_part('year',o_since)=date_part('year',CURRENT_DATE)");
  query.append(" AND " + year);
  emit sendHistoryQuery(query);
}
