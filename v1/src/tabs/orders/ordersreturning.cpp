// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordersreturning.h"

#include <QDebug>
#include <QLabel>
#include <QLayout>

OrdersReturning::OrdersReturning(QWidget *parent) : QWidget{parent} {
  setObjectName("orders_returning_page");
  setContentsMargins(0, 0, 0, 0);
  QGridLayout *layout = new QGridLayout(this);
  layout->addWidget(new QLabel("todo", this), 0, 1, 1, 1);

  setLayout(layout);
}

void OrdersReturning::setReturnArticle(qint64 orderId) {
  qDebug() << Q_FUNC_INFO << orderId;
}
