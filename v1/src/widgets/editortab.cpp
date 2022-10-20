// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editortab.h"

EditorTab::EditorTab(QWidget *parent) : QTabWidget{parent} {
  setFixedHeight(180);
  setContentsMargins(1, 1, 1, 1);
  m_tabBar = new EditorTabBar(this);
  setTabBar(m_tabBar);
}

const QIcon EditorTab::defaultIcon() { return QIcon(":icons/edit.png"); }
