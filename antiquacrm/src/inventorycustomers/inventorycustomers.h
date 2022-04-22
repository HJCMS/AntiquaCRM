// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORYCUSTOMERS_H
#define INVENTORYCUSTOMERS_H

#include "inventory.h"

#include <QtCore/QObject>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

class SearchBar;

class InventoryCustomers : public Inventory {
  Q_OBJECT

private:
  SearchBar *m_searchBar;

private Q_SLOTS:
  // TODO

public:
  explicit InventoryCustomers(QWidget *parent = nullptr);
};

#endif // INVENTORYCUSTOMERS_H
