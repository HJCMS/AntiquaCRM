// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "taborders.h"
#include "orderseditor.h"
#include "orderssearchbar.h"
#include "ordersstatusbar.h"
#include "orderstableview.h"

#include <QIcon>
#include <QLayout>
#include <QMessageBox>

TabOrders::TabOrders(QWidget *parent) : Inventory{"orders_tab", parent} {
  setObjectName("inventory_orders");
  setWindowTitle(tr("Orders"));
  setWindowIcon(getTabIcon("edit"));
  // Begin MainPage layout
  m_mainPage = new QWidget(this);
  QVBoxLayout *m_p1Layout = new QVBoxLayout(m_mainPage);
  m_p1Layout->setContentsMargins(0, 0, 0, 0);
  m_searchBar = new OrdersSearchBar(m_mainPage);
  m_p1Layout->addWidget(m_searchBar);
  m_table = new OrdersTableView(m_mainPage);
  m_p1Layout->addWidget(m_table);
  m_statusBar = new OrdersStatusBar(m_mainPage);
  m_p1Layout->addWidget(m_statusBar);
  m_mainPage->setLayout(m_p1Layout);
  insertWidget(0, m_mainPage);
  // End

  // Begin Editor
  m_editorPage = new QScrollArea(this);
  m_editorPage->setObjectName("orders_editor_scrollarea");
  m_editorPage->setWidgetResizable(true);
  m_editorWidget = new OrdersEditor(m_editorPage);
  m_editorPage->setWidget(m_editorWidget);
  insertWidget(1, m_editorPage);
  // End

  // Signals::OrdersSearchBar
  connect(this, SIGNAL(sendSetSearchFocus()), m_searchBar,
          SLOT(setSearchFocus()));

  connect(this, SIGNAL(sendSetSearchFilter()), m_searchBar,
          SLOT(setFilterFocus()));

  connect(m_searchBar, SIGNAL(sendSearchClicked()), SLOT(createSearchQuery()));
  connect(m_searchBar, SIGNAL(sendRestoreView()), SLOT(setDefaultTableView()));

  // Signals::OrdersTableView
  connect(m_table, SIGNAL(sendQueryReport(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  connect(m_table, SIGNAL(sendCopyToClibboard(const QString &)),
          SLOT(copyToClipboard(const QString &)));

  connect(m_table, SIGNAL(sendOpenEntry(qint64)), SLOT(openEntry(qint64)));

  connect(m_table, SIGNAL(sendCurrentId(qint64)),
          SIGNAL(sendIdToOrder(qint64)));

  connect(m_table, SIGNAL(sendCreateNewEntry()), SLOT(createNewEntry()));

  // Signals::OrdersEditor
  connect(m_editorWidget, SIGNAL(sendLeaveEditor()), SLOT(openStartPage()));

  // Signals::OrdersStatusBar
  connect(m_statusBar, SIGNAL(sendHistoryQuery(const QString &)),
          SLOT(createSearchQuery(const QString &)));

  connect(m_statusBar, SIGNAL(sendReloadView()), m_table,
          SLOT(setReloadView()));
}

void TabOrders::setDefaultTableView() {
  m_editorPage->setEnabled(false);
  m_searchBar->setFilter(0);
  setCurrentIndex(0);
  m_table->setQuery(OrdersTableView::defaultWClause());
}

void TabOrders::openStartPage() {
  m_editorPage->setEnabled(false);
  if (m_table->rowCount() > 0 && m_table->rowCount() < 20)
    m_table->setReloadView();

  setCurrentIndex(0);
}

void TabOrders::createSearchQuery(const QString &query) {
  if (!query.isEmpty()) {
    m_table->setQuery(query);
    return;
  }

  QString w_sql = m_searchBar->getSearchStatement();
  if (m_searchBar->searchLength() > 1 && w_sql.length() > 1) {
    m_table->setQuery(w_sql);
  }
}

void TabOrders::createNewEntry() {
  if (m_editorWidget->createNewEntry()) {
    m_editorPage->setEnabled(true);
    setCurrentWidget(m_editorPage);
  }
}

void TabOrders::openEntry(qint64 customerId) {
  if (customerId < 1)
    return;

  if (currentIndex() != 0) {
    QString info(tr("Cannot open this order.") + "<br>");
    info.append(tr("Because the order tab is not in overview mode."));
    info.append("<p>");
    info.append(tr("Please save and close all open order first."));
    info.append("</p>");
    QMessageBox::information(this, tr("Ordereditor"), info);
    return;
  }

  if (m_editorWidget->openEditEntry(customerId)) {
    m_editorPage->setEnabled(true);
    setCurrentWidget(m_editorPage);
  }
}

void TabOrders::onEnterChanged() {
  if (!initialed) {
    m_searchBar->setFilter(0);
    m_editorPage->setEnabled(false);
    setCurrentIndex(0);
    initialed = m_table->setQuery();
  }
}
