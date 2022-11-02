// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerstabwidget.h"

#include <ASettings>
#include <QIcon>

CustomersTabWidget::CustomersTabWidget(QWidget *parent) : QTabWidget{parent} {
  setObjectName("customers_tab_widget");

  AntiquaCRM::ASettings cfg(this);
  cfg.setObjectName("tabwidget_settings");

  bool mouseWheel = cfg.value("mouse_wheel_actions", false).toBool();
  m_tabBar = new AntiquaTabBar(this, mouseWheel);
  setTabBar(m_tabBar);

  //  connect(m_tabBar, SIGNAL(sendTabChanged(int)), SLOT(setTabChanged(int)));
  //  connect(m_tabBar, SIGNAL(sendCloseTab(int)), SLOT(setTabToClose(int)));
  //  connect(m_tabBar, SIGNAL(tabBarClicked(int)), SLOT(setTabToVisit(int)));
  //  connect(this, SIGNAL(tabCloseRequested(int)), SLOT(setTabToClose(int)));
}

int CustomersTabWidget::placeTab(QWidget *page, const QString &title) {
  QIcon icon(":icons/groups.png");
  int index = addTab(page, icon, title);
  m_tabBar->setTabCloseable(index, false);
  return index;
}
