// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_INVENTORYTABLE_H
#define ANTIQUACRM_INVENTORYTABLE_H

#include <QObject>
#include <QTableView>
#include <QHeaderView>
#include <QWidget>

class InventoryTable : public QTableView {
  Q_OBJECT

public Q_SLOTS:
  virtual void refreshTable() = 0;

public:
  explicit InventoryTable(QWidget *parent = nullptr);
  virtual bool initTable() = 0;
};

#endif // ANTIQUACRM_INVENTORYTABLE_H
