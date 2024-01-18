// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "variousindex.h"
#include "variousconfig.h"
#include "variouseditor.h"
#include "varioussearchbar.h"
#include "variousstatusbar.h"
#include "varioustableview.h"

VariousIndex::VariousIndex(QWidget *parent)
    : AntiquaCRM::TabsIndex{VARIOUS_INTERFACE_TABID, parent} {
  setObjectName("various_tab_index");
  setWindowIcon(AntiquaCRM::antiquaIcon("applications-other"));
  setClosable(true);

  // Begin MainPage layout
  m_mainPage = new QWidget(this);
  QVBoxLayout *m_p1Layout = new QVBoxLayout(m_mainPage);
  m_p1Layout->setContentsMargins(0, 0, 0, 0);

  m_searchBar = new VariousSearchBar(m_mainPage);
  m_p1Layout->addWidget(m_searchBar);

  m_table = new VariousTableView(m_mainPage);
  m_p1Layout->addWidget(m_table);
  m_p1Layout->setStretch(1, 1);

  m_statusBar = new VariousStatusBar(m_mainPage);
  m_p1Layout->addWidget(m_statusBar);

  m_mainPage->setLayout(m_p1Layout);
  insertWidget(0, m_mainPage);
  // End

  // Begin Editor
  m_editorPage = new QScrollArea(this);
  m_editorPage->setObjectName("stitches_editor_scrollarea");
  m_editorPage->setWidgetResizable(true);
  m_editorWidget = new VariousEditor(this);
  m_editorWidget->installEventFilter(this);
  m_editorPage->setWidget(m_editorWidget);
  insertWidget(1, m_editorPage);
  // End

  setCurrentIndex(ViewPage::MainView);

  // Signals
  connect(this, SIGNAL(sendSetSearchFocus()), m_searchBar,
          SLOT(setSearchFocus()));

  connect(this, SIGNAL(sendSetSearchFilter()), m_searchBar,
          SLOT(setFilterFocus()));

  connect(m_searchBar, SIGNAL(sendSearchClicked()), SLOT(createSearchQuery()));

  connect(m_searchBar, SIGNAL(sendWithStockEnabled(bool)), m_statusBar,
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

void VariousIndex::setDefaultTableView() {
  if (currentPage() != ViewPage::MainView)
    return;

  m_searchBar->setClearAndFocus();
  m_table->setQuery(m_table->defaultWhereClause());
  m_statusBar->setCreateButtonEnabled(false);
}

void VariousIndex::openStartPage() {
  setCurrentIndex(ViewPage::MainView);
  if (m_table->isAutoRefreshEnabled()) {
    m_statusBar->setCreateButtonEnabled(false);
    m_table->setReloadView();
  }

#ifdef ANTIQUA_DEVELOPEMENT
  if (isWindowModified())
    qDebug() << Q_FUNC_INFO << isWindowModified();
#endif
}

void VariousIndex::createSearchQuery(const QString &history) {
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
    qWarning("VariousIndex::createSearchQuery „length()“, to small!");
    return;
  }
  // qDebug() << "Various:Search" << _sql;
  m_statusBar->startProgress();
  m_table->setQuery(_sql);
  m_statusBar->setCreateButtonEnabled(true);
  m_statusBar->finalizeProgress();
}

void VariousIndex::createNewEntry() {
  if (currentPage() == ViewPage::MainView && m_editorWidget->createNewEntry()) {
    setCurrentIndex(ViewPage::EditorView);
  }
#ifdef ANTIQUA_DEVELOPEMENT
  else {
    qWarning("Reject VariousIndex::createNewEntry - no main page view!");
  }
#endif
}

void VariousIndex::openEntry(qint64 articleId) {
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

void VariousIndex::onEnterChanged() {
  if (!initialed) {
    initialed = m_table->setQuery();
    return;
  }
  if (m_table->isAutoRefreshEnabled())
    m_table->setReloadView();
}

const QString VariousIndex::getTitle() const { return tr("Various"); }

bool VariousIndex::customAction(const QJsonObject &obj) {
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

const QStringList VariousIndex::acceptsCustomActions() const {
  return QStringList({"open_article"});
}
