// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventorybooks.h"
#include "bookeditor.h"
#include "booksearchbar.h"
#include "bookstable.h"
#include "myicontheme.h"
#include <AntiquaCRM>

#include <QDebug>
#include <QHash>
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>

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

  m_searchBar = new BookSearchBar(this);
  siteOneLayout->addWidget(m_searchBar);

  m_tableView = new BooksTable(this);
  siteOneLayout->addWidget(m_tableView);

  m_statsBookBar = new StatsActionBar(this);
  siteOneLayout->addWidget(m_statsBookBar);
  m_stackedWidget->insertWidget(0, siteOneWidget);
  // END Page#0

  // BEGIN Page#1
  m_editorWidget = new QScrollArea(m_stackedWidget);
  m_editorWidget->setObjectName("editor_scroll_area");
  m_editorWidget->setWidgetResizable(true);
  m_bookEditor = new BookEditor(m_editorWidget);
  m_editorWidget->setWidget(m_bookEditor);
  m_stackedWidget->insertWidget(1, m_editorWidget);
  m_bookEditor->setEnabled(false);
  // END Page#1

  m_stackedWidget->setCurrentIndex(0);
  layout->addWidget(m_stackedWidget);

  setLayout(layout);

  // Signals
  connect(this, SIGNAL(s_setSearchFocus()), m_searchBar, SLOT(setSearchFocus()));

  connect(this, SIGNAL(s_setSearchFilter()), m_searchBar, SLOT(setFilterFocus()));

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
  if (m_searchBar->searchLength() <= minLength)
    return;

  searchConvert();
}

void InventoryBooks::searchConvert() {
  QString query = m_searchBar->getSearchStatement();
  if (query.length() < 1)
    return;

  if (m_tableView != nullptr)
    m_tableView->queryStatement(query);
}

void InventoryBooks::openTableView() {
  m_stackedWidget->setCurrentIndex(0);
  m_bookEditor->setEnabled(false);
}

void InventoryBooks::openEditor(const QString &condition) {
  if (!condition.isEmpty()) {
    m_bookEditor->setEnabled(true);
    m_bookEditor->editBookEntry(condition);
    m_stackedWidget->setCurrentWidget(m_editorWidget);
  }
}

void InventoryBooks::createBookEntry() {
  m_bookEditor->createBookEntry();
  m_stackedWidget->setCurrentWidget(m_editorWidget);
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
