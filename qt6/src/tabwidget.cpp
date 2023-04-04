// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabwidget.h"

#include <QApplication>

TabWidget::TabWidget(QMainWindow *parent) : QTabWidget{parent} {
  setObjectName("antiqua_ui_tabwidget");

  m_cfg = new AntiquaCRM::ASettings(this);
  bool _mouse_wheel_support =
      m_cfg->groupValue("window_behavior", "mouse_wheel_support", false)
          .toBool();
  m_tabBar = new AntiquaCRM::TabsBar(this, _mouse_wheel_support);
  setTabBar(m_tabBar);
}

TabWidget::~TabWidget() {}

void TabWidget::viewTab() {
  qDebug() << Q_FUNC_INFO << "TODO::open" << sender()->objectName();
}

bool TabWidget::beforeCloseAllTabs() {
  // TODO
  return true;
}
