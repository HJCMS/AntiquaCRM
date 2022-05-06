// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryorders.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "myicontheme.h"
#include "ordereditor.h"
#include "orderstable.h"

#include <QDebug>
#include <QHash>
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>

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

  QWidget *m_statusBar = new QWidget(this);
  QHBoxLayout *statusLayout = new QHBoxLayout(m_statusBar);
  m_statusInfo = new QLabel(m_statusBar);
  statusLayout->addWidget(m_statusInfo);
  statusLayout->addStretch(1);
  QPushButton *btn_refresh = new QPushButton(m_statusBar);
  btn_refresh->setText(tr("Refresh"));
  btn_refresh->setIcon(myIcon("reload"));
  statusLayout->addWidget(btn_refresh);
  m_statusBar->setLayout(statusLayout);
  siteOneLayout->addWidget(m_statusBar);

  m_stackedWidget->insertWidget(0, siteOneWidget);
  // END Page#0

  // BEGIN Page#1
  m_editor = new OrderEditor(this);
  m_editor->setEnabled(false);
  m_stackedWidget->insertWidget(1, m_editor);
  // END Page#1

  m_stackedWidget->setCurrentIndex(0);
  layout->addWidget(m_stackedWidget);

  setLayout(layout);

  connect(m_tableView, SIGNAL(s_editOrder(int)), this,
          SLOT(orderSelected(int)));

  connect(m_editor, SIGNAL(s_postMessage(const QString &)), this,
          SLOT(displayMessageBox(const QString &)));

  connect(m_editor, SIGNAL(s_leaveEditor()), this, SLOT(openTableView()));

  connect(btn_refresh, SIGNAL(clicked()), m_tableView,
          SLOT(refreshView()));

  m_tableView->initOrders();
}

void InventoryOrders::searchConvert(const QString &query) {}

void InventoryOrders::searchConvert() {}

void InventoryOrders::openTableView() {
  m_stackedWidget->setCurrentIndex(0);
  m_editor->setEnabled(false);
  m_tableView->initOrders();
}

void InventoryOrders::openEditor(const QString &condition) {
  Q_UNUSED(condition)
}

void InventoryOrders::orderSelected(int id) {
  if (id < 1) {
    return;
  }
  // qDebug() << Q_FUNC_INFO << id;
  m_editor->setEnabled(true);
  m_editor->openUpdateOrder(id);
  m_stackedWidget->setCurrentWidget(m_editor);
}

void InventoryOrders::createOrder(int costumerId) {
  m_editor->setEnabled(true);
  m_editor->openCreateOrder(costumerId);
  m_stackedWidget->setCurrentWidget(m_editor);
}
