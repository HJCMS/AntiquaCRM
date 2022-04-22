// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventorycustomers.h"
#include "searchbar.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets/QLayout>
#include <QtWidgets/QVBoxLayout>

InventoryCustomers::InventoryCustomers(int index, QTabWidget *parent)
    : QWidget{parent} {
  setObjectName("InventoryCustomers");
  setWindowTitle("TabCustomers");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("InventoryCustomersLayout");

  m_searchBar = new SearchBar(this);
  m_searchBar->setObjectName("InventoryCustomersSearchBar");
  layout->addWidget(m_searchBar);

  QWidget *w = new QWidget(this);
  layout->addWidget(w);

  setLayout(layout);
  // connect(, SIGNAL(s_isModified(bool)), this, SLOT(setClosable(bool)));
}
