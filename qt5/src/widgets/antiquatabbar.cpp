// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquatabbar.h"

#include <QAction>
#include <QDebug>
#include <QIcon>
#include <QLabel>
#include <QMenu>

AntiquaTabBar::AntiquaTabBar(QWidget *parent, bool wheelEvents)
    : QTabBar{parent}, enableWheel{wheelEvents} {
  setMovable(true);
  setExpanding(true);
  setTabsClosable(true);
  connect(this, SIGNAL(currentChanged(int)), SLOT(tabIndexChanged(int)));
}

void AntiquaTabBar::tabInserted(int index) {
  if (tabIcon(index).isNull())
    setTabIcon(index, QIcon(":icons/tab.png"));

  QTabBar::tabInserted(index);
}

void AntiquaTabBar::wheelEvent(QWheelEvent *event) {
  event->setAccepted(enableWheel);
  if (enableWheel)
    QTabBar::wheelEvent(event);
}

void AntiquaTabBar::tabIndexChanged(int index) { emit sendTabChanged(index); }

void AntiquaTabBar::checkToClose() {
  if (index >= 0)
    emit sendCloseTab(index);
}

void AntiquaTabBar::setTabCloseable(int index, bool closeable) {
  setTabData(index, closeable);
  if (!closeable)
    setTabButton(index, QTabBar::RightSide, new QLabel(this));
}
