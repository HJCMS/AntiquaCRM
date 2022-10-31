// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "taborders.h"

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

  // m_p1Layout->addWidget();
  m_mainPage->setLayout(m_p1Layout);
  insertWidget(0, m_mainPage);
  // End

  // Begin Editor
  m_editorPage = new QScrollArea(this);
  m_editorPage->setObjectName("orders_editor_scrollarea");
  m_editorPage->setWidgetResizable(true);
  // m_editorWidget = new OrderEditor(m_editorPage);
  // m_editorPage->setWidget(m_editorWidget);
  insertWidget(1, m_editorPage);
  // End
}

void TabOrders::openStartPage() { setCurrentIndex(0); }

void TabOrders::createSearchQuery(const QString &query) {}

void TabOrders::createNewEntry() {
  //  if (m_editorWidget->createNewEntry()) {
  //    setCurrentWidget(m_editorPage);
  //  }
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

  //  if (m_editorWidget->openEditEntry(articleId)) {
  //    setCurrentWidget(m_editorPage);
  //  }
}

void TabOrders::onEnterChanged() {
  if (!initialed) {
    // initialed = m_table->setQuery();
    // m_searchBar->setFilter(0);
    setCurrentWidget(m_mainPage);
  }
}
