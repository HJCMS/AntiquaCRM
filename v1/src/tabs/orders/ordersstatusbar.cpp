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

  QString year("date_part('year',o_since)=date_part('year',CURRENT_DATE)");
  QString query("o_order_status=");
  query.append(QString::number(index) + " AND " + year);
  emit sendHistoryQuery(query);
}

void OrdersStatusBar::setPaymentAction(int index) {
  if (index < 0)
    return;

  QString year("date_part('year',o_since)=date_part('year',CURRENT_DATE)");
  QString query("o_payment_status=");
  query.append((index == 0) ? "false" : "true");
  query.append(" AND " + year);
  emit sendHistoryQuery(query);
}
