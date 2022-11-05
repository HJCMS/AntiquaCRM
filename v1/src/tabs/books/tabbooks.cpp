// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabbooks.h"
#include "bookeditor.h"
#include "booksearchbar.h"
#include "bookstatusbar.h"
#include "booktableview.h"

#include <QDebug>
#include <QLayout>
#include <QMessageBox>

TabBooks::TabBooks(QWidget *parent) : Inventory{"books_tab", parent} {
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
  m_editorPage = new QScrollArea(this);
  m_editorPage->setObjectName("book_editor_scrollarea");
  m_editorPage->setWidgetResizable(true);
  m_editorWidget = new BookEditor(m_editorPage);
  m_editorPage->setWidget(m_editorWidget);
  insertWidget(1, m_editorPage);
  // End

  // Signals
  connect(this, SIGNAL(sendSetSearchFocus()), m_searchBar,
          SLOT(setSearchFocus()));

  connect(this, SIGNAL(sendSetSearchFilter()), m_searchBar,
          SLOT(setFilterFocus()));

  // maintable
  connect(m_table, SIGNAL(sendQueryReport(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  connect(m_table, SIGNAL(sendCopyToClibboard(const QString &)),
          SLOT(copyToClipboard(const QString &)));

  connect(m_table, SIGNAL(sendOpenEntry(qint64)), SLOT(openEntry(qint64)));

  connect(m_table, SIGNAL(sendCurrentId(qint64)),
          SIGNAL(sendIdToOrder(qint64)));

  connect(m_table, SIGNAL(sendCreateNewEntry()), SLOT(createNewEntry()));

  connect(m_table, SIGNAL(sendResultExists(bool)), m_statusBar,
          SLOT(setCreateButtonEnabled(bool)));

  // editor
  connect(m_editorWidget, SIGNAL(sendLeaveEditor()), SLOT(openStartPage()));

  // searchbar
  connect(m_searchBar, SIGNAL(sendSearchClicked()), SLOT(createSearchQuery()));

  // statusbar
  connect(m_statusBar, SIGNAL(sendCreateEntry()), SLOT(createNewEntry()));
  connect(m_statusBar, SIGNAL(sendHistoryQuery(const QString &)),
          SLOT(createSearchQuery(const QString &)));

  connect(m_statusBar, SIGNAL(sendReloadView()), m_table,
          SLOT(setReloadView()));
}

void TabBooks::openStartPage() {
  m_editorPage->setEnabled(false);
  if (m_table->rowCount() > 0 && m_table->rowCount() < 20)
    m_table->setReloadView();

  setCurrentIndex(0);
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
  if (m_editorWidget->createNewEntry()) {
    m_editorPage->setEnabled(true);
    setCurrentWidget(m_editorPage);
  }
}

void TabBooks::openEntry(qint64 articleId) {
  if (articleId < 1)
    return;

  if (currentIndex() != 0) {
    QString info(tr("Cannot open this article.") + "<br>");
    info.append(tr("Because the book tab is not in overview mode."));
    info.append("<p>");
    info.append(tr("Please save and close all open book entries first."));
    info.append("</p>");
    QMessageBox::information(this, tr("Bookeditor"), info);
    return;
  }

  if (m_editorWidget->openEditEntry(articleId)) {
    m_editorPage->setEnabled(true);
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
