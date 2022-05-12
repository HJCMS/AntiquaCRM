// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventorycostumers.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "costumertableview.h"
#include "editcostumer.h"
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

InventoryCostumers::InventoryCostumers(QWidget *parent) : Inventory{parent} {
  setObjectName("InventoryCostumers");
  setWindowTitle("TabCostumers");
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
  siteOneWidget->setObjectName("costumer_site_one_widget");
  QVBoxLayout *siteOneLayout = new QVBoxLayout(siteOneWidget);
  siteOneLayout->setObjectName("costumer_site_one_layout");

  m_searchBar = new SearchBar(this);
  m_searchBar->setObjectName("costumers_searchbar");
  m_searchBar->addSearchFilters(bookSearchFilter());
  siteOneLayout->addWidget(m_searchBar);

  m_tableView = new CostumerTableView(this);
  siteOneLayout->addWidget(m_tableView);

  m_statsActionBar = new StatsActionBar(this);
  m_statsActionBar->setObjectName("costumers_statusbar");
  siteOneLayout->addWidget(m_statsActionBar);

  siteOneWidget->setLayout(siteOneLayout);
  m_stackedWidget->insertWidget(0, siteOneWidget);
  // END Page#0

  // BEGIN Page#1
  m_editCostumer = new EditCostumer(m_stackedWidget);
  m_editCostumer->setEnabled(false);
  m_stackedWidget->insertWidget(1, m_editCostumer);
  // END Page#1

  setLayout(layout);

  connect(m_searchBar, SIGNAL(searchTextChanged(const QString &)), this,
          SLOT(searchConvert(const QString &)));

  connect(m_searchBar, SIGNAL(searchClicked()), this, SLOT(searchConvert()));

  connect(m_tableView, SIGNAL(s_updateCostumer(int)), this,
          SLOT(editCostumer(int)));

  connect(m_tableView, SIGNAL(s_insertCostumer()), this,
          SLOT(createCostumer()));

  connect(m_tableView, SIGNAL(s_createOrder(int)), this,
          SIGNAL(s_createOrder(int)));

  connect(m_tableView, SIGNAL(s_reportQuery(const QString &)), m_statsActionBar,
          SLOT(showMessage(const QString &)));

  connect(m_editCostumer, SIGNAL(s_leaveEditor()), this, SLOT(openTableView()));

  connect(m_statsActionBar, SIGNAL(s_queryHistory(const QString &)),
          m_tableView, SLOT(queryHistory(const QString &)));

  connect(m_statsActionBar, SIGNAL(s_refreshView()), m_tableView,
          SLOT(refreshView()));

  connect(m_editCostumer, SIGNAL(s_postMessage(const QString &)), this,
          SLOT(displayMessageBox(const QString &)));
  connect(m_editCostumer, SIGNAL(s_isModified(bool)), this,
          SLOT(setIsModified(bool)));
}

void InventoryCostumers::openEditor(const QString &costumer) {
  if (costumer.contains(primaryIndex)) {
    m_editCostumer->setEnabled(true);
    m_editCostumer->updateCostumer(costumer);
    m_stackedWidget->setCurrentWidget(m_editCostumer);
  }
}

void InventoryCostumers::editCostumer(int id) {
  if (id < 1)
    return;

  QString s(primaryIndex);
  s.append("=");
  s.append(QString::number(id));
  openEditor(s);
}

void InventoryCostumers::createCostumer() {
  m_editCostumer->setEnabled(true);
  m_editCostumer->createCostumer();
  m_stackedWidget->setCurrentWidget(m_editCostumer);
}

void InventoryCostumers::searchConvert(const QString &search) {
  if (search.length() <= minLength)
    return;

  searchConvert();
}

void InventoryCostumers::searchConvert() {
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

void InventoryCostumers::openTableView() {
  m_stackedWidget->setCurrentIndex(0);
  m_editCostumer->setEnabled(false);
}
