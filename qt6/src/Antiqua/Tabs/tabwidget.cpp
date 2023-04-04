// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabwidget.h"
#include "tabsbar.h"

#include <AntiquaInput>

namespace AntiquaCRM {

TabWidget::TabWidget(QWidget *parent) : QTabWidget{parent} {
  setContentsMargins(1, 1, 1, 1);
  m_tabBar = new TabsBar(this);
  setTabBar(m_tabBar);
}

const QIcon TabWidget::defaultIcon() {
  return AntiquaApplIcon("action-edit-document");
}

} // namespace AntiquaCRM
