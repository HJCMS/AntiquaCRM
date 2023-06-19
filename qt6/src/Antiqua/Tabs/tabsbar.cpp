// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsbar.h"
#include "ainputwidget.h"

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
  connect(this, SIGNAL(currentChanged(int)), SLOT(setChangedIndex(int)));
}

void TabsBar::tabInserted(int index) {
  if (tabIcon(index).isNull())
    setTabIcon(index, AntiquaCRM::AntiquaApplIcon("tab-new"));

  QTabBar::tabInserted(index);
}

void TabsBar::wheelEvent(QWheelEvent *event) {
  if (enableWheel)
    QTabBar::wheelEvent(event);
}

void TabsBar::setChangedIndex(int index) {
  changedIndex = index;
  emit sendTabChanged(index);
}

void TabsBar::setCheckToClose() {
  if (changedIndex >= 0)
    emit sendCloseTab(changedIndex);
}

int TabsBar::getChangedIndex() { return changedIndex; }

void TabsBar::setTabCloseable(int index, bool closeable) {
  setTabData(index, closeable);
  if (!closeable)
    setTabButton(index, QTabBar::RightSide, new QLabel(this));
}

TabsBar::~TabsBar() {
  // restore
  changedIndex = -1;
}

} // namespace AntiquaCRM
