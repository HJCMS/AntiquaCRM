// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef RETURN_EDIT_H
#define RETURN_EDIT_H

#include <AntiquaWidgets>
#include <QLabel>
#include <QObject>
#include <QWidget>

class ReturnEdit : public QWidget {
  Q_OBJECT

public:
  SerialID *o_id;
  SerialID *o_invoice_id;
  SerialID *o_customer_id;
  DeliveryId *o_delivery;
  TextField *c_postal_address;

  explicit ReturnEdit(QWidget *parent = nullptr);
};

#endif // RETURN_EDIT_H
