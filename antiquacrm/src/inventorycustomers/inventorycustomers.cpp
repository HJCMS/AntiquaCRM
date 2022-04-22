// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventorycustomers.h"
#include "applsettings.h"
#include "editcustomer.h"
#include "searchbar.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets/QLayout>
#include <QtWidgets/QVBoxLayout>

InventoryCustomers::InventoryCustomers(QWidget *parent) : Inventory{parent} {
  setObjectName("InventoryCustomers");
  setWindowTitle("TabCustomers");

  ApplSettings cfg;
  minLength = cfg.value("search/startlength", 5).toInt();

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("inventory_books_layout");

  m_stackedWidget = new QStackedWidget(this);
  m_stackedWidget->setObjectName("books_stacked_widget");
  layout->addWidget(m_stackedWidget);

  // BEGIN Page#0
  QWidget *siteOneWidget = new QWidget(m_stackedWidget);
  siteOneWidget->setObjectName("customer_site_one_widget");
  QVBoxLayout *siteOneLayout = new QVBoxLayout(siteOneWidget);
  siteOneLayout->setObjectName("customer_site_one_layout");

  m_searchBar = new SearchBar(this);
  m_searchBar->setObjectName("customers_searchbar");
  siteOneLayout->addWidget(m_searchBar);

  m_tableView = new QTableView(this);
  siteOneLayout->addWidget(m_tableView);

  siteOneWidget->setLayout(siteOneLayout);
  m_stackedWidget->insertWidget(0, siteOneWidget);
  // END Page#0

  // BEGIN Page#1
  m_editCustomer = new EditCustomer(m_stackedWidget);
  m_stackedWidget->insertWidget(1, m_editCustomer);
  // m_editCustomer->setEnabled(false);
  // END Page#1

  m_stackedWidget->setCurrentIndex(1);

  setLayout(layout);
  // connect(, SIGNAL(s_isModified(bool)), this, SLOT(setClosable(bool)));
}

void InventoryCustomers::openEditor(const QString &) {

}
