// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventorycostumers.h"
#include "costumertableview.h"
#include "applsettings.h"
#include "editcostumer.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets/QLayout>
#include <QtWidgets/QVBoxLayout>

InventoryCostumers::InventoryCostumers(QWidget *parent) : Inventory{parent} {
  setObjectName("InventoryCostumers");
  setWindowTitle("TabCostumers");

  ApplSettings cfg;
  minLength = cfg.value("search/startlength", 5).toInt();

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("inventory_books_layout");

  m_stackedWidget = new QStackedWidget(this);
  m_stackedWidget->setObjectName("books_stacked_widget");
  layout->addWidget(m_stackedWidget);

  // BEGIN Page#0
  QWidget *siteOneWidget = new QWidget(m_stackedWidget);
  siteOneWidget->setObjectName("costumer_site_one_widget");
  QVBoxLayout *siteOneLayout = new QVBoxLayout(siteOneWidget);
  siteOneLayout->setObjectName("costumer_site_one_layout");

  m_searchBar = new QWidget(this);
  m_searchBar->setObjectName("costumers_searchbar");
  siteOneLayout->addWidget(m_searchBar);

  m_tableView = new CostumerTableView(this);
  siteOneLayout->addWidget(m_tableView);
  m_tableView->queryStatement("TESTING");

  siteOneWidget->setLayout(siteOneLayout);
  m_stackedWidget->insertWidget(0, siteOneWidget);
  // END Page#0

  // BEGIN Page#1
  m_editCostumer = new EditCostumer(m_stackedWidget);
  m_editCostumer->setEnabled(false);
  m_stackedWidget->insertWidget(1, m_editCostumer);
  // END Page#1

  setLayout(layout);
  // connect(, SIGNAL(s_isModified(bool)), this, SLOT(setClosable(bool)));
}

void InventoryCostumers::openEditor(const QString &condition) {
  if (!condition.isEmpty()) {
    m_editCostumer->setEnabled(true);
    // m_editCostumer->(condition);
    m_stackedWidget->setCurrentWidget(m_editCostumer);
  }
}
