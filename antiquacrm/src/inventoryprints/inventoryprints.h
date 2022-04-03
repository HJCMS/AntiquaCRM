// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORYPRINTS_H
#define INVENTORYPRINTS_H

#include "inventory.h"

#include <QtCore/QObject>
#include <QtWidgets/QTabWidget>

class SearchBar;

class InventoryPrints : public Inventory
{
  Q_OBJECT

private:
  SearchBar *m_searchBar;

public:
  explicit InventoryPrints(int index,QTabWidget *parent = nullptr);

};

#endif // INVENTORYPRINTS_H

