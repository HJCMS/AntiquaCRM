// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabprintsstitches.h"
#include "printsstitcheseditor.h"
#include "printsstitchessearchbar.h"
#include "printsstitchesstatusbar.h"
#include "printsstitchestableview.h"

#include <QDebug>
#include <QLayout>
#include <QMessageBox>

TabPrintsStitches::TabPrintsStitches(QWidget *parent)
    : Inventory{"printsstitches_tab", parent} {
  setObjectName("inventory_printsstitches_tab");
  setWindowTitle(tr("Prints and Stitches"));
  setWindowIcon(getTabIcon("view_image"));
  setClosable(true);

  // Begin MainPage layout
  m_mainPage = new QWidget(this);
  QVBoxLayout *m_p1Layout = new QVBoxLayout(m_mainPage);
  m_p1Layout->setContentsMargins(0, 0, 0, 0);
  m_searchBar = new PrintsStitchesSearchBar(this);
  m_p1Layout->addWidget(m_searchBar);
  m_table = new PrintsStitchesTableView(m_mainPage);
  m_p1Layout->addWidget(m_table);
  m_statusBar = new PrintsStitchesStatusBar(this);
  m_p1Layout->addWidget(m_statusBar);
  m_mainPage->setLayout(m_p1Layout);
  insertWidget(0, m_mainPage);
  // End

  // Begin Editor
  m_editorPage = new QScrollArea(this);
  m_editorPage->setObjectName("prints_editor_scrollarea");
  m_editorPage->setWidgetResizable(true);
  m_editorWidget = new PrintsStitchesEditor(this);
  m_editorWidget->installEventFilter(this);
  m_editorPage->setWidget(m_editorWidget);
  insertWidget(1, m_editorPage);
  // End

  setCurrentIndex(0);

  // Signals::PrintsStitchesSearchBar
  connect(this, SIGNAL(sendSetSearchFocus()), m_searchBar,
          SLOT(setSearchFocus()));
  connect(this, SIGNAL(sendSetSearchFilter()), m_searchBar,
          SLOT(setFilterFocus()));
  connect(m_searchBar, SIGNAL(sendSearchClicked()), SLOT(createSearchQuery()));
  connect(m_searchBar, SIGNAL(sendRestoreView()), SLOT(setDefaultTableView()));

  // Signals::PrintsStitchesTableView
  connect(m_table, SIGNAL(sendQueryReport(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  connect(m_table, SIGNAL(sendCopyToClibboard(const QString &)),
          SLOT(copyToClipboard(const QString &)));

  connect(m_table, SIGNAL(sendOpenEntry(qint64)), SLOT(openEntry(qint64)));

  connect(m_table, SIGNAL(sendCreateNewEntry()), SLOT(createNewEntry()));

  connect(m_table, SIGNAL(sendSocketOperation(const QJsonObject &)),
          SLOT(sendSocketOperation(const QJsonObject &)));

  // Signals::PrintsEditor
  connect(m_editorWidget, SIGNAL(sendLeaveEditor()), SLOT(openStartPage()));

  // Signals::PrintsStitchesStatusBar
  connect(m_statusBar, SIGNAL(sendCreateEntry()), SLOT(createNewEntry()));
  connect(m_statusBar, SIGNAL(sendHistoryQuery(const QString &)),
          SLOT(createSearchQuery(const QString &)));

  connect(m_statusBar, SIGNAL(sendReloadView()), m_table,
          SLOT(setReloadView()));
}

void TabPrintsStitches::popupWarningTabInEditMode() {
  QString info(tr("Can't open this Article"));
  info.append("<p>");
  info.append(tr("Because Prints and Stitches tab is in edit mode."));
  info.append("</p><p>");
  info.append(tr("You need to save and close that opened first."));
  info.append("</p>");
  QMessageBox::information(this, windowTitle(), info);
}

void TabPrintsStitches::setDefaultTableView() {
  m_searchBar->setClearAndFocus();
  m_table->setQuery(m_table->defaultWhereClause());
  // m_statusBar->setCreateButtonEnabled(false);
  if (currentIndex() != 0)
    setCurrentIndex(0);
}

void TabPrintsStitches::openStartPage() {
  if (m_table->isAutoRefreshEnabled()) {
    // m_statusBar->setCreateButtonEnabled(false);
    m_table->setReloadView();
  }
  if (currentIndex() != 0)
    setCurrentIndex(0);
}

void TabPrintsStitches::createSearchQuery(const QString &query) {
  if (query.isEmpty()) {
    // Die Standardabfrage wird aufgerufen!
    QString w_sql = m_searchBar->getSearchStatement();
    if (m_searchBar->searchLength() > 1 && w_sql.length() > 1) {
      m_table->setQuery(w_sql);
      // Nur Aktivieren wenn eine Suche ausgeführt wurde.
      // m_statusBar->setCreateButtonEnabled(true);
    }
    return;
  }
  // Verlaufs und Suchanfrage
  m_table->setQuery(query);
  //  m_statusBar->setCreateButtonEnabled(false);
}

void TabPrintsStitches::createNewEntry() {
  if (m_editorWidget->createNewEntry()) {
    setCurrentIndex(1);
  }
}

void TabPrintsStitches::openEntry(qint64 articleId) {
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

void TabPrintsStitches::onEnterChanged() {
  if (!initialed) {
    initialed = m_table->setQuery();
    m_searchBar->setFilter(0);
  }
}

bool TabPrintsStitches::customAction(const QJsonObject &obj) {
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
