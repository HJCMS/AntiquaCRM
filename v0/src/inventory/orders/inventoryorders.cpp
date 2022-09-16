// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryorders.h"
#include "ordereditor.h"
#include "orderstable.h"
#include "orderstoolbar.h"
#include <AntiquaCRM>

#include <QDebug>
#include <QHash>
#include <QLabel>
#include <QLayout>
#include <QVBoxLayout>

InventoryOrders::InventoryOrders(QWidget *parent) : Inventory{parent} {
  setObjectName("InventoryOrders");
  setWindowTitle(tr("Orders") + "[*]");
  setClosable(false);

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

  m_toolBar = new OrdersToolBar(this);
  siteOneLayout->addWidget(m_toolBar);

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

  connect(m_tableView, SIGNAL(s_editOrder(int)), this, SLOT(updateOrder(int)));
  connect(m_tableView, SIGNAL(s_openCustomer(int)), this,
          SIGNAL(s_viewCustomer(int)));
  connect(m_editor, SIGNAL(sendStatusMessage(const QString &)), this,
          SLOT(displayMessageBox(const QString &)));
  connect(m_editor, SIGNAL(sendStatusBarMessage(const QString &)), this,
          SIGNAL(sendStatusBarMessage(const QString &)));
  connect(m_editor, SIGNAL(s_leaveEditor()), this, SLOT(openTableView()));
  connect(m_editor, SIGNAL(s_isModified(bool)), this,
          SLOT(setIsModified(bool)));
  connect(m_toolBar, SIGNAL(sendRefreshView()), m_tableView,
          SLOT(refreshView()));
  connect(m_toolBar, SIGNAL(sendDefaultView()), m_tableView,
          SLOT(initOrders()));
  connect(m_toolBar, SIGNAL(sendCustomQuery(const QString &)), m_tableView,
          SLOT(setCustomQuery(const QString &)));
  connect(m_toolBar, SIGNAL(sendSearchText(const QString &)), this,
          SLOT(searchConvert(const QString &)));
  connect(m_editor, SIGNAL(s_articleCount(int, int)), this,
          SIGNAL(s_articleCount(int, int)));
}

void InventoryOrders::searchConvert(const QString &txt) {
  int searchCell; // Siehe OrdersTableModel::headerData
  switch (m_toolBar->getSearchSection()) {
  case (OrdersToolBar::SEARCH_CUSTOMER):
    searchCell = 4; // customer
    break;

  case (OrdersToolBar::SEARCH_PROVIDER):
    searchCell = 6; // o_provider_info
    break;

  default:          // OrdersToolBar::SEARCH_ORDER_ID
    searchCell = 0; // o_id
    break;
  };
  m_tableView->searchHighlight(txt, searchCell);
}

void InventoryOrders::openTableView() {
  m_stackedWidget->setCurrentIndex(0);
  m_editor->setEnabled(false);
  m_tableView->initOrders();
  setIsModified(false);
}

void InventoryOrders::onEnterChanged() { m_tableView->initOrders(); }

void InventoryOrders::openEditor(const QString &condition) {
  Q_UNUSED(condition)
}

void InventoryOrders::refreshView() {
  m_tableView->setFocus();
  m_tableView->refreshView();
}

void InventoryOrders::updateOrder(int orderId) {
  if (orderId < 1) {
    return;
  }
  if (m_editor->openUpdateOrder(orderId)) {
    m_editor->setEnabled(true);
    m_stackedWidget->setCurrentWidget(m_editor);
  }
}

void InventoryOrders::createOrder(int customerId) {
  if (customerId < 1) {
    return;
  }
  m_editor->setEnabled(true);
  m_editor->openCreateOrder(customerId);
  m_stackedWidget->setCurrentWidget(m_editor);
}

void InventoryOrders::createOrder(const ProviderOrder &order) {
  ProviderOrder copy(order);
  int cid = copy.customerId();
  if (cid < 0) {
    emit sendPostMessage(Antiqua::ErrorStatus::WARNING,
                         tr("can't create order without costumer Id."));
    return;
  }
  /**< @deprecated mark to removed */
  if (cid < 1) {
    displayMessageBox(tr("Missing a valid costumer Id!"));
    return;
  }

  m_editor->setEnabled(true);
  m_editor->openCreateOrder(order);
  m_stackedWidget->setCurrentWidget(m_editor);
}

bool InventoryOrders::viewOrderById(int orderId) {
  if (m_editor->openUpdateOrder(orderId)) {
    m_editor->setEnabled(true);
    m_stackedWidget->setCurrentWidget(m_editor);
    return true;
  }

  displayMessageBox(
      tr("This Order has been completed and is no longer available."));
  return false;
}

bool InventoryOrders::addArticleToOrder(int articleId) {
  if (isEditorActive()) {
    m_editor->addArticleId(articleId);
    return true;
  }
  displayMessageBox(tr("There is no Order open!"));
  return false;
}

bool InventoryOrders::isEditorActive() {
  if (m_stackedWidget->currentWidget() == m_editor) {
    return m_editor->isEnabled();
  }
  return false;
}
