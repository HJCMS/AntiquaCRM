// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "workspace.h"
#include "antiqua_global.h"
#include "inventorybooks.h"
#include "inventorycostumers.h"
#include "inventoryorders.h"
#include "inventoryprints.h"
#include "myicontheme.h"

#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QMetaObject>
#include <QTabBar>

WorkspaceTabBar::WorkspaceTabBar(QWidget *parent) : QTabBar{parent} {
  setMovable(true);
}

void WorkspaceTabBar::checkToClose() {
  if (index >= 0)
    emit s_closeTab(index);
}

void WorkspaceTabBar::tabInserted(int index) {
  setTabIcon(index, myIcon("tab"));
  QTabBar::tabInserted(index);
}

void WorkspaceTabBar::contextMenuEvent(QContextMenuEvent *ev) {
  index = tabAt(ev->pos());
  bool b = tabData(index).toBool();
  QMenu *m = new QMenu("TabAction", this);
  m->setEnabled(b);
  QAction *ac_close = m->addAction(myIcon("tab_remove"), tr("Close"));
  ac_close->setObjectName("ac_context_close_tab");
  connect(ac_close, SIGNAL(triggered()), this, SLOT(checkToClose()));
  m->exec(ev->globalPos());
  delete m;
}

Workspace::Workspace(QWidget *parent) : QTabWidget{parent} {
  setObjectName("WorkspaceTabWidget");
  setTabsClosable(false);
  setUsesScrollButtons(false);

  m_tabBar = new WorkspaceTabBar(tabBar());
  setTabBar(m_tabBar);

  connect(this, SIGNAL(tabCloseRequested(int)), SLOT(closeTabClicked(int)));
  connect(m_tabBar, SIGNAL(s_closeTab(int)), SLOT(closeTabClicked(int)));
}

int Workspace::addInventoryBooks(int index) {
  m_tabBooks = new InventoryBooks(this);
  connect(m_tabBooks, SIGNAL(s_addArticleOrder(int)), this,
          SLOT(addArticleOrder(int)));
  connect(m_tabBooks, SIGNAL(s_postMessage(const QString &)), this,
          SIGNAL(s_postMessage(const QString &)));
  int i = insertTab(index, m_tabBooks, tr("Books"));
  m_tabBar->setTabData(i, m_tabBooks->isClosable());
  setTabToolTip(i, tr("Book Inventory"));
  setTabIcon(i, myIcon("toggle_log"));
  return i;
}

int Workspace::addInventoryPrints(int index) {
  m_tabPrints = new InventoryPrints(this);
  connect(m_tabPrints, SIGNAL(s_addArticleOrder(int)), this,
          SLOT(addArticleOrder(int)));
  connect(m_tabPrints, SIGNAL(s_postMessage(const QString &)), this,
          SIGNAL(s_postMessage(const QString &)));
  int i = insertTab(index, m_tabPrints, tr("Prints"));
  m_tabBar->setTabData(i, m_tabPrints->isClosable());
  setTabToolTip(i, tr("Prints, Stitches and Photo inventory"));
  setTabIcon(i, myIcon("image"));
  return i;
}

int Workspace::addInventoryCostumers(int index) {
  m_tabCostumers = new InventoryCostumers(this);
  connect(m_tabCostumers, SIGNAL(s_createOrder(int)), this,
          SLOT(createOrder(int)));
  connect(m_tabCostumers, SIGNAL(s_postMessage(const QString &)), this,
          SIGNAL(s_postMessage(const QString &)));
  int i = insertTab(index, m_tabCostumers, tr("Costumers"));
  m_tabBar->setTabData(i, m_tabCostumers->isClosable());
  setTabToolTip(i, tr("Costumers inventory"));
  setTabIcon(i, myIcon("edit_group"));
  return i;
}

int Workspace::addInventoryOrders(int index) {
  m_tabOrders = new InventoryOrders(this);
  connect(m_tabOrders, SIGNAL(s_postMessage(const QString &)), this,
          SIGNAL(s_postMessage(const QString &)));
  int i = insertTab(index, m_tabOrders, tr("Orders"));
  m_tabBar->setTabData(i, m_tabOrders->isClosable());
  setTabToolTip(i, tr("Order Inventory"));
  setTabIcon(i, myIcon("autostart"));
  return i;
}

void Workspace::closeTabClicked(int index) {
  Inventory *tab = qobject_cast<Inventory *>(widget(index));
  if (tab != nullptr && tab->isClosable()) {
    if (!tab->isWindowModified()) {
      removeTab(index);
      return;
    }
    emit s_postMessage(tr("Cant close this tab, unsafed changes!"));
  }
}

void Workspace::createOrder(int costumerId) {
  if (costumerId > 0 && (m_tabOrders != nullptr)) {
    m_tabOrders->createOrder(costumerId);
    setCurrentWidget(m_tabOrders);
  } else {
    emit s_postMessage(tr("Order tab isn't open!"));
  }
}

void Workspace::addArticleOrder(int articleId) {
  if (articleId > 0 && (m_tabOrders != nullptr)) {
    bool b = m_tabOrders->addArticleToOrder(articleId);
    if (b) {
      setCurrentWidget(m_tabOrders);
    }
  } else {
    emit s_postMessage(tr("Order tab isn't open!"));
  }
}

void Workspace::tabRemoved(int index) {
  if (!isTabEnabled(index))
    delete widget(index);
}

void Workspace::openTab(int index) {
  int i = 0;
  if (index == Tab::Books) {
    if (indexOf(m_tabBooks) < 0) {
      i = addInventoryBooks(count() + 1);
    } else {
      setCurrentWidget(m_tabBooks);
      return;
    }
  } else if (index == Tab::Prints) {
    if (indexOf(m_tabPrints) < 0) {
      i = addInventoryPrints(count() + 1);
    } else {
      setCurrentWidget(m_tabPrints);
      return;
    }
  } else if (index == Tab::Costumers) {
    if (indexOf(m_tabCostumers) < 0) {
      i = addInventoryCostumers(count() + 1);
    } else {
      setCurrentWidget(m_tabCostumers);
      return;
    }
  } else if (index == Tab::Orders) {
    if (indexOf(m_tabOrders) < 0) {
      i = addInventoryOrders(count() + 1);
    } else {
      setCurrentWidget(m_tabOrders);
      return;
    }
  }
  setCurrentIndex(i);
}
