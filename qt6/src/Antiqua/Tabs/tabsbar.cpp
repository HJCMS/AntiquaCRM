// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsbar.h"

#include <QAction>
#include <QDebug>
#include <QIcon>
#include <QLabel>
#include <QMenu>

namespace AntiquaCRM {

TabsBar::TabsBar(QWidget *parent, bool wheelEvents)
    : QTabBar{parent}, enableWheel{wheelEvents} {
  setMovable(true);
  setExpanding(true);
  setTabsClosable(true);
  connect(this, SIGNAL(currentChanged(int)), SLOT(setIndexChanged(int)));
}

void TabsBar::tabInserted(int index) {
  if (tabIcon(index).isNull())
    setTabIcon(index, QIcon("://icons/tab.png"));

  QTabBar::tabInserted(index);
}

void TabsBar::wheelEvent(QWheelEvent *event) {
  if (enableWheel)
    QTabBar::wheelEvent(event);
}

void TabsBar::setIndexChanged(int index) {
    emit sendTabChanged(index);
}

void TabsBar::setCheckToClose() {
  if (index >= 0)
    emit sendCloseTab(index);
}

void TabsBar::setTabCloseable(int index, bool closeable) {
  setTabData(index, closeable);
  if (!closeable)
    setTabButton(index, QTabBar::RightSide, new QLabel(this));
}

TabsBar::~TabsBar() {}

} // namespace AntiquaCRM
