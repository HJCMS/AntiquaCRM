// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "menubar.h"

#include <AntiquaWidgets>

MenuBar::MenuBar(QWidget *parent) : QMenuBar{parent} {
  setObjectName("antiqua_ui_menubar");
  setNativeMenuBar(true);
  setStyleSheet("");

  m_applMenu = addMenu("Antiqua CRM");
  QAction *ac_quit = m_applMenu->addAction(tr("Application Quit"));
  ac_quit->setIcon(AntiquaCRM::AntiquaApplIcon("action-quit"));
  ac_quit->setShortcut(QKeySequence::Quit);
  connect(ac_quit, SIGNAL(triggered()), this, SIGNAL(sendApplicationQuit()));
  addMenu(m_applMenu);

  m_viewsMenu = new QMenu(tr("Views"), this);
  addMenu(m_viewsMenu);

  m_configMenu = new QMenu(tr("Configuration"), this);
  addMenu(m_configMenu);

  m_aboutMenu = new QMenu(tr("About"), this);
  addMenu(m_aboutMenu);
}

void MenuBar::setViewsMenu(QMenu *menu) {
  if (menu == nullptr)
    return;

  m_viewsMenu->addMenu(menu);
}

const QIcon MenuBar::tabIcon(const QString &name) {
  QString _name = name.isEmpty() ? "action-tab" : name;
  return AntiquaCRM::AntiquaApplIcon(_name);
}
