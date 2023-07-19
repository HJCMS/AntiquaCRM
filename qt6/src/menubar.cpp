// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "menubar.h"
#ifndef ANTIQUACRM_UTILS_DIALOGS_H
#include "antiquadialogs.h"
#endif

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

  // BEGIN::Configurations
  m_configMenu = addMenu(tr("Configuration"));

  // BEGIN::Dialogs
  m_dialogMenu = m_configMenu->addMenu(tr("Dialogs"));
  m_dialogMenu->setIcon(AntiquaCRM::antiquaIcon("configure"));
  // KeywordsDialog
  QAction *ac_ksd = m_dialogMenu->addAction(tr("Keywords"));
  ac_ksd->setIcon(AntiquaCRM::antiquaIcon("configure"));
  connect(ac_ksd, SIGNAL(triggered()), SLOT(openKeywordsDialog()));
  // DeliveryDialog
  QAction *ac_ddy = m_dialogMenu->addAction(tr("Delivery"));
  ac_ddy->setIcon(AntiquaCRM::antiquaIcon("configure"));
  connect(ac_ddy, SIGNAL(triggered()), SLOT(openDeliveryDialog()));
  // CompanyDialog
  QAction *ac_dcy = m_dialogMenu->addAction(tr("Company"));
  ac_dcy->setIcon(AntiquaCRM::antiquaIcon("configure"));
  connect(ac_dcy, SIGNAL(triggered()), SLOT(openCompanyDialog()));
  // StoragesDialog
  QAction *ac_dse = m_dialogMenu->addAction(tr("Storage"));
  ac_dse->setIcon(AntiquaCRM::antiquaIcon("configure"));
  connect(ac_dse, SIGNAL(triggered()), SLOT(openStoragesDialog()));
  // END::Dialogs

  // BEGIN::SystemConfig
  m_configMenu->addSeparator();
  QAction *ac_cfg = m_configMenu->addAction(tr("Configuration"));
  ac_cfg->setIcon(AntiquaCRM::antiquaIcon("configure"));
  connect(ac_cfg, SIGNAL(triggered()), SLOT(openConfigDialog()));
  // END::SystemConfig
  // END::Configurations

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

void MenuBar::openConfigDialog() {
  ConfigDialog *d = new ConfigDialog(this);
  d->exec();
}

void MenuBar::openDeliveryDialog() {
  DeliveryDialog *d = new DeliveryDialog(this);
  d->exec();
}

void MenuBar::openCompanyDialog() {
  CompanyDialog *d = new CompanyDialog(this);
  d->exec();
}

void MenuBar::openKeywordsDialog() {
  KeywordsDialog *d = new KeywordsDialog(this);
  d->exec();
}

void MenuBar::openStoragesDialog() {
  StoragesDialog *d = new StoragesDialog(this);
  d->exec();
}
