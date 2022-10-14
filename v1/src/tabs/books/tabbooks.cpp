// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabbooks.h"
#include "booksearchbar.h"
#include "bookstable.h"

#include <QDebug>
#include <QLayout>

TabBooks::TabBooks(QWidget *parent) : Inventory{parent} {
  setObjectName("inventory_books");
  setWindowTitle(tr("Books"));
  setClosable(false);

  // Begin MainPage layout
  m_mainPage = new QWidget(this);
  QVBoxLayout *m_p1Layout = new QVBoxLayout(m_mainPage);
  m_p1Layout->setContentsMargins(0, 0, 0, 0);
  m_searchBar = new BookSearchBar(m_mainPage);
  m_p1Layout->addWidget(m_searchBar);
  m_table = new BooksTable(m_mainPage);
  m_p1Layout->addWidget(m_table);
  m_mainPage->setLayout(m_p1Layout);
  insertWidget(0, m_mainPage);
  // End

  // Signals
  connect(this, SIGNAL(sendSetSearchFocus()), m_searchBar,
          SLOT(setSearchFocus()));

  connect(this, SIGNAL(sendSetSearchFilter()), m_searchBar,
          SLOT(setFilterFocus()));

  connect(m_searchBar, SIGNAL(sendSearchClicked()), this,
          SLOT(createSearchQuery()));

  connect(this, SIGNAL(sendCreateNewEntry()), this, SLOT(createNewEntry()));

}

void TabBooks::createSearchQuery() {
  QString sql = m_searchBar->getSearchStatement();
  if (m_searchBar->searchLength() > 1 && sql.length() > 1) {
    qDebug() << Q_FUNC_INFO << sql;
  }
}

void TabBooks::createNewEntry() { qDebug() << Q_FUNC_INFO << "TODO"; }

void TabBooks::openEntry(qint64 id) { qDebug() << Q_FUNC_INFO << id; }

void TabBooks::onEnterChanged() {
  if (!initialed) {
    initialed = m_table->initTable();
    m_searchBar->setFilter(0);
  }
}
