// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_ORDERSCUSTOMERINFO_H
#define ANTIQUACRM_ORDERSCUSTOMERINFO_H

#include <AntiquaWidgets>
#include <QObject>
#include <QStackedWidget>
#include <QWidget>

/**
 * @brief Orders Customer Info Widget
 * @ingroup _orders
 */
class OrdersCustomerInfo : public QWidget {
  Q_OBJECT

private:
  QStackedWidget *m_stackedWidget;
  QLabel *m_pageLabel;

  void setInfoText(int);

private Q_SLOTS:
  void swapPage();

public:
  SerialID *o_customer_id;
  TextField *c_postal_address;
  TextField *c_shipping_address;
  explicit OrdersCustomerInfo(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_ORDERSCUSTOMERINFO_H
