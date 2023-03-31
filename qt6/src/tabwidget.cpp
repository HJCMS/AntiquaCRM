// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabwidget.h"

TabWidget::TabWidget(QMainWindow *parent) : QTabWidget{parent} {
  setObjectName("antiqua_ui_tabwidget");

  m_cfg = new AntiquaCRM::ASettings(this);
  QString _section("window_behavior/mouse_wheel_support");
  bool _mouse_wheel = m_cfg->value(_section, false).toBool();
  m_tabBar = new TabsBar(this, _mouse_wheel);
  setTabBar(m_tabBar);
}

TabWidget::~TabWidget() {}

const QMap<QString, QString> TabWidget::availableTabs() {
  QMap<QString, QString> m;
  // m.insert("books", tr("Books"));
  // m.insert("customers", tr("Customers"));
  // m.insert("orders", tr("Orders"));
  // m.insert("providers", tr("Providers"));
  // m.insert("printsstitches", tr("Prints and Stitches"));
  // m.insert("cdvinyl", tr("CD and Vinyl"));
  // m.insert("various", tr("Various"));
  return m;
}

bool TabWidget::beforeCloseAllTabs() {
  // TODO
  return true;
}
