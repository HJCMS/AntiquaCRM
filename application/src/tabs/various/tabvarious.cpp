// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabvarious.h"
#include "variouseditor.h"
#include "varioussearchbar.h"
#include "variousstatusbar.h"
#include "varioustableview.h"

#include <QDebug>
#include <QLayout>
#include <QMessageBox>

TabVarious::TabVarious(QWidget *parent) : Inventory{"various_tab", parent} {
  setObjectName("tab_various");
  setWindowTitle(tr("Various"));
  setWindowIcon(getTabIcon("view_info"));
  setClosable(true);

  // Begin MainPage layout
  m_mainPage = new QWidget(this);
  QVBoxLayout *m_p1Layout = new QVBoxLayout(m_mainPage);
  m_p1Layout->setContentsMargins(0, 0, 0, 0);
  m_searchBar = new VariousSearchBar(m_mainPage);
  m_p1Layout->addWidget(m_searchBar);
  m_table = new VariousTableView(m_mainPage);
  m_p1Layout->addWidget(m_table);
  m_statusBar = new VariousStatusBar(m_mainPage);
  m_p1Layout->addWidget(m_statusBar);
  m_mainPage->setLayout(m_p1Layout);
  insertWidget(0, m_mainPage);
  // End

  // Begin Editor
  m_editorPage = new QScrollArea(this);
  m_editorPage->setWidgetResizable(true);
  m_editorWidget = new VariousEditor(this);
  m_editorWidget->installEventFilter(this);
  m_editorPage->setWidget(m_editorWidget);
  insertWidget(1, m_editorPage);
  // End

  setCurrentIndex(0);

  // Signals::VariousSearchBar
  connect(this, SIGNAL(sendSetSearchFocus()), m_searchBar,
          SLOT(setSearchFocus()));
  connect(this, SIGNAL(sendSetSearchFilter()), m_searchBar,
          SLOT(setFilterFocus()));
  connect(m_searchBar, SIGNAL(sendSearchClicked()), SLOT(createSearchQuery()));
  connect(m_searchBar, SIGNAL(sendRestoreView()), SLOT(setDefaultTableView()));

  // Signals::VariousTableView
  connect(m_table, SIGNAL(sendQueryReport(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  connect(m_table, SIGNAL(sendCopyToClibboard(const QString &)),
          SLOT(copyToClipboard(const QString &)));

  connect(m_table, SIGNAL(sendOpenEntry(qint64)), SLOT(openEntry(qint64)));

  connect(m_table, SIGNAL(sendCreateNewEntry()), SLOT(createNewEntry()));

  connect(m_table, SIGNAL(sendSocketOperation(const QJsonObject &)),
          SLOT(sendSocketOperation(const QJsonObject &)));

  // Signals::VariousEditor
  connect(m_editorWidget, SIGNAL(sendLeaveEditor()), SLOT(openStartPage()));

  // Signals::VariousStatusBar
  connect(m_statusBar, SIGNAL(sendCreateEntry()), SLOT(createNewEntry()));
  connect(m_statusBar, SIGNAL(sendHistoryQuery(const QString &)),
          SLOT(createSearchQuery(const QString &)));

  connect(m_statusBar, SIGNAL(sendReloadView()), m_table,
          SLOT(setReloadView()));
}

void TabVarious::popupWarningTabInEditMode() {
  QString info(tr("Can't open this Article"));
  info.append("<p>");
  info.append(tr("Because Various tab is in edit mode."));
  info.append("</p><p>");
  info.append(tr("You need to save and close that opened first."));
  info.append("</p>");
  QMessageBox::information(this, windowTitle(), info);
}

void TabVarious::setDefaultTableView() {
  m_searchBar->setClearAndFocus();
  m_table->setQuery(m_table->defaultWhereClause());
  m_statusBar->setCreateButtonEnabled(false);
  if (currentIndex() != 0)
    setCurrentIndex(0);
}

void TabVarious::openStartPage() {
  if (m_table->isAutoRefreshEnabled()) {
    m_statusBar->setCreateButtonEnabled(false);
    m_table->setReloadView();
  }
  if (currentIndex() != 0)
    setCurrentIndex(0);
}

void TabVarious::createSearchQuery(const QString &query) {
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

void TabVarious::createNewEntry() {
  if (m_editorWidget->createNewEntry()) {
    setCurrentIndex(1);
  }
}

void TabVarious::openEntry(qint64 articleId) {
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

void TabVarious::onEnterChanged() {
  if (!initialed) {
    initialed = m_table->setQuery();
    m_searchBar->setFilter(0);
  }
}

Inventory::ViewIndex TabVarious::currentView() {
  switch (currentIndex()) {
  case 1:
    return Inventory::ViewIndex::EditorView;

  default:
    return Inventory::ViewIndex::MainView;
  }
}

bool TabVarious::customAction(const QJsonObject &obj) {
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
