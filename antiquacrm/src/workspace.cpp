#include "version.h"
#include "applsettings.h"
#include "inventorybooks.h"
#include "inventoryprints.h"
#include "inventorycustomers.h"
#include "workspace.h"
#include <QtCore/QDebug>

static const QIcon ptabIcon()
{
    return myIcon("windowlist");
}

Workspace::Workspace(QWidget * parent) : QTabWidget{parent}
{
    setObjectName("WorkspaceTabWidget");
    setMovable(false);
    setTabsClosable(true);
    setUsesScrollButtons(true);

    m_cfg = new ApplSettings();

    tabIndex = 0;
    m_inventoryBooks = new InventoryBooks(tabIndex,this);
    insertTab(tabIndex,m_inventoryBooks,tr("Books"));
    setTabToolTip(tabIndex,tr("Book Inventory"));
    setTabIcon(tabIndex,myIcon("spreadsheet"));

    m_inventoryPrints = new InventoryPrints(++tabIndex,this);
    insertTab(tabIndex,m_inventoryPrints,tr("Prints"));
    setTabToolTip(tabIndex,tr("Prints, Stitches and Photo inventory"));
    setTabIcon(tabIndex,myIcon("image"));

    connect(this,SIGNAL(tabCloseRequested(int)),SLOT(closeTabClicked(int)));
}

void Workspace::addCustomersTab()
{
    qDebug() << __FUNCTION__ << ++tabIndex;
    return;
    m_inventoryCustomers = new InventoryCustomers(tabIndex,this);
    insertTab(tabIndex,m_inventoryCustomers,tr("Customers"));
    setTabToolTip(tabIndex,tr("Customers inventory"));
    setTabIcon(tabIndex,myIcon("edit_group"));
}

void Workspace::closeTabClicked(int index)
{
    qDebug() << __FUNCTION__ << index << "TODO Check State before close...";
    // removeTab(index);
}
