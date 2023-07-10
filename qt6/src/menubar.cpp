// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "menubar.h"
#include "dialogs/config/configdialog.h"

#include <AntiquaTabs>
#include <AntiquaWidgets>

MenuBar::MenuBar(QWidget *parent) : QMenuBar{parent} {
  setObjectName("antiqua_ui_menubar");
  setNativeMenuBar(true);

  m_applMenu = addMenu("Antiqua CRM");
  QAction *ac_quit = m_applMenu->addAction(tr("Application Quit"));
  ac_quit->setIcon(AntiquaCRM::antiquaIcon("action-quit"));
  ac_quit->setShortcut(QKeySequence::Quit);
  connect(ac_quit, SIGNAL(triggered()), SIGNAL(sendApplicationQuit()));
  addMenu(m_applMenu);

  m_viewsMenu = new AntiquaCRM::TabsMenu(this);
  addMenu(m_viewsMenu);

  m_configMenu = new QMenu(tr("Configuration"), this);

  QAction *ac_cfg = m_configMenu->addAction(tr("Configuration"));
  ac_cfg->setIcon(AntiquaCRM::antiquaIcon("configure"));
  connect(ac_cfg, SIGNAL(triggered()), SLOT(openConfiguration()));

  addMenu(m_configMenu);

  m_aboutMenu = new QMenu(tr("About"), this);
  addMenu(m_aboutMenu);
}

void MenuBar::setViewsMenu(AntiquaCRM::TabsMenu *menu) {
  if (menu == nullptr)
    return;

  m_viewsMenu->addMenu(menu);
}

const QIcon MenuBar::tabIcon(const QString &name) {
  QString _name = name.isEmpty() ? "action-tab" : name;
  return AntiquaCRM::antiquaIcon(_name);
}

void MenuBar::openConfiguration() {
  ConfigDialog *d = new ConfigDialog(this);
  d->exec();
}
