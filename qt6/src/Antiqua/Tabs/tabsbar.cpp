// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsbar.h"
#include "antiquaicon.h"

#include <QIcon>
#include <QLabel>

namespace AntiquaCRM {

TabsBar::TabsBar(QWidget *parent, bool wheelEvents)
    : QTabBar{parent}, p_wheel_support{wheelEvents} {
  setMovable(true);
  setExpanding(true);
  setTabsClosable(true);
}

void TabsBar::tabInserted(int index) {
  if (tabIcon(index).isNull())
    setTabIcon(index, AntiquaCRM::antiquaIcon("tab-new"));

  QTabBar::tabInserted(index);
}

void TabsBar::wheelEvent(QWheelEvent *event) {
  if (p_wheel_support)
    QTabBar::wheelEvent(event);
}

void TabsBar::setTabCloseable(int index, bool closeable) {
  setTabData(index, closeable);
  if (!closeable)
    setTabButton(index, QTabBar::RightSide, new QLabel(this));
}

bool TabsBar::isTabCloseable(int index) {
  // @ref setTabCloseable
  return tabData(index).toBool();
}

} // namespace AntiquaCRM
