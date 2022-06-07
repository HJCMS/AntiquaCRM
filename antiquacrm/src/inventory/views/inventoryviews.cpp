// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventoryviews.h"
#include "viewstable.h"
#include "viewstoolbar.h"

#include <QtCore>
#include <QtWidgets>

InventoryViews::InventoryViews(QWidget *parent) : Inventory{parent} {
  setObjectName("inventory_views");
  setClosable(true);

  currentView = QString();

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_toolBar = new ViewsToolBar(this);
  layout->insertWidget(0, m_toolBar);

  m_tableView = new ViewsTable(this);
  layout->insertWidget(1, m_tableView);
  layout->setStretch(1, 1);

  setLayout(layout);

  connect(m_toolBar, SIGNAL(s_queryTableView(const QString &)), m_tableView,
          SLOT(sqlQuery(const QString &)));
  connect(m_tableView, SIGNAL(s_articleSelected(int, const QString &)), this,
          SLOT(columnEditClicked(int, const QString &)));
}

void InventoryViews::searchConvert(const QString &) { /* not used */
}

void InventoryViews::searchConvert() { /* not used */
}

void InventoryViews::openTableView() { /* not used */
}

void InventoryViews::columnEditClicked(int id, const QString &section) {
  if (id < 1)
    return;

  if (section == "bookeditor") {
    emit s_editBookEntry(id);
  } else if (section == "printsditor") {
    emit s_editPrintsEntry(id);
  } else if (section == "customereditor") {
    emit s_editCustomerEntry(id);
  }
}

void InventoryViews::onEnterChanged() { /* not used */
}
