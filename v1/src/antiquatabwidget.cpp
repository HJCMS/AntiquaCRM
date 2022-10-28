// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquatabwidget.h"
#include "antiquatabbar.h"
#include "tabbooks.h"
#include "tabviews.h"

AntiquaTabWidget::AntiquaTabWidget(QMainWindow *parent) : QTabWidget{parent} {
  setObjectName("window_tabwidget");

  AntiquaCRM::ASettings cfg(this);
  cfg.setObjectName("tabwidget_settings");

  bool mouseWheel = cfg.value("mouse_wheel_actions", false).toBool();
  m_tabBar = new AntiquaTabBar(this, mouseWheel);
  setTabBar(m_tabBar);

  connect(m_tabBar, SIGNAL(sendTabChanged(int)), SLOT(setTabChanged(int)));
  connect(m_tabBar, SIGNAL(sendCloseTab(int)), SLOT(setTabToClose(int)));
  connect(m_tabBar, SIGNAL(tabBarClicked(int)), SLOT(setTabToVisit(int)));
  connect(this, SIGNAL(tabCloseRequested(int)), SLOT(setTabToClose(int)));
}

Inventory *AntiquaTabWidget::tabWidget(int index) const {
  return qobject_cast<Inventory *>(widget(index));
}

void AntiquaTabWidget::addViewsTab(const QString &name) {
  int index = indexOf(m_views);
  if (index >= 0) {
    m_views->createSearchQuery(name);
    setCurrentIndex(index);
    return;
  }

  m_views = new TabViews(this);
  m_views->createSearchQuery(name);
  int c = (m_tabBar->count() + 1);
  index = insertTab(c, m_views, m_views->windowIcon(), m_views->windowTitle());
  m_tabBar->setTabCloseable(index, m_views->isClosable());
  setCurrentIndex(index);
}

void AntiquaTabWidget::setTabChanged(int index) {
  Inventory *m_tab = tabWidget(index);
  if (m_tab != nullptr)
    m_tab->onEnterChanged();
}

void AntiquaTabWidget::setTabToClose(int index) {
  Inventory *m_tab = tabWidget(index);
  if (m_tab != nullptr && m_tab->isClosable()) {
    if (!m_tab->isWindowModified()) {
      removeTab(index);
      return;
    }
    qDebug() << tr("Cant close this tab, unsafed changes!");
  }
}

void AntiquaTabWidget::setTabToVisit(int index) {
  Inventory *m_tab = tabWidget(index);
  if (m_tab != nullptr)
    m_tab->onEnterChanged();
}

bool AntiquaTabWidget::loadDefaultTabs() {
  m_books = new TabBooks(this);
  int i = insertTab(0, m_books, m_books->windowIcon(), m_books->windowTitle());
  m_tabBar->setTabCloseable(i, m_books->isClosable());
  return true;
}
