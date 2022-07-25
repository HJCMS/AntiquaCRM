// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryprints.h"
#include "messagebox.h"
#include "myicontheme.h"
#include "printseditor.h"
#include "printstable.h"

#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QList>
#include <QVBoxLayout>

PrintsSearchBar::PrintsSearchBar(QWidget *parent) : SearchBar{parent} {
  setObjectName("prints_search_bar");
  setValidation(SearchBar::Pattern);
  addSearchFilters(commonSearchFilter());
}

const QList<SearchFilter> PrintsSearchBar::commonSearchFilter() const {
  QList<SearchFilter> filter;
  SearchFilter a;
  a.setTitle(tr("Title or Author"));
  a.setFields("ip_title,ip_title_extended,ip_author");
  a.setType(SearchFilter::STRINGS);
  filter.append(a);
  a.setTitle(tr("Article ID"));
  a.setFields("ip_id");
  a.setType(SearchFilter::NUMERIC);
  filter.append(a);
  a.setTitle(tr("Author"));
  a.setFields("ip_author");
  a.setType(SearchFilter::STRINGS);
  filter.append(a);
  a.setTitle(tr("Technique"));
  a.setFields("ip_technique");
  a.setType(SearchFilter::STRINGS);
  filter.append(a);
  return filter;
}

InventoryPrints::InventoryPrints(QWidget *parent) : Inventory{parent} {
  setObjectName("InventoryPrints");
  setWindowTitle(tr("Prints") + " [*]");
  setClosable(true);

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

  m_searchBar = new PrintsSearchBar(this);
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
  m_editorWidget = new QScrollArea(m_stackedWidget);
  m_editorWidget->setObjectName("editor_scroll_area");
  m_editorWidget->setWidgetResizable(true);
  m_printsEditor = new PrintsEditor(m_editorWidget);
  m_printsEditor->setEnabled(false);
  m_editorWidget->setWidget(m_printsEditor);
  m_stackedWidget->insertWidget(1, m_editorWidget);
  // END Page#1

  layout->addWidget(m_stackedWidget);
  setLayout(layout);

  // Signals
  connect(this, SIGNAL(s_setSearchFocus()), m_searchBar, SLOT(clearAndFocus()));
  connect(this, SIGNAL(s_setSearchFilter()), m_searchBar,
          SLOT(setFilterFocus()));
  connect(this, SIGNAL(s_createNewEntry()), this, SLOT(createPrintsEntry()));
  connect(m_searchBar, SIGNAL(searchTextChanged(const QString &)), this,
          SLOT(searchConvert(const QString &)));
  connect(m_searchBar, SIGNAL(searchClicked()), this, SLOT(searchConvert()));
  connect(m_tableView, SIGNAL(s_articleSelected(int)), this,
          SLOT(articleSelected(int)));
  connect(m_tableView, SIGNAL(s_articleToOrders(int)), this,
          SIGNAL(s_addArticleOrder(int)));
  connect(m_statsPrintBar, SIGNAL(s_queryHistory(const QString &)), m_tableView,
          SLOT(queryHistory(const QString &)));
  connect(m_statsPrintBar, SIGNAL(s_refreshView()), m_tableView,
          SLOT(refreshView()));
  connect(m_tableView, SIGNAL(s_newEntryPlease()), this,
          SLOT(createPrintsEntry()));
  connect(m_tableView, SIGNAL(s_toClibboard(const QVariant &)), this,
          SLOT(copyIntoClipboard(const QVariant &)));
  connect(m_printsEditor, SIGNAL(s_postMessage(const QString &)), this,
          SLOT(displayMessageBox(const QString &)));
  connect(m_printsEditor, SIGNAL(s_leaveEditor()), this, SLOT(openTableView()));
  connect(m_printsEditor, SIGNAL(s_isModified(bool)), this,
          SLOT(setIsModified(bool)));
  connect(m_printsEditor, SIGNAL(s_articleCount(int, int)), this,
          SIGNAL(s_articleCount(int, int)));
  connect(m_printsEditor, SIGNAL(s_uploadImage(int)), this,
          SIGNAL(s_uploadImage(int)));
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
  int index = m_searchBar->currentFilterIndex();
  if (buf.length() >= 2) {
    SearchFilter s;
    s.setSearch(buf);
    QJsonObject js = m_searchBar->getSearchFilter(index);
    s.setFields(js.value("filter").toString().split(","));
    int i = js.value("type").toInt();
    s.setType((SearchFilter::SearchType)i);
    s.setTitle(js.value("title").toString());
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
    m_stackedWidget->setCurrentWidget(m_editorWidget);
  }
}

void InventoryPrints::createPrintsEntry() {
  m_printsEditor->createPrintsEntry();
  m_stackedWidget->setCurrentWidget(m_editorWidget);
}

void InventoryPrints::editPrintsEntry(int id) {
  if (id < 1)
    return;

  if (m_stackedWidget->currentIndex() != 0) {
    QMessageBox::information(
        this, tr("Editor"),
        tr("Cannot open the article because the tab is not in overview mode.<p>Please "
           "close all open prints, photo and stitches first.</p>"));
    return;
  }

  QString aid = QString::number(id);
  QString condition("ip_id=");
  condition.append(aid);
  openEditor(condition);
}

void InventoryPrints::articleSelected(int id) {
  if (id < 1)
    return;

  QString s("ip_id=");
  s.append(QString::number(id));
  openEditor(s);
}
