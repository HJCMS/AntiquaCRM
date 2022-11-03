// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CUSTOMERSORDERS_H
#define ANTIQUACRM_CUSTOMERSORDERS_H

#include <QObject>
#include <QTableWidget>
#include <QTableWidgetItem>

class CustomersOrders : public QTableWidget {
  Q_OBJECT

public Q_SLOTS:
  void restore();

public:
  explicit CustomersOrders(QWidget *parent = nullptr);
  QTableWidgetItem* numidItem(const QVariant &data);
  QTableWidgetItem* createItem(const QVariant &data);
  QTableWidgetItem* createDate(const QVariant &data);
};

#endif // CUSTOMERSORDERS_H
