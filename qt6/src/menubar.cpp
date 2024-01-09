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
  ac_quit->setIcon(tabIcon("action-quit"));
  ac_quit->setShortcut(QKeySequence::Quit);
  connect(ac_quit, SIGNAL(triggered()), SIGNAL(sendApplicationQuit()));
  addMenu(m_applMenu);

  // BEGIN::Views
  tabsMenu = new AntiquaCRM::TabsMenu(this);
  tabsMenu->setTitle(tr("Views"));
  addMenu(tabsMenu);
  // END::Views

  // BEGIN::Configurations
  m_configMenu = addMenu(tr("Configuration"));

  const QIcon _conf_icon = tabIcon("configure");

  // BEGIN::Dialogs
  m_dialogMenu = m_configMenu->addMenu(tr("Dialogs"));
  m_dialogMenu->setIcon(_conf_icon);
  // KeywordsDialog
  QAction *ac_ksd = m_dialogMenu->addAction(tr("Keywords"));
  ac_ksd->setIcon(_conf_icon);
  connect(ac_ksd, SIGNAL(triggered()), SLOT(openKeywordsDialog()));
  // CompanyDialog
  QAction *ac_dcy = m_dialogMenu->addAction(tr("Company"));
  ac_dcy->setIcon(_conf_icon);
  connect(ac_dcy, SIGNAL(triggered()), SLOT(openCompanyDialog()));
  // DeliveryDialog
  QAction *ac_ddy = m_dialogMenu->addAction(tr("Delivery"));
  ac_ddy->setIcon(_conf_icon);
  connect(ac_ddy, SIGNAL(triggered()), SLOT(openDeliveryDialog()));
  // DesignationsDialog
  QAction *ac_dsn = m_dialogMenu->addAction(tr("Designation"));
  ac_dsn->setIcon(_conf_icon);
  connect(ac_dsn, SIGNAL(triggered()), SLOT(openDesignationDialog()));
  // StoragesDialog
  QAction *ac_dse = m_dialogMenu->addAction(tr("Storage"));
  ac_dse->setIcon(_conf_icon);
  connect(ac_dse, SIGNAL(triggered()), SLOT(openStoragesDialog()));
  // PublisherDialog
  QAction *ac_dpl = m_dialogMenu->addAction(tr("Book publisher"));
  ac_dpl->setIcon(_conf_icon);
  connect(ac_dpl, SIGNAL(triggered()), SLOT(openPublisherDialog()));
  // END::Dialogs

  // BEGIN::SystemConfig
  m_configMenu->addSeparator();
  QAction *ac_cfg = m_configMenu->addAction(tr("Configuration"));
  ac_cfg->setIcon(_conf_icon);
  connect(ac_cfg, SIGNAL(triggered()), SLOT(openConfigDialog()));
  // END::SystemConfig
  // END::Configurations

  m_aboutMenu = new AboutMenu(this);
  addMenu(m_aboutMenu);
}

const QIcon MenuBar::tabIcon(const QString &name) {
  return AntiquaCRM::antiquaIcon(name);
}

void MenuBar::openConfigDialog() {
  ConfigDialog *d = new ConfigDialog(this);
  d->exec();
  d->deleteLater();
}

void MenuBar::openDeliveryDialog() {
  DeliveryDialog *d = new DeliveryDialog(this);
  d->exec();
  d->deleteLater();
}

void MenuBar::openCompanyDialog() {
  CompanyDialog *d = new CompanyDialog(this);
  d->exec();
  d->deleteLater();
}

void MenuBar::openDesignationDialog() {
  DesignationsDialog *d = new DesignationsDialog(this);
  d->exec();
  d->deleteLater();
}

void MenuBar::openKeywordsDialog() {
  KeywordsDialog *d = new KeywordsDialog(this);
  d->exec();
  d->deleteLater();
}

void MenuBar::openStoragesDialog() {
  StoragesDialog *d = new StoragesDialog(this);
  d->exec();
  d->deleteLater();
}

void MenuBar::openPublisherDialog() {
  PublisherDialog *d = new PublisherDialog(this);
  d->exec();
  d->deleteLater();
}
