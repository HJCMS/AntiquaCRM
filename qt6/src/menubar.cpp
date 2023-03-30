// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "menubar.h"

MenuBar::MenuBar(QWidget *parent) : QMenuBar{parent} {
  setObjectName("antiqua_ui_menubar");
  setNativeMenuBar(true);
  setStyleSheet("");

  m_applMenu = addMenu("Antiqua CRM");
  addMenu(m_applMenu);

  m_viewsMenu = new QMenu(tr("Views"), this);
  addMenu(m_viewsMenu);

  m_configMenu = new QMenu(tr("Configuration"), this);
  addMenu(m_configMenu);

  m_aboutMenu = new QMenu(tr("About"), this);
  addMenu(m_aboutMenu);
}
