// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "workspacetabbar.h"
#include "myicontheme.h"

#include <QAction>
#include <QBrush>
#include <QDebug>
#include <QMenu>
#include <QPalette>
#include <QStyleOption>

WorkspaceTabBar::WorkspaceTabBar(QWidget *parent) : QTabBar{parent} {
  setMovable(true);
  setExpanding(true);
  fontColorNormal = palette().text().color();
  fontColorActive = QColor(0, 170, 0);
  connect(this, SIGNAL(currentChanged(int)), SLOT(tabIndexChanged(int)));
}

void WorkspaceTabBar::tabInserted(int index) {
  setTabIcon(index, myIcon("tab"));
  QTabBar::tabInserted(index);
}

void WorkspaceTabBar::contextMenuEvent(QContextMenuEvent *ev) {
  index = tabAt(ev->pos());
  bool b = tabData(index).toBool();
  QMenu *m = new QMenu("TabAction", this);
  m->setEnabled(b);
  QAction *ac_close = m->addAction(myIcon("tab_remove"), tr("Close"));
  ac_close->setObjectName("ac_context_close_tab");
  connect(ac_close, SIGNAL(triggered()), this, SLOT(checkToClose()));
  m->exec(ev->globalPos());
  delete m;
}

void WorkspaceTabBar::wheelEvent(QWheelEvent *event) {
  // wird hier ignoriert!
}

void WorkspaceTabBar::tabIndexChanged(int index) {
  for (int i = 0; i < count(); i++) {
    if (i == index) {
      setTabTextColor(i, fontColorActive);
    } else {
      setTabTextColor(i, fontColorNormal);
    }
  }
}

void WorkspaceTabBar::checkToClose() {
  if (index >= 0)
    emit sendCloseTab(index);
}
