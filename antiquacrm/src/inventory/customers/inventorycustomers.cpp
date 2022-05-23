// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventorycustomers.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "customertableview.h"
#include "editcustomer.h"
#include "myicontheme.h"
#include "searchbar.h"
#include "statsactionbar.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QLayout>
#include <QPushButton>
#include <QVBoxLayout>

/**
   @ref SearchBar::addSearchFilters
*/
static const QList<SearchBar::SearchFilter> bookSearchFilter() {
  SearchBar::SearchFilter a;
  QList<SearchBar::SearchFilter> filter;
  a.index = 0;
  a.title = QObject::tr("Sort by Name");
  a.filter = QString("shurename");
  filter.append(a);
  a.index = 1;
  a.title = QObject::tr("Sort by ID");
  a.filter = QString("id");
  filter.append(a);
  a.index = 2;
  a.title = QObject::tr("Sort by Date");
  a.filter = QString("since");
  filter.append(a);
  return filter;
}

InventoryCustomers::InventoryCustomers(QWidget *parent) : Inventory{parent} {
  setObjectName("InventoryCustomers");
  setWindowTitle(tr("Customers") + "[*]");
  setClosable(false);

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
  m_searchBar->addSearchFilters(bookSearchFilter());
  siteOneLayout->addWidget(m_searchBar);

  m_tableView = new CustomerTableView(this);
  siteOneLayout->addWidget(m_tableView);

  m_statsActionBar = new StatsActionBar(this);
  m_statsActionBar->setObjectName("customers_statusbar");
  siteOneLayout->addWidget(m_statsActionBar);

  siteOneWidget->setLayout(siteOneLayout);
  m_stackedWidget->insertWidget(0, siteOneWidget);
  // END Page#0

  // BEGIN Page#1
  m_editCustomer = new EditCustomer(m_stackedWidget);
  m_editCustomer->setEnabled(false);
  m_stackedWidget->insertWidget(1, m_editCustomer);
  // END Page#1

  setLayout(layout);

  connect(m_searchBar, SIGNAL(searchTextChanged(const QString &)), this,
          SLOT(searchConvert(const QString &)));

  connect(m_searchBar, SIGNAL(searchClicked()), this, SLOT(searchConvert()));

  connect(m_tableView, SIGNAL(s_updateCustomer(int)), this,
          SLOT(editCustomer(int)));

  connect(m_tableView, SIGNAL(s_insertCustomer()), this,
          SLOT(createCustomer()));

  connect(m_tableView, SIGNAL(s_createOrder(int)), this,
          SIGNAL(s_createOrder(int)));

  connect(m_tableView, SIGNAL(s_reportQuery(const QString &)), m_statsActionBar,
          SLOT(showMessage(const QString &)));

  connect(m_editCustomer, SIGNAL(s_leaveEditor()), this, SLOT(openTableView()));

  connect(m_statsActionBar, SIGNAL(s_queryHistory(const QString &)),
          m_tableView, SLOT(queryHistory(const QString &)));

  connect(m_statsActionBar, SIGNAL(s_refreshView()), m_tableView,
          SLOT(refreshView()));

  connect(m_editCustomer, SIGNAL(s_postMessage(const QString &)), this,
          SLOT(displayMessageBox(const QString &)));
  connect(m_editCustomer, SIGNAL(s_isModified(bool)), this,
          SLOT(setIsModified(bool)));
}

void InventoryCustomers::openEditor(const QString &customer) {
  if (customer.contains(primaryIndex)) {
    m_editCustomer->setEnabled(true);
    m_editCustomer->updateCustomer(customer);
    m_stackedWidget->setCurrentWidget(m_editCustomer);
  }
}

void InventoryCustomers::editCustomer(int id) {
  if (id < 1)
    return;

  QString s(primaryIndex);
  s.append("=");
  s.append(QString::number(id));
  openEditor(s);
}

void InventoryCustomers::createCustomer() {
  m_editCustomer->setEnabled(true);
  m_editCustomer->createCustomer();
  m_stackedWidget->setCurrentWidget(m_editCustomer);
}

void InventoryCustomers::searchConvert(const QString &search) {
  if (search.length() <= minLength)
    return;

  searchConvert();
}

void InventoryCustomers::searchConvert() {
  if (m_searchBar->currentSearchText().length() < 2)
    return;

  QString buf = m_searchBar->currentSearchText();
  if (buf.length() >= 2) {
    SearchStatement s;
    s.SearchField =
        m_searchBar->getSearchFilter(m_searchBar->currentFilterIndex());
    s.SearchString = buf;
    // qDebug("'%s':'%s'", qPrintable(s.SearchField),
    // qPrintable(s.SearchString));
    if (m_tableView != nullptr)
      m_tableView->queryStatement(s);
  }
}

void InventoryCustomers::openTableView() {
  m_stackedWidget->setCurrentIndex(0);
  m_editCustomer->setEnabled(false);
}
