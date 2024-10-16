// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "stitcheswidget.h"
#include "stitcheseditor.h"
#include "stitchessearchbar.h"
#include "stitchesstatusbar.h"
#include "stitchestableview.h"

#include <QDebug>
#include <QLayout>
#include <QMessageBox>

StitchesWidget::StitchesWidget(QWidget* parent) : AntiquaCRM::TabsIndex{"stitches_tab", parent} {
  setObjectName("stitches_tab_widget");
  setWindowIcon(AntiquaCRM::antiquaIcon("kjournal"));
  setWindowTitle(getTitle());
  setClosable(true);

  // Begin MainPage layout
  m_mainPage = new QWidget(this);
  QVBoxLayout* m_p1Layout = new QVBoxLayout(m_mainPage);
  m_p1Layout->setContentsMargins(0, 0, 0, 0);

  // Searchbar
  m_searchBar = new StitchesSearchBar(m_mainPage);
  m_searchBar->setMinLength(3);
  m_p1Layout->addWidget(m_searchBar);

  // TableView
  m_table = new StitchesTableView(m_mainPage);
  m_p1Layout->addWidget(m_table);

  // StatusBar
  m_statusBar = new StitchesStatusBar(m_mainPage);
  m_p1Layout->addWidget(m_statusBar);

  m_mainPage->setLayout(m_p1Layout);
  insertWidget(0, m_mainPage);
  // End

  // Begin Editor
  m_editorPage = new QScrollArea(this);
  m_editorPage->setObjectName("stitches_editor_scrollarea");
  m_editorPage->setWidgetResizable(true);
  m_editorWidget = new StitchesEditor(this);
  m_editorWidget->installEventFilter(this);
  m_editorPage->setWidget(m_editorWidget);
  insertWidget(1, m_editorPage);
  // End

  setCurrentIndex(ViewPage::MainView);

  // Signals::StitchesSearchBar
  connect(this, SIGNAL(sendSetSearchFocus()), m_searchBar, SLOT(setSearchFocus()));
  connect(this, SIGNAL(sendSetSearchFilter()), m_searchBar, SLOT(setFilterFocus()));
  connect(m_searchBar, SIGNAL(sendSearchClicked()), SLOT(createSearchQuery()));
  connect(m_searchBar, SIGNAL(sendWithStockEnabled(bool)), m_statusBar,
          SLOT(setStockEnabled(bool)));
  connect(m_searchBar, SIGNAL(sendNotify(QString)), m_statusBar, SLOT(showMessage(QString)));

  // Signals::StitchesTableView
  connect(m_table, SIGNAL(sendQueryReport(QString)), m_statusBar, SLOT(showMessage(QString)));

  connect(m_table, SIGNAL(sendCopyToClibboard(QString)), SLOT(copyToClipboard(QString)));

  connect(m_table, SIGNAL(sendOpenEntry(qint64)), SLOT(openEntry(qint64)));

  connect(m_table, SIGNAL(sendCreateNewEntry()), SLOT(createNewEntry()));

  connect(m_table, SIGNAL(sendSocketOperation(QJsonObject)),
          SLOT(sendSocketOperation(QJsonObject)));

  // Signals::StitchesEditor
  connect(m_editorWidget, SIGNAL(sendLeaveEditor()), SLOT(openStartPage()));

  // Signals::StitchestatusBar
  connect(m_statusBar, SIGNAL(sendCreateEntry()), SLOT(createNewEntry()));
  connect(m_statusBar, SIGNAL(sendHistoryQuery(QString)), SLOT(createSearchQuery(QString)));

  connect(m_statusBar, SIGNAL(sendDefaultView()), SLOT(setDefaultTableView()));
  connect(m_statusBar, SIGNAL(sendReloadView()), m_table, SLOT(setReloadView()));
}

void StitchesWidget::setDefaultTableView() {
  if (currentPage() != ViewPage::MainView)
    return;

  m_searchBar->setClearAndFocus();
  m_table->setQuery(m_table->defaultWhereClause());
  m_statusBar->setCreateButtonEnabled(false);
}

void StitchesWidget::openStartPage() {
  setCurrentIndex(ViewPage::MainView);
  if (m_table->isAutoRefreshEnabled()) {
    m_statusBar->setCreateButtonEnabled(false);
    m_table->setReloadView();
  }

#ifdef ANTIQUA_DEVELOPMENT
  if (isWindowModified())
    qDebug() << Q_FUNC_INFO << isWindowModified();
#endif
}

void StitchesWidget::createSearchQuery(const QString& history) {
  // Verlaufs und Suchanfrage
  if (history.length() > 10) {
    m_statusBar->startProgress();
    m_table->setQuery(history);
    // Nur Aktivieren wenn eine Suche ausgeführt wurde.
    m_statusBar->setCreateButtonEnabled(false);
    m_statusBar->finalizeProgress();
    return;
  }
  // Die Standardabfrage wird aufgerufen!
  const QString _sql = m_searchBar->getSearchStatement();
  if (_sql.isEmpty()) {
    qWarning("StitchesWidget::createSearchQuery „length()“, to small!");
    return;
  }
  // qDebug() << "Stitches:Search" << _sql;
  m_statusBar->startProgress();
  m_table->setQuery(_sql);
  m_statusBar->setCreateButtonEnabled(true);
  m_statusBar->finalizeProgress();
}

void StitchesWidget::createNewEntry() {
  if (currentPage() == ViewPage::MainView && m_editorWidget->createNewEntry()) {
    setCurrentIndex(ViewPage::EditorView);
  }
#ifdef ANTIQUA_DEVELOPMENT
  else {
    qWarning("Reject StitchesWidget::createNewEntry - no main page view!");
  }
#endif
}

void StitchesWidget::openEntry(qint64 articleId) {
  if (articleId < 1)
    return;

  if (currentPage() != ViewPage::MainView) {
    openWarningPopUpPageIndex(windowTitle());
    return;
  }

  if (m_editorWidget->openEditEntry(articleId)) {
    setCurrentIndex(ViewPage::EditorView);
  }
}

void StitchesWidget::onEnterChanged() {
  if (!initialed) {
    initialed = m_table->setQuery();
    m_searchBar->setFilter(0);
    return;
  }
  if (m_table->isAutoRefreshEnabled())
    m_table->setReloadView();
}

const QString StitchesWidget::getTitle() const {
  return tr("Prints && Stitches");
}

bool StitchesWidget::customAction(const QJsonObject& obj) {
  if (obj.isEmpty() || !obj.contains("ACTION"))
    return false;

  // first call?
  onEnterChanged();

  if (currentPage() != ViewPage::MainView) {
    openWarningPopUpPageIndex(windowTitle());
    return false;
  }

  const QString _action = obj.value("ACTION").toString();
  if (_action == "open_article") {
    qint64 _aid = obj.value("VALUE").toInt();
    if (m_editorWidget->openEditEntry(_aid)) {
      setCurrentIndex(ViewPage::EditorView);
      return true;
    }
  }

  return false;
}

const QStringList StitchesWidget::acceptsCustomActions() const {
  return QStringList({"open_article"});
}
