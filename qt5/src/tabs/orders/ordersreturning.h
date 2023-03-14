// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_ORDERS_RETURNING_H
#define ANTIQUACRM_ORDERS_RETURNING_H

#include <QObject>
#include <QWidget>

/**
 * @brief Inventory Orders returning widget
 * @ingroup _orders
 */
class OrdersReturning : public QWidget {
  Q_OBJECT

public Q_SLOTS:
  void setReturnArticle(qint64 orderId);

public:
  explicit OrdersReturning(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_ORDERS_RETURNING_H
