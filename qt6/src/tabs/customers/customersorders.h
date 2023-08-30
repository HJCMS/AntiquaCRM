// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_CUSTOMERSORDERS_H
#define ANTIQUACRM_PLUGIN_CUSTOMERSORDERS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>

/**
 * @class CustomersOrders
 * @brief Customers orders table ...
 * @ingroup _customers
 */
class ANTIQUACRM_LIBRARY CustomersOrders final : public QTableWidget {
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

#endif // ANTIQUACRM_PLUGIN_CUSTOMERSORDERS_H
