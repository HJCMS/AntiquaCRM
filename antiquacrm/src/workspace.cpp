#include "workspace.h"
#include "inventorybooks.h"
#include "inventorycostumers.h"
#include "inventoryprints.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QMetaObject>
#include <QtWidgets/QTabBar>

Workspace::Workspace(QWidget *parent) : QTabWidget{parent} {
  setObjectName("WorkspaceTabWidget");
  setMovable(true);
  setTabsClosable(true);
  setUsesScrollButtons(false);
#ifndef Q_OS_UNIX
  setDocumentMode(true);
  tabBar()->setExpanding(true);
#endif

  connect(this, SIGNAL(tabCloseRequested(int)), SLOT(closeTabClicked(int)));
}

void Workspace::addInventoryBooks(int index) {
  m_tabBooks = new InventoryBooks(this);
  int i = insertTab(index, m_tabBooks, tr("Books"));
  setTabToolTip(i, tr("Book Inventory"));
  setTabIcon(i, myIcon("spreadsheet"));
}

void Workspace::addInventoryPrints(int index) {
  m_tabPrints = new InventoryPrints(this);
  int i = insertTab(index, m_tabPrints, tr("Prints"));
  setTabToolTip(i, tr("Prints, Stitches and Photo inventory"));
  setTabIcon(i, myIcon("image"));
}

void Workspace::addInventoryCostumers(int index) {
  m_tabCostumers = new InventoryCostumers(this);
  int i = insertTab(index, m_tabCostumers, tr("Costumers"));
  setTabToolTip(i, tr("Costumers inventory"));
  setTabIcon(i, myIcon("edit_group"));
}

void Workspace::closeTabClicked(int index) {
  bool closable = false;
  QMetaObject::invokeMethod(widget(index), "isClosable", Qt::DirectConnection,
                            Q_RETURN_ARG(bool, closable));

  if (closable) {
    removeTab(index);
    return;
  }
  emit s_postMessage(tr("Cant close this tab, unsafed changes!"));
}

void Workspace::tabRemoved(int index) {
  if (!isTabEnabled(index))
    delete widget(index);
}

void Workspace::openTab(int index) {
  if (index == Tab::Books) {
    if (indexOf(m_tabBooks) < 0) {
      addInventoryBooks(count() + 1);
    } else {
      setCurrentWidget(m_tabBooks);
    }
  } else if (index == Tab::Prints) {
    if (indexOf(m_tabPrints) < 0) {
      addInventoryPrints(count() + 1);
    } else {
      setCurrentWidget(m_tabPrints);
    }
  } else if (index == Tab::Costumers) {
    if (indexOf(m_tabCostumers) < 0) {
      addInventoryCostumers(count() + 1);
    } else {
      setCurrentWidget(m_tabCostumers);
    }
  }
}
