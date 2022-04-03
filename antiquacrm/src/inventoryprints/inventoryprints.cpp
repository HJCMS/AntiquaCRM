// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryprints.h"
#include "searchbar.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QVBoxLayout>

InventoryPrints::InventoryPrints(int index,QTabWidget *parent) : Inventory{index,parent} {
  setObjectName("InventoryPrints");
  setWindowTitle("TabPrints");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("InventoryPrintsLayout");

  m_searchBar = new SearchBar(this);
  m_searchBar->setObjectName("InventoryPrintsSearchBar");
  layout->addWidget(m_searchBar);

  QWidget *w = new QWidget(this);
  layout->addWidget(w);

  setLayout(layout);
}
