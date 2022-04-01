#include "version.h"
#include "applsettings.h"
#include "pmetatypes.h"
#include "inventorybooks.h"
#include "inventoryprints.h"
#include "workspace.h"
#include <QtCore/QDebug>

Workspace::Workspace(QWidget * parent) : QTabWidget{parent}
{
    setObjectName("WorkspaceTabWidget");
    setTabsClosable(true);
    setMovable(true);
    setUsesScrollButtons(true);

    m_cfg = new ApplSettings();

    m_inventoryBooks = new InventoryBooks(this);
    addTab(m_inventoryBooks,myIcon("windowlist"),tr("Books"));

    m_inventoryPrints = new InventoryPrints(this);
    addTab(m_inventoryPrints,myIcon("windowlist"),tr("Prints & Stitches"));

    connect(this,SIGNAL(tabCloseRequested(int)),SLOT(closeTabClicked(int)));
}

void Workspace::closeTabClicked(int index)
{
    qDebug() << __FUNCTION__ << index << "TODO Check State before close...";
}

