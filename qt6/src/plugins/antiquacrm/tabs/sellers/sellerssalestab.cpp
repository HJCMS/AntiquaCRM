// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sellerssalestab.h"
#include "sellerssalewidget.h"

SellersSalesTab::SellersSalesTab(QWidget *parent) : QTabWidget{parent} {
  m_tabsBar = new AntiquaCRM::TabsBar(tabBar());
  setTabBar(m_tabsBar);
  setTabPosition(QTabWidget::South);

  connect(m_tabsBar, SIGNAL(tabCloseRequested(int)), SLOT(setTabClosed(int)));
}

void SellersSalesTab::setTabClosed(int index) { removeTab(index); }

SellersSalesWidget *SellersSalesTab::currentPage() {
  return qobject_cast<SellersSalesWidget *>(currentWidget());
}

int SellersSalesTab::addPage(const QString &id, SellersSalesWidget *page) {
  if (page == nullptr || id.isEmpty())
    return -1;

  page->setObjectName(id);
  int index = addTab(page, AntiquaCRM::antiquaIcon("user-properties"), id);
  return index;
}
