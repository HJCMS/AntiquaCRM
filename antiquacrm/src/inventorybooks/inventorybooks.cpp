// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "version.h"
#include "pmetatypes.h"
#include "inventorybooks.h"
#include "searchbookbar.h"
#include "bookstableview.h"

#include <QtCore/QDebug>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLayout>

InventoryBooks::InventoryBooks(QWidget *parent) : QWidget{parent}
{
  setObjectName("InventoryBooks");

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_toolBar = new SearchBookBar(this);
  layout->addWidget(m_toolBar);

  m_tableView = new  BooksTableView(this);
  layout->addWidget(m_tableView);

  setLayout(layout);

  connect(m_toolBar, SIGNAL (s_sqlQueryChanged(const SearchStatement &)),
          m_tableView, SLOT (queryStatement(const SearchStatement &)));
}

