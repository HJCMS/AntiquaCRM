// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryorders.h"
#include "applsettings.h"
#include "orderstable.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <QtCore/QDebug>
#include <QtCore/QHash>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>
#include <QtWidgets/QVBoxLayout>

InventoryOrders::InventoryOrders(QWidget *parent) : Inventory{parent} {
  setObjectName("InventoryOrders");
  setWindowTitle(tr("Orders"));

  ApplSettings cfg;
  minLength = cfg.value("search/startlength", 5).toInt();

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("inventory_assignments_layout");

  m_stackedWidget = new QStackedWidget(this);
  m_stackedWidget->setObjectName("assignments_stacked_widget");

  // BEGIN Page#0
  QWidget *siteOneWidget = new QWidget(m_stackedWidget);
  siteOneWidget->setObjectName("assignments_site_one_widget");
  QVBoxLayout *siteOneLayout = new QVBoxLayout(siteOneWidget);
  siteOneLayout->setObjectName("assignments_site_one_layout");

  m_tableView = new OrdersTable(this);
  siteOneLayout->addWidget(m_tableView);
  // END Page#0

  // BEGIN Page#1
  // TODO Editor
  // END Page#1

  m_stackedWidget->insertWidget(0, siteOneWidget);
  m_stackedWidget->setCurrentIndex(0);
  layout->addWidget(m_stackedWidget);

  setLayout(layout);

  // Signals
//   connect(m_tableView, SIGNAL(s_articleSelected(int)), this,
//           SLOT(articleSelected(int)));
//   connect(m_bookEditor, SIGNAL(s_postMessage(const QString &)), this,
//           SLOT(displayMessageBox(const QString &)));

  m_tableView->initOrders();
}

void InventoryOrders::searchConvert(const QString &query) {
  qDebug() << Q_FUNC_INFO << "TODO" << query;
}

void InventoryOrders::searchConvert() {
  qDebug() << Q_FUNC_INFO << "TODO";
}

void InventoryOrders::openTableView() {
  qDebug() << Q_FUNC_INFO << "TODO";
}

void InventoryOrders::openEditor(const QString &condition) {
  qDebug() << Q_FUNC_INFO << "TODO" << condition;
}

void InventoryOrders::articleSelected(int id) {
  qDebug() << Q_FUNC_INFO << "TODO" << id;
}

void InventoryOrders::updateValidator(int id) {
  qDebug() << Q_FUNC_INFO << "TODO" << id;
}
