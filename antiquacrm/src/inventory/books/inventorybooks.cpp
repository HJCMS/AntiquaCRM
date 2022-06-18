// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventorybooks.h"
#include "bookeditor.h"
#include "bookstable.h"
#include "myicontheme.h"
#include <AntiquaCRM>

#include <QDebug>
#include <QHash>
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>

BookSearchBar::BookSearchBar(QWidget *parent) : SearchBar{parent} {
  setObjectName("book_search_bar");
  setValidation(SearchBar::Pattern);
  addSearchFilters(commonSearchFilter());
}

const QList<SearchFilter> BookSearchBar::commonSearchFilter() const {
  SearchFilter a;
  QList<SearchFilter> filter;
  a.setTitle(tr("Book Title or Author"));
  a.setFields("ib_title,ib_title_extended,ib_author");
  a.setType(SearchFilter::STRINGS);
  filter.append(a);
  a.setTitle(tr("Book Title only"));
  a.setFields("ib_title,ib_title_extended");
  a.setType(SearchFilter::STRINGS);
  filter.append(a);
  a.setTitle(tr("Article ID"));
  a.setFields("ib_id");
  a.setType(SearchFilter::NUMERIC);
  filter.append(a);
  a.setTitle(tr("ISBN"));
  a.setFields("ib_isbn");
  a.setType(SearchFilter::NUMERIC);
  filter.append(a);
  a.setTitle(tr("Author"));
  a.setFields("ib_author");
  a.setType(SearchFilter::STRINGS);
  filter.append(a);
  a.setTitle(tr("Publisher"));
  a.setFields("ib_publisher");
  a.setType(SearchFilter::STRINGS);
  filter.append(a);
  a.setTitle(tr("Duration by Keyword"));
  a.setFields("storage_id");
  a.setType(SearchFilter::REFERENCES);
  filter.append(a);
  return filter;
}

InventoryBooks::InventoryBooks(QWidget *parent) : Inventory{parent} {
  setObjectName("InventoryBooks");
  setWindowTitle(tr("Books") + "[*]");
  setClosable(false);

  ApplSettings cfg;
  minLength = cfg.value("search/startlength", 5).toInt();

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("inventory_books_layout");

  m_stackedWidget = new QStackedWidget(this);
  m_stackedWidget->setObjectName("books_stacked_widget");

  // BEGIN Page#0
  QWidget *siteOneWidget = new QWidget(m_stackedWidget);
  siteOneWidget->setObjectName("books_site_one_widget");
  QVBoxLayout *siteOneLayout = new QVBoxLayout(siteOneWidget);
  siteOneLayout->setObjectName("books_site_one_layout");

  /**
    @brief m_searchBar
    Siehe auch @ref updateValidator(int)!
  */
  m_searchBar = new BookSearchBar(this);
  siteOneLayout->addWidget(m_searchBar);

  m_tableView = new BooksTable(this);
  siteOneLayout->addWidget(m_tableView);

  m_statsBookBar = new StatsActionBar(this);
  siteOneLayout->addWidget(m_statsBookBar);
  m_stackedWidget->insertWidget(0, siteOneWidget);
  // END Page#0

  // BEGIN Page#1
  m_bookEditor = new BookEditor(m_stackedWidget);
  m_stackedWidget->insertWidget(1, m_bookEditor);
  m_bookEditor->setEnabled(false);
  // END Page#1

  m_stackedWidget->setCurrentIndex(0);
  layout->addWidget(m_stackedWidget);

  setLayout(layout);

  // Signals
  connect(this, SIGNAL(s_setSearchFocus()), m_searchBar, SLOT(clearAndFocus()));

  connect(this, SIGNAL(s_setSearchFilter()), m_searchBar,
          SLOT(setFilterFocus()));

  connect(this, SIGNAL(s_createNewEntry()), this, SLOT(createBookEntry()));

  connect(m_searchBar, SIGNAL(searchTextChanged(const QString &)), this,
          SLOT(searchConvert(const QString &)));

  connect(m_searchBar, SIGNAL(searchClicked()), this, SLOT(searchConvert()));

  connect(m_statsBookBar, SIGNAL(s_queryHistory(const QString &)), m_tableView,
          SLOT(queryHistory(const QString &)));

  connect(m_statsBookBar, SIGNAL(s_refreshView()), m_tableView,
          SLOT(refreshView()));

  connect(m_tableView, SIGNAL(s_articleSelected(int)), this,
          SLOT(articleSelected(int)));

  connect(m_tableView, SIGNAL(s_articleToOrders(int)), this,
          SIGNAL(s_addArticleOrder(int)));

  connect(m_tableView, SIGNAL(s_toClibboard(const QVariant &)), this,
          SLOT(copyIntoClipboard(const QVariant &)));

  connect(m_tableView, SIGNAL(s_reportQuery(const QString &)), m_statsBookBar,
          SLOT(showMessage(const QString &)));

  connect(m_tableView, SIGNAL(s_newEntryPlease()), this,
          SLOT(createBookEntry()));

  connect(m_bookEditor, SIGNAL(s_postMessage(const QString &)), this,
          SLOT(displayMessageBox(const QString &)));

  connect(m_bookEditor, SIGNAL(s_leaveEditor()), this, SLOT(openTableView()));
  connect(m_bookEditor, SIGNAL(s_isModified(bool)), this,
          SLOT(setIsModified(bool)));
  connect(m_bookEditor, SIGNAL(s_articleCount(int, int)), this,
          SIGNAL(s_articleCount(int, int)));
}

void InventoryBooks::searchConvert(const QString &query) {
  if (query.length() <= minLength)
    return;

  QString info(tr("Automatic search disabled in this mode. Press enter to "
                  "start the search!"));

  int filter = m_searchBar->currentFilterIndex();
  switch (filter) {
  case 0: {
    m_statsBookBar->showMessage(info);
    return;
  }
  case 6: {
    m_statsBookBar->showMessage(info);
    return;
  }
  default:
    break;
  };

  searchConvert();
}

void InventoryBooks::searchConvert() {
  if (m_searchBar->currentSearchText().length() < 2)
    return;

  QString buf = m_searchBar->currentSearchText();
  int index = m_searchBar->currentFilterIndex();
  if (buf.length() > 1) {
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

void InventoryBooks::openTableView() {
  m_stackedWidget->setCurrentIndex(0);
  m_bookEditor->setEnabled(false);
}

void InventoryBooks::openEditor(const QString &condition) {
  if (!condition.isEmpty()) {
    m_bookEditor->setEnabled(true);
    m_bookEditor->editBookEntry(condition);
    m_stackedWidget->setCurrentWidget(m_bookEditor);
  }
}

void InventoryBooks::createBookEntry() {
  m_bookEditor->createBookEntry();
  m_stackedWidget->setCurrentWidget(m_bookEditor);
}

void InventoryBooks::editBookEntry(int id) {
  QString condition("ib_id=");
  QString ib_id = QString::number(id);
  if (!ib_id.isEmpty()) {
    condition.append(ib_id);
    openEditor(condition);
  }
}

void InventoryBooks::articleSelected(int id) {
  if (id < 1)
    return;

  QString s("ib_id=");
  s.append(QString::number(id));
  openEditor(s);
}
