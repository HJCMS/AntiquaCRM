// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabbooks.h"
#include "bookeditor.h"
#include "booksearchbar.h"
#include "bookstatusbar.h"
#include "booktableview.h"

#include <QDebug>
#include <QLayout>

TabBooks::TabBooks(QWidget *parent) : Inventory{parent} {
  setObjectName("inventory_books");
  setWindowTitle(tr("Books"));
  setWindowIcon(getTabIcon("view_log"));

  // Begin MainPage layout
  m_mainPage = new QWidget(this);
  QVBoxLayout *m_p1Layout = new QVBoxLayout(m_mainPage);
  m_p1Layout->setContentsMargins(0, 0, 0, 0);
  m_searchBar = new BookSearchBar(m_mainPage);
  m_p1Layout->addWidget(m_searchBar);
  m_table = new BookTableView(m_mainPage);
  m_table->setObjectName("books_table");
  m_p1Layout->addWidget(m_table);
  m_statusBar = new BookStatusBar(m_mainPage);
  m_p1Layout->addWidget(m_statusBar);
  m_mainPage->setLayout(m_p1Layout);
  insertWidget(0, m_mainPage);
  // End

  // Begin Editor
  m_editorPage = new BookEditor(this);
  insertWidget(1, m_editorPage);
  // End

  // Signals
  connect(this, SIGNAL(sendSetSearchFocus()), m_searchBar,
          SLOT(setSearchFocus()));

  connect(this, SIGNAL(sendSetSearchFilter()), m_searchBar,
          SLOT(setFilterFocus()));

  connect(m_table, SIGNAL(sendQueryReport(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  connect(m_table, SIGNAL(sendCopyToClibboard(const QString &)), this,
          SLOT(copyToClipboard(const QString &)));

  connect(m_table, SIGNAL(sendOpenEntry(qint64)), this,
          SLOT(openEntry(qint64)));

  connect(m_table, SIGNAL(sendArticleId(qint64)), this,
          SIGNAL(sendArticle2Order(qint64)));

  connect(m_table, SIGNAL(sendCreateNewEntry()), this, SLOT(createNewEntry()));

  connect(m_searchBar, SIGNAL(sendSearchClicked()), this,
          SLOT(createSearchQuery()));

  connect(m_statusBar, SIGNAL(sendHistoryQuery(const QString &)), this,
          SLOT(createSearchQuery(const QString &)));

  connect(m_statusBar, SIGNAL(sendReloadView()), m_table,
          SLOT(setReloadView()));
}

void TabBooks::createSearchQuery(const QString &query) {
  if (!query.isEmpty()) {
    m_table->setQuery(query);
    return;
  }

  QString w_sql = m_searchBar->getSearchStatement();
  if (m_searchBar->searchLength() > 1 && w_sql.length() > 1) {
    m_table->setQuery(w_sql);
  }
}

void TabBooks::createNewEntry() {
  if (m_editorPage->createNewEntry()) {
    setCurrentWidget(m_editorPage);
  }
}

void TabBooks::openEntry(qint64 articleId) {
  if (m_editorPage->openEditEntry(articleId)) {
    setCurrentWidget(m_editorPage);
  }
}

void TabBooks::onEnterChanged() {
  if (!initialed) {
    initialed = m_table->setQuery();
    m_searchBar->setFilter(0);
    setCurrentWidget(m_mainPage);
  }
}
