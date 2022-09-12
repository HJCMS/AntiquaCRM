// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerspageview.h"
#include "myicontheme.h"

#include <AntiquaInterface>
#include <QAction>
#include <QDebug>
#include <QMenu>

ProvidersTabBar::ProvidersTabBar(QTabBar *parent) : QTabBar{parent} {
  setMovable(true);
}

void ProvidersTabBar::checkToClose() {
  if (index >= 0)
    emit s_closeTab(index);
}

void ProvidersTabBar::tabInserted(int index) {
  setTabIcon(index, myIcon("edit_group"));
  QTabBar::tabInserted(index);
}

void ProvidersTabBar::contextMenuEvent(QContextMenuEvent *ev) {
  index = tabAt(ev->pos());
  QMenu *m = new QMenu(tr("Order Actions"), this);
  QAction *ac_close = m->addAction(myIcon("tab_remove"), tr("Close"));
  ac_close->setObjectName("ac_context_close_order");
  connect(ac_close, SIGNAL(triggered()), this, SLOT(checkToClose()));
  m->exec(ev->globalPos());
  delete m;
}

void ProvidersTabBar::setInfo(const QString &txt) {
  setToolTip(txt);
  info = txt;
}

const QString ProvidersTabBar::getInfo() { return info; }

ProvidersPageView::ProvidersPageView(QWidget *parent) : QTabWidget{parent} {
  m_tabBar = new ProvidersTabBar(tabBar());
  m_tabBar->setInfo(tr("Order tabs"));
  setTabBar(m_tabBar);

  connect(this, SIGNAL(tabCloseRequested(int)), SLOT(closeTabClicked(int)));
  connect(m_tabBar, SIGNAL(s_closeTab(int)), SLOT(closeTabClicked(int)));
  connect(this, SIGNAL(currentChanged(int)), this, SLOT(pageEntered(int)));
}

void ProvidersPageView::closeTabClicked(int index) {
  Antiqua::InterfaceWidget *tab =
      qobject_cast<Antiqua::InterfaceWidget *>(widget(index));
  if (tab != nullptr) {
    removeTab(index);
    tab->deleteLater();
    return;
  }
}

void ProvidersPageView::pageEntered(int) { emit orderPageChanged(); }

Antiqua::InterfaceWidget *ProvidersPageView::currentPage() {
  return reinterpret_cast<Antiqua::InterfaceWidget *>(currentWidget());
}

int ProvidersPageView::addPage(Antiqua::InterfaceWidget *aiw,
                               const QString &id) {
  if (aiw == nullptr || id.isEmpty())
    return -1;

  aiw->setObjectName(id);
  int index = addTab(aiw, myIcon("edit_group"), id);
  return index;
}

void ProvidersPageView::setTabDescription(int index, const QString &txt) {
  setTabToolTip(index, txt);
  setTabWhatsThis(index, txt);
}
