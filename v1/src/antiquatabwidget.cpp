// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquatabwidget.h"
#include "tabbooks.h"

AntiquaTabWidget::AntiquaTabWidget(QMainWindow *parent) : QTabWidget{parent} {
  setObjectName("tab_widgets_main");

  connect(tabBar(), SIGNAL(currentChanged(int)), SLOT(tabChanged(int)));
}

void AntiquaTabWidget::tabChanged(int index) {
  reinterpret_cast<Inventory *>(widget(index))->onEnterChanged();
}

bool AntiquaTabWidget::loadDefaultTabs() {
  m_books = new TabBooks(this);
  insertTab(0, m_books, m_books->windowIcon(), m_books->windowTitle());
  return true;
}
