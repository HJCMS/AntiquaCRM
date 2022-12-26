// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "returnedit.h"

#include <QLayout>

ReturnEdit::ReturnEdit(QWidget *parent) : QWidget{parent} {
  int row = 0;
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(1, 1, 1, 1);

  o_id = new SerialID(this);
  o_id->setObjectName("o_id");
  o_id->setInfo(tr("Order Id"));
  layout->addWidget(o_id, row, 0, 1, 1);

  o_invoice_id = new SerialID(this);
  o_invoice_id->setObjectName("o_invoice_id");
  o_invoice_id->setInfo(tr("Invoice Id"));
  layout->addWidget(o_invoice_id, row, 1, 1, 1);

  o_customer_id = new SerialID(this);
  o_customer_id->setObjectName("o_customer_id");
  o_customer_id->setInfo(tr("Customer Id"));
  layout->addWidget(o_customer_id, row, 2, 1, 1);

  o_delivery = new DeliveryId(this);
  o_delivery->setObjectName("o_delivery");
  o_delivery->setInfo(tr("Delivery note"));
  layout->addWidget(o_delivery, row++, 3, 1, 1);

  c_postal_address = new TextField(this);
  c_postal_address->setObjectName("c_postal_address");
  c_postal_address->setInfo(tr("Address"));
  c_postal_address->setEditable(false);
  layout->addWidget(c_postal_address, row++, 0, 1, 4);

  layout->setRowStretch(row++, 1);
  setLayout(layout);
}
