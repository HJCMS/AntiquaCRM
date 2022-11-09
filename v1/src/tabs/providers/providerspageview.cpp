// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerspageview.h"
#include "providersorderpage.h"

#include <QAction>
#include <QDebug>
#include <QIcon>
#include <QMenu>

ProvidersPageView::ProvidersPageView(QWidget *parent) : QTabWidget{parent} {
  m_tabBar = new AntiquaTabBar(tabBar());
  setTabBar(m_tabBar);

  connect(this, SIGNAL(tabCloseRequested(int)), SLOT(closeTabClicked(int)));
  connect(m_tabBar, SIGNAL(sendCloseTab(int)), SLOT(closeTabClicked(int)));
  connect(this, SIGNAL(currentChanged(int)), this, SLOT(pageEntered(int)));
}

void ProvidersPageView::closeTabClicked(int index) {
  QWidget *tab = qobject_cast<QWidget *>(widget(index));
  if (tab != nullptr) {
    removeTab(index);
    tab->deleteLater();
    return;
  }
}

void ProvidersPageView::pageEntered(int) { emit sendPageChanged(); }

ProvidersOrderPage *ProvidersPageView::currentPage() {
  return reinterpret_cast<ProvidersOrderPage *>(currentWidget());
}

int ProvidersPageView::addPage(ProvidersOrderPage *aiw, const QString &id) {
  if (aiw == nullptr || id.isEmpty())
    return -1;

  aiw->setObjectName(id);
  int index = addTab(aiw, QIcon("://icons/groups.png"), id);
  return index;
}

void ProvidersPageView::addFixedTab(QWidget *tab, const QString &title) {
  if (tab == nullptr || title.isEmpty())
    return;

  tab->setObjectName(title.toLower());
  int index = addTab(tab, QIcon("://icons/groups.png"), title);
  m_tabBar->setTabCloseable(index, false);
}

void ProvidersPageView::setTabDescription(int index, const QString &txt) {
  setTabToolTip(index, txt);
  setTabWhatsThis(index, txt);
}
