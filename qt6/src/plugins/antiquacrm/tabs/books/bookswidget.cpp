// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookswidget.h"
#include "booksconfig.h"
#include "bookseditor.h"
#include "bookssearchbar.h"
#include "booksstatusbar.h"
#include "bookstableview.h"

#include <QDebug>
#include <QLayout>
#include <QMessageBox>

BooksWidget::BooksWidget(QWidget *parent)
    : AntiquaCRM::TabsIndex{"books_tab", parent} {
  setObjectName(BOOKS_SQL_TABLE_NAME);
  setWindowTitle(tr("Books"));
  setWindowIcon(AntiquaCRM::AntiquaApplIcon("view-log"));

  // Begin MainPage layout
  m_mainPage = new QWidget(this);
  QVBoxLayout *m_p1Layout = new QVBoxLayout(m_mainPage);
  m_p1Layout->setContentsMargins(0, 0, 0, 0);

  // Searchbar
  m_searchBar = new BooksSearchBar(m_mainPage);
  m_searchBar->setMinLength(3);
  m_p1Layout->addWidget(m_searchBar);

  // TableView
  m_table = new BooksTableView(m_mainPage);
  m_p1Layout->addWidget(m_table);

  // StatusBar
  m_statusBar = new BooksStatusBar(m_mainPage);
  m_p1Layout->addWidget(m_statusBar);

  m_mainPage->setLayout(m_p1Layout);
  insertWidget(0, m_mainPage);
  // End

  // Begin Editor
  m_editorPage = new QScrollArea(this);
  m_editorPage->setObjectName("book_editor_scrollarea");
  m_editorPage->setWidgetResizable(true);
  m_editorWidget = new BooksEditor(this);
  m_editorWidget->installEventFilter(this);
  m_editorPage->setWidget(m_editorWidget);
  insertWidget(1, m_editorPage);
  // End

  setCurrentIndex(0);

  // Signals::BooksSearchBar
  connect(this, SIGNAL(sendSetSearchFocus()), m_searchBar,
          SLOT(setSearchFocus()));
  connect(this, SIGNAL(sendSetSearchFilter()), m_searchBar,
          SLOT(setFilterFocus()));
  connect(m_searchBar, SIGNAL(sendSearchClicked()), SLOT(createSearchQuery()));
  connect(m_searchBar, SIGNAL(sendStockEnabled(bool)), m_statusBar,
          SLOT(setStockEnabled(bool)));
  connect(m_searchBar, SIGNAL(sendNotify(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  // Signals::BooksTableView
  connect(m_table, SIGNAL(sendQueryReport(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  connect(m_table, SIGNAL(sendCopyToClibboard(const QString &)),
          SLOT(copyToClipboard(const QString &)));

  connect(m_table, SIGNAL(sendOpenEntry(qint64)), SLOT(openEntry(qint64)));

  connect(m_table, SIGNAL(sendCreateNewEntry()), SLOT(createNewEntry()));

  connect(m_table, SIGNAL(sendSocketOperation(const QJsonObject &)),
          SLOT(sendSocketOperation(const QJsonObject &)));

  // Signals::BooksEditor
  connect(m_editorWidget, SIGNAL(sendLeaveEditor()), SLOT(openStartPage()));

  // Signals::BookStatusBar
  connect(m_statusBar, SIGNAL(sendCreateEntry()), SLOT(createNewEntry()));
  connect(m_statusBar, SIGNAL(sendHistoryQuery(const QString &)),
          SLOT(createSearchQuery(const QString &)));

  connect(m_statusBar, SIGNAL(sendDefaultView()), SLOT(setDefaultTableView()));
  connect(m_statusBar, SIGNAL(sendReloadView()), m_table,
          SLOT(setReloadView()));
}

void BooksWidget::popupWarningTabInEditMode() {
  QString info(tr("Can't open this Article"));
  info.append("<p>");
  info.append(tr("Because books tab is in edit mode."));
  info.append("</p><p>");
  info.append(tr("You need to save and close that open book first."));
  info.append("</p>");
  QMessageBox::information(this, windowTitle(), info);
}

void BooksWidget::setDefaultTableView() {
  m_searchBar->setClearAndFocus();
  m_table->setQuery(m_table->defaultWhereClause());
  m_statusBar->setCreateButtonEnabled(false);
  if (currentIndex() != 0)
    setCurrentIndex(0);
}

void BooksWidget::openStartPage() {
  if (m_table->isAutoRefreshEnabled()) {
    m_statusBar->setCreateButtonEnabled(false);
    m_table->setReloadView();
  }
  if (currentIndex() != 0)
    setCurrentIndex(0);
}

void BooksWidget::createSearchQuery(const QString &query) {
  if (query.isEmpty()) {
    // Die Standardabfrage wird aufgerufen!
    QString w_sql = m_searchBar->getSearchStatement();
    if (m_searchBar->searchLength() > 1 && w_sql.length() > 1) {
      m_table->setQuery(w_sql);
      // Nur Aktivieren wenn eine Suche ausgeführt wurde.
      m_statusBar->setCreateButtonEnabled(true);
    }
    return;
  }
  // Verlaufs und Suchanfrage
  m_table->setQuery(query);
  m_statusBar->setCreateButtonEnabled(false);
}

void BooksWidget::createNewEntry() {
  if (m_editorWidget->createNewEntry()) {
    setCurrentIndex(1);
  }
}

void BooksWidget::openEntry(qint64 articleId) {
  if (articleId < 1)
    return;

  if (currentIndex() != 0) {
    popupWarningTabInEditMode();
    return;
  }

  if (m_editorWidget->openEditEntry(articleId)) {
    setCurrentIndex(1);
  }
}

void BooksWidget::onEnterChanged() {
  if (!initialed) {
    initialed = m_table->setQuery();
    m_searchBar->setFilter(0);
  }
}

AntiquaCRM::TabsIndex::ViewPage BooksWidget::currentView() {
  switch (currentIndex()) {
  case 1:
    return AntiquaCRM::TabsIndex::EditorView;

  default:
    return AntiquaCRM::TabsIndex::MainView;
  }
}

bool BooksWidget::customAction(const QJsonObject &obj) {
  if (obj.isEmpty() || !obj.contains("window_operation"))
    return false;

  if (!initialed) /**< first call? */
    onEnterChanged();

  if (currentIndex() != 0) {
    popupWarningTabInEditMode();
    return false;
  }

  QString op = obj.value("window_operation").toString();
  if (!obj.contains(op))
    return false;

  qint64 a_id = obj.value(op).toInt();
  if (op == "open_article" && a_id > 0) {
    openEntry(a_id);
    return true;
  }

  return false;
}
