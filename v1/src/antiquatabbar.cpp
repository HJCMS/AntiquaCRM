// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquatabbar.h"

#include <QAction>
#include <QIcon>
#include <QMenu>

AntiquaTabBar::AntiquaTabBar(QWidget *parent, bool wheelEvents)
  : QTabBar{parent}, enableWheel{wheelEvents} {
  setMovable(true);
  setExpanding(true);
  connect(this, SIGNAL(currentChanged(int)), SLOT(tabIndexChanged(int)));
}

void AntiquaTabBar::tabInserted(int index) {
  if (tabIcon(index).isNull())
    setTabIcon(index, QIcon(":icons/tab.png"));

  QTabBar::tabInserted(index);
}

void AntiquaTabBar::contextMenuEvent(QContextMenuEvent *ev) {
  index = tabAt(ev->pos());
  bool b = tabData(index).toBool();
  QMenu *m = new QMenu("TabAction", this);
  m->setEnabled(b);
  QAction *ac_close = m->addAction(QIcon(":icons/tab_remove.png"), tr("Close"));
  ac_close->setObjectName("ac_context_close_tab");
  connect(ac_close, SIGNAL(triggered()), this, SLOT(checkToClose()));
  m->exec(ev->globalPos());
  delete m;
}

void AntiquaTabBar::wheelEvent(QWheelEvent *event) {
  if (enableWheel)
    QTabBar::wheelEvent(event);
}

void AntiquaTabBar::tabIndexChanged(int index) {
  // TODO
  emit sendTabChanged(index);
}

void AntiquaTabBar::checkToClose() {
  if (index >= 0)
    emit sendCloseTab(index);
}
