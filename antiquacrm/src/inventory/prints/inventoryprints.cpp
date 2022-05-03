// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryprints.h"
#include "applsettings.h"
#include "messagebox.h"
#include "printseditor.h"
#include "printstable.h"
#include "searchbar.h"
#include "statsactionbar.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <QDebug>
#include <QList>
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>

/**
   @brief bookSearchFilter
   Muss für jedes Suchtab einzeln definiert sein.
   @ref SearchBar::addSearchFilters
   @return const QList<SearchFilter>
*/
static const QList<SearchBar::SearchFilter> bookSearchFilter() {
  SearchBar::SearchFilter a;
  QList<SearchBar::SearchFilter> filter;
  a.index = 0;
  a.title = QObject::tr("Title");
  a.filter = QString("");
  filter.append(a);
  a.index = 1;
  a.title = QObject::tr("Title (starts with)");
  a.filter = QString("title_first");
  filter.append(a);
  a.index = 2;
  a.title = QObject::tr("Article ID");
  a.filter = QString("id");
  filter.append(a);
  a.index = 3;
  a.title = QObject::tr("Author");
  a.filter = QString("author");
  filter.append(a);
  a.index = 4;
  a.title = QObject::tr("Current inventory");
  a.filter = QString("count");
  filter.append(a);
  return filter;
}

InventoryPrints::InventoryPrints(QWidget *parent)
    : Inventory{parent} {
  setObjectName("InventoryPrints");
  setWindowTitle("TabPrints");

  ApplSettings cfg;
  minLength = cfg.value("search/startlength", 5).toInt();

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("InventoryPrintsLayout");

  m_stackedWidget = new QStackedWidget(this);
  m_stackedWidget->setObjectName("prints_stacked_widget");

  // BEGIN Page#0
  QWidget *siteOneWidget = new QWidget(m_stackedWidget);
  siteOneWidget->setObjectName("prints_site_one_widget");
  QVBoxLayout *siteOneLayout = new QVBoxLayout(siteOneWidget);
  siteOneLayout->setObjectName("prints_site_one_layout");

  m_searchBar = new SearchBar(this);
  m_searchBar->setValidation(SearchBar::Pattern);
  m_searchBar->addSearchFilters(bookSearchFilter());
  siteOneLayout->addWidget(m_searchBar);

  m_tableView = new PrintsTable(this);
  siteOneLayout->addWidget(m_tableView);

  m_statsPrintBar = new StatsActionBar(this);
  m_statsPrintBar->setObjectName("stats_action_prints");
  siteOneLayout->addWidget(m_statsPrintBar);

  siteOneWidget->setLayout(siteOneLayout);
  m_stackedWidget->insertWidget(0, siteOneWidget);
  // END Page#0

  // BEGIN Page#1
  m_printsEditor = new PrintsEditor(m_stackedWidget);
  m_printsEditor->setEnabled(false);
  m_stackedWidget->insertWidget(1, m_printsEditor);
  // END Page#1

  layout->addWidget(m_stackedWidget);
  setLayout(layout);

  // Signals
  connect(m_searchBar, SIGNAL(searchTextChanged(const QString &)), this,
          SLOT(searchConvert(const QString &)));

  connect(m_searchBar, SIGNAL(searchClicked()), this, SLOT(searchConvert()));

  connect(m_tableView, SIGNAL(s_articleSelected(int)), this,
          SLOT(articleSelected(int)));

  connect(m_statsPrintBar, SIGNAL(s_queryHistory(const QString &)), m_tableView,
          SLOT(queryHistory(const QString &)));

  connect(m_statsPrintBar, SIGNAL(s_refreshView()), m_tableView,
          SLOT(refreshView()));

  connect(m_tableView, SIGNAL(s_newEntryPlease()), this,
          SLOT(createPrintsEntry()));

  connect(m_searchBar, SIGNAL(currentFilterChanged(int)), this,
          SLOT(updateValidator(int)));

  connect(m_printsEditor, SIGNAL(s_postMessage(const QString &)), this,
          SLOT(displayMessageBox(const QString &)));

  connect(m_printsEditor, SIGNAL(s_leaveEditor()), this, SLOT(openTableView()));
  connect(m_printsEditor, SIGNAL(s_isModified(bool)), this,
          SLOT(setClosable(bool)));

//  connect(m_printsEditor, SIGNAL(s_articleActivation(bool)), this,
//          SLOT(articleActivation(bool)));
}

void InventoryPrints::searchConvert(const QString &query) {
  if (query.length() <= minLength)
    return;

  searchConvert();
}

void InventoryPrints::searchConvert() {
  if (m_searchBar->currentSearchText().length() < 2)
    return;

  QString buf = m_searchBar->currentSearchText();
  if (buf.length() >= 2) {
    SearchStatement s;
    s.SearchField =
        m_searchBar->getSearchFilter(m_searchBar->currentFilterIndex());
    s.SearchString = buf;
    qDebug("QueryStatement:'%s':'%s'", qPrintable(s.SearchField),
           qPrintable(s.SearchString));

    if (m_tableView != nullptr)
      m_tableView->queryStatement(s);
  }
}

void InventoryPrints::openTableView() {
  m_stackedWidget->setCurrentIndex(0);
  m_printsEditor->setEnabled(false);
}

void InventoryPrints::openEditor(const QString &condition) {
  if (!condition.isEmpty()) {
    m_printsEditor->setEnabled(true);
    m_printsEditor->editPrintsEntry(condition);
    m_stackedWidget->setCurrentWidget(m_printsEditor);
  }
}

void InventoryPrints::createPrintsEntry() {
  m_printsEditor->createPrintsEntry();
  m_stackedWidget->setCurrentWidget(m_printsEditor);
}

void InventoryPrints::articleSelected(int id) {
  if (id < 1)
    return;

  QString s("ip_id=");
  s.append(QString::number(id));
  openEditor(s);
}

void InventoryPrints::updateValidator(int id) {
  switch (id) {
  case 2: /**< Artikel ID */
  case 4: /**< Im Bestand */
    m_searchBar->setValidation(SearchBar::Number);
    break;

  default: /**< Zeichenketten */
    m_searchBar->setValidation(SearchBar::Pattern);
    break;
  };
}
