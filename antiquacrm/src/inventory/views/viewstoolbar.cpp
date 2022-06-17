// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "viewstoolbar.h"
#include "myicontheme.h"

#include <QSizePolicy>

ViewsToolBar::ViewsToolBar(QWidget *parent) : QToolBar{parent} {
  setObjectName("inventory_views_toolbar");
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  QSizePolicy sp(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);

  m_comboBox = new QComboBox(this);
  int i = 0;
  m_comboBox->insertItem(i++, tr("select your operation"), QVariant());
  m_comboBox->insertItem(i++, tr("Show me all Bookentries without Pictures"),
                         QString("view_no_book_picture"));
  m_comboBox->insertItem(i++,
                         tr("Show me all Prints and Stitches without Pictures"),
                         QString("view_no_print_picture"));
  m_comboBox->insertItem(i++,
                         tr("All categories grouped by keyword plus total "
                            "price of these categories."),
                         QString("view_group_by_keywords"));
  m_comboBox->insertItem(i++,
                         tr("Book count, total price and price average."),
                         QString("view_all_with_duration"));
  m_comboBox->setSizePolicy(sp);
  addWidget(m_comboBox);

  addSeparator();

  m_query = addAction(tr("start query"));
  m_query->setIcon(myIcon("search"));

  connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(operationChanged(int)));
  connect(m_query, SIGNAL(triggered()), this, SLOT(refresh()));
}

void ViewsToolBar::refresh() {
  int index = m_comboBox->currentIndex();
  if (index > 0)
    operationChanged(index);
}

void ViewsToolBar::operationChanged(int index) {
  QVariant data = m_comboBox->itemData(index, Qt::UserRole);
  if (!data.isNull())
    emit s_queryTableView(data.toString());
}
