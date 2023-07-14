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
    : AntiquaCRM::TabsIndex{BOOKS_INTERFACE_TABID, parent} {
  setObjectName("books_tab_widget");
  setWindowIcon(AntiquaCRM::antiquaIcon("kjournal"));
  setClosable(false);

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
  if (currentIndex() != 0)
    return;

  m_searchBar->setClearAndFocus();
  m_table->setQuery(m_table->defaultWhereClause());
  m_statusBar->setCreateButtonEnabled(false);
}

void BooksWidget::openStartPage() {
  setCurrentIndex(0);
  if (m_table->isAutoRefreshEnabled()) {
    m_statusBar->setCreateButtonEnabled(false);
    m_table->setReloadView();
  }

#ifdef ANTIQUA_DEVELOPEMENT
  if (isWindowModified())
    qDebug() << Q_FUNC_INFO << isWindowModified();
#endif
}

void BooksWidget::createSearchQuery(const QString &history) {
  // Verlaufs und Suchanfrage
  if (history.length() > 10) {
    m_table->setQuery(history);
    m_statusBar->setCreateButtonEnabled(false);
    return;
  }
  // Die Standardabfrage wird aufgerufen!
  QString _sql = m_searchBar->getSearchStatement();
  if (_sql.isEmpty()) {
    qWarning("BooksWidget::createSearchQuery „length()“, to small!");
    return;
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << "Books:Search" << _sql;
#endif

  m_table->setQuery(_sql);
  // Nur Aktivieren wenn eine Suche ausgeführt wurde.
  m_statusBar->setCreateButtonEnabled(true);
}

void BooksWidget::createNewEntry() {
  if (currentIndex() == 0 && m_editorWidget->createNewEntry()) {
    setCurrentIndex(1);
  }
#ifdef ANTIQUA_DEVELOPEMENT
  else {
    qWarning("Reject BooksWidget::createNewEntry - no main page view!");
  }
#endif
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

const QString BooksWidget::getTitle() const { return tr("Books"); }

bool BooksWidget::customAction(const QJsonObject &obj) {
  if (obj.isEmpty() || !obj.contains("ACTION"))
    return false;

  // first call?
  onEnterChanged();

  if (currentIndex() != 0) {
    popupWarningTabInEditMode();
    return false;
  }

  QString op = obj.value("ACTION").toString();
  if (!obj.contains(op))
    return false;

  qint64 a_id = obj.value(op).toInt();
  if (op == "open_article" && a_id > 0) {
    openEntry(a_id);
    return true;
  }

  return false;
}
