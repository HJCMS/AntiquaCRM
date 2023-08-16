// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_CUSTOMERSORDERS_H
#define ANTIQUACRM_CUSTOMERSORDERS_H

#include <QContextMenuEvent>
#include <QModelIndex>
#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>

/**
 * @brief Customers table widget
 * @ingroup _customers
 */
class CustomersOrders : public QTableWidget {
  Q_OBJECT

private:
  QModelIndex p_model;
  Qt::ItemFlags default_flags = (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
  void contextMenuEvent(QContextMenuEvent *) override;

private Q_SLOTS:
  void openOrder();
  void openInvoice();

Q_SIGNALS:
  void pushMessage(const QString &);

public Q_SLOTS:
  void restore();

public:
  explicit CustomersOrders(QWidget *parent = nullptr);
  QTableWidgetItem *paymentItem(const QVariant &data);
  QTableWidgetItem *numidItem(const QVariant &data);
  QTableWidgetItem *createItem(const QVariant &data);
  QTableWidgetItem *createDate(const QVariant &data);
};

#endif // CUSTOMERSORDERS_H
