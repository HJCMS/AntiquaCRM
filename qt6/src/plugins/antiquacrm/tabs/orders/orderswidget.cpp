// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderswidget.h"
#include "ordersconfig.h"
#include "orderseditor.h"
#include "orderssearchbar.h"
#include "ordersstatusbar.h"
#include "orderstableview.h"

#include <AntiquaCRM>
#include <QtWidgets>

OrdersWidget::OrdersWidget(QWidget *parent)
    : AntiquaCRM::TabsIndex{ORDERS_INTERFACE_TABID, parent} {
  setObjectName("orders_tab_widget");
  setWindowIcon(AntiquaCRM::antiquaIcon("view-financial-transfer"));
  setClosable(false);
  // Begin MainPage layout
  m_mainPage = new QWidget(this);
  QVBoxLayout *m_p1Layout = new QVBoxLayout(m_mainPage);
  m_p1Layout->setContentsMargins(0, 0, 0, 0);

  m_searchBar = new OrdersSearchBar(m_mainPage);
  m_p1Layout->addWidget(m_searchBar);

  m_table = new OrdersTableView(m_mainPage);
  m_p1Layout->addWidget(m_table);
  m_p1Layout->setStretch(1, 1);

  m_statusBar = new OrdersStatusBar(m_mainPage);
  m_p1Layout->addWidget(m_statusBar);

  m_mainPage->setLayout(m_p1Layout);
  insertWidget(0, m_mainPage);
  // End

  // Begin Editor
  m_editorPage = new QScrollArea(this);
  m_editorPage->setObjectName("CDsVinyl_editor_scrollarea");
  m_editorPage->setWidgetResizable(true);
  m_editorWidget = new OrdersEditor(this);
  m_editorWidget->installEventFilter(this);
  m_editorPage->setWidget(m_editorWidget);
  insertWidget(1, m_editorPage);
  // End

  setCurrentIndex(0);

  // Signals::OrdersSearchBar
  connect(this, SIGNAL(sendSetSearchFocus()), m_searchBar,
          SLOT(setSearchFocus()));
  connect(this, SIGNAL(sendSetSearchFilter()), m_searchBar,
          SLOT(setFilterFocus()));
  connect(m_searchBar, SIGNAL(sendSearchClicked()), SLOT(createSearchQuery()));
  connect(m_searchBar, SIGNAL(sendStockEnabled(bool)), m_statusBar,
          SLOT(setStockEnabled(bool)));
  connect(m_searchBar, SIGNAL(sendNotify(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  // Signals::OrdersTableView
  connect(m_table, SIGNAL(sendQueryReport(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  connect(m_table, SIGNAL(sendCopyToClibboard(const QString &)),
          SLOT(copyToClipboard(const QString &)));

  connect(m_table, SIGNAL(sendOpenEntry(qint64)), SLOT(openEntry(qint64)));

  connect(m_table, SIGNAL(sendCreateNewEntry()), SLOT(createNewEntry()));

  connect(m_table, SIGNAL(sendSocketOperation(const QJsonObject &)),
          SLOT(sendSocketOperation(const QJsonObject &)));

  // Signals::OrdersEditor
  connect(m_editorWidget, SIGNAL(sendLeaveEditor()), SLOT(openStartPage()));

  // Signals::OrdersStatusBar
  connect(m_statusBar, SIGNAL(sendHistoryQuery(const QString &)),
          SLOT(createSearchQuery(const QString &)));
  connect(m_statusBar, SIGNAL(sendDefaultView()), SLOT(setDefaultTableView()));
  connect(m_statusBar, SIGNAL(sendReloadView()), m_table,
          SLOT(setReloadView()));
}

void OrdersWidget::popupWarningTabInEditMode() {
  QString info(tr("Can't open this Order"));
  info.append("<p>");
  info.append(tr("Because Orders tab is in edit mode."));
  info.append("</p><p>");
  info.append(tr("You need to save and close the editor first."));
  info.append("</p>");
  QMessageBox::information(this, windowTitle(), info);
}

void OrdersWidget::setDefaultTableView() {
  m_searchBar->setClearAndFocus();
  setCurrentIndex(0);
  m_table->setQuery(m_table->defaultWhereClause());
}

void OrdersWidget::openStartPage() {
  if (m_table->isAutoRefreshEnabled())
    m_table->setReloadView();

  setCurrentIndex(0);
}

void OrdersWidget::createSearchQuery(const QString &history) {
  if (!history.isEmpty()) {
    qDebug() << "QUERY:" << history;
    m_table->setQuery(history);
    return;
  }

  QString w_sql = m_searchBar->getSearchStatement();
  if (m_searchBar->searchLength() > 1 && w_sql.length() > 1) {
    m_table->setQuery(w_sql);
  }
}

void OrdersWidget::createNewEntry() {
  //
  qDebug() << Q_FUNC_INFO << "__TODO__";
}

void OrdersWidget::openEntry(qint64 oid) {
  if (oid < 1)
    return;

  if (currentIndex() != 0) {
    popupWarningTabInEditMode();
    return;
  }

  if (m_editorWidget->openEditEntry(oid)) {
    setCurrentWidget(m_editorPage);
  }
}

void OrdersWidget::onEnterChanged() {
  if (!initialed) {
    initialed = m_table->setQuery();
    setCurrentIndex(0);
  }
}

const QString OrdersWidget::getTitle() const { return tr("Orders"); }

bool OrdersWidget::customAction(const QJsonObject &obj) {
  qDebug() << Q_FUNC_INFO << "__TODO__" << obj;
  return false;
}
