#include "workspace.h"
#include "applsettings.h"
#include "inventorybooks.h"
#include "inventorycustomers.h"
#include "inventoryprints.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QMetaObject>

static const QIcon ptabIcon() { return myIcon("windowlist"); }

Workspace::Workspace(QWidget *parent) : QTabWidget{parent} {
  setObjectName("WorkspaceTabWidget");
  setMovable(true);
  setTabsClosable(true);
  setUsesScrollButtons(false);

  m_cfg = new ApplSettings();

  connect(this, SIGNAL(tabCloseRequested(int)), SLOT(closeTabClicked(int)));
}

void Workspace::addInventoryBooks(int index) {
  m_tabBooks = new InventoryBooks(index, this);
  insertTab(index, m_tabBooks, tr("Books"));
  setTabToolTip(index, tr("Book Inventory"));
  setTabIcon(index, myIcon("spreadsheet"));
}

void Workspace::addInventoryPrints(int index) {
  m_tabPrints = new InventoryPrints(index, this);
  insertTab(index, m_tabPrints, tr("Prints"));
  setTabToolTip(index, tr("Prints, Stitches and Photo inventory"));
  setTabIcon(index, myIcon("image"));
}

void Workspace::addInventoryCustomers(int index) {
  m_tabCustomers = new InventoryCustomers(index, this);
  insertTab(index, m_tabCustomers, tr("Customers"));
  setTabToolTip(index, tr("Customers inventory"));
  setTabIcon(index, myIcon("edit_group"));
}

void Workspace::closeTabClicked(int index) {
  bool closable = false;

  qDebug() << Q_FUNC_INFO << widget(index)->objectName();

  if (closable) {
    removeTab(index);
  }
}

void Workspace::tabRemoved(int index) {
  qDebug() << Q_FUNC_INFO << index << "TODO ...";
  // delete (widget(index));
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
  } else if (index == Tab::Customers) {
    if (indexOf(m_tabCustomers) < 0) {
      addInventoryCustomers(count() + 1);
    } else {
      setCurrentWidget(m_tabCustomers);
    }
  }
}
