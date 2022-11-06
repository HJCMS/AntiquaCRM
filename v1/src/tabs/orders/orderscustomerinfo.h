// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSCUSTOMERINFO_H
#define ANTIQUACRM_ORDERSCUSTOMERINFO_H

#include <AntiquaWidgets>
#include <QObject>
#include <QStackedWidget>
#include <QWidget>

class OrdersCustomerInfo : public QWidget {
  Q_OBJECT

private:
  QStackedWidget *m_stackedWidget;
  QLabel *m_pageLabel;

private Q_SLOTS:
  void back();
  void forward();

public:
  SerialID *o_customer_id;
  TextField *c_postal_address;
  TextField *c_shipping_address;
  explicit OrdersCustomerInfo(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_ORDERSCUSTOMERINFO_H
