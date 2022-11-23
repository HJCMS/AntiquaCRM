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

#ifndef ORDERS_EDITOR_INDEX
#define ORDERS_EDITOR_INDEX 1
#endif

TabOrders::TabOrders(QWidget *parent) : Inventory{"orders_tab", parent} {
  setObjectName("inventory_orders");
  setWindowTitle(tr("Orders"));
  setWindowIcon(getTabIcon("spreadsheet"));
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
  insertWidget(ORDERS_EDITOR_INDEX, m_editorPage);
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

  connect(m_table, SIGNAL(sendCreateNewEntry()), SLOT(createNewEntry()));

  // Signals::OrdersEditor
  connect(m_editorWidget, SIGNAL(sendLeaveEditor()), SLOT(openStartPage()));

  // Signals::OrdersStatusBar
  connect(m_statusBar, SIGNAL(sendHistoryQuery(const QString &)),
          SLOT(createSearchQuery(const QString &)));
  connect(m_statusBar, SIGNAL(sendDefaultView()), SLOT(setDefaultTableView()));
  connect(m_statusBar, SIGNAL(sendReloadView()), m_table,
          SLOT(setReloadView()));
}

void TabOrders::popupWarningTabInEditMode() {
  QString info("<p>" + tr("Cannot open Order Editor!"));
  info.append("</p><p>");
  info.append(tr("Because the order tab is not in mainview mode."));
  info.append("</p><p>");
  info.append(tr("Please save and close opened orders first."));
  info.append("</p>");
  QMessageBox::information(this, tr("Ordereditor"), info);
}

void TabOrders::setDefaultTableView() {
  m_editorPage->setEnabled(false);
  m_searchBar->setFilter(0);
  setCurrentIndex(0);
  m_table->setQuery(OrdersTableView::defaultWClause());
}

void TabOrders::openStartPage() {
  m_editorPage->setEnabled(false);
  if (m_table->rowCount() > 0 && m_table->rowCount() < 50)
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

void TabOrders::openEntry(qint64 o_id) {
  if (o_id < 1)
    return;

  if (currentIndex() != 0) {
    popupWarningTabInEditMode();
    return;
  }

  if (m_editorWidget->openEditEntry(o_id)) {
    m_editorPage->setEnabled(true);
    setCurrentWidget(m_editorPage);
  }
}

void TabOrders::onEnterChanged() {
  if (!initialed) {
    initialed = m_table->setQuery();
    m_editorPage->setEnabled(false);
    setCurrentIndex(0);
  }
}

bool TabOrders::customAction(const QJsonObject &obj) {
  if (obj.isEmpty() || !obj.contains("window_operation"))
    return false;

  if (!initialed) /**< first call? */
    onEnterChanged();

  QString operation = obj.value("window_operation").toString();
  if (!obj.contains(operation))
    return false;

  QJsonValue value = obj.value(operation);
  QJsonValue::Type type = value.type();
  if (value.isNull()) {
    sendStatusMessage(tr("Some arguments missing for a new Order!"));
    return false;
  }

  // Aktionen bei geöffneten Editor
  if (operation == "add_article" && type == QJsonValue::Double) {
    qint64 aid = value.toInt();
    if (currentIndex() != ORDERS_EDITOR_INDEX || aid < 1) {
      openWarningPopUp(tr("Add Article %1 to Order rejected.").arg(aid),
                       tr("Can't add Article when no Order is opened!"));
      return false;
    }
    return m_editorWidget->addArticle(aid);
  }

  // Alle weiteren Aktionen benötigen die Startseite
  if (currentIndex() != 0) {
    popupWarningTabInEditMode();
    return false;
  }

  if (operation == "open_order" && type == QJsonValue::Double) {
    qint64 o_id = value.toInt();
    if (o_id > 0) {
      openEntry(o_id);
      return true;
    }
  } else if (operation == "create_order" && type == QJsonValue::String) {
    QString pr_order = value.toString().trimmed();
    if (pr_order.isEmpty()) {
      sendStatusMessage(tr("Some arguments missing for a new Order!"));
      return false;
    }
    if (m_editorWidget->createNewProviderOrder(pr_order)) {
      m_editorPage->setEnabled(true);
      setCurrentWidget(m_editorPage);
      return true;
    }
  } else if (operation == "new_order" && type == QJsonValue::Double) {
    qint64 c_id = value.toInt();
    if (c_id < 1) {
      sendStatusMessage(tr("Some arguments missing for a new Order!"));
      return false;
    }
    if (m_editorWidget->createNewOrder(c_id)) {
      m_editorPage->setEnabled(true);
      setCurrentWidget(m_editorPage);
      return true;
    }
    return false;
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << obj << Qt::endl;
#endif

  return false;
}
