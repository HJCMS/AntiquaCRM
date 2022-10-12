// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquamenubar.h"

AntiquaMenuBar::AntiquaMenuBar(QMainWindow *parent)
    : QMenuBar{parent}, defaultIcon(QString(":icons/antiqua.png")) {
  setNativeMenuBar(true);
  setStyleSheet("");

  m_applMenu = addMenu("Antiqua CRM");
  addApplMenu();

  m_viewsMenu = addMenu(tr("Views"));
  addViewsMenu();

  m_configMenu = addMenu(tr("Configuration"));
  addConfigMenu();

  m_aboutMenu = addMenu(tr("About"));
  addAboutMenu();
}

void AntiquaMenuBar::addApplMenu() {
  QAction *ac_quit = m_applMenu->addAction(tr("Quit"));
  ac_quit->setIcon(QIcon(":icons/action_quit.png"));
  ac_quit->setShortcut(QKeySequence::Quit);
  connect(ac_quit, SIGNAL(triggered()), this, SIGNAL(sendApplQuit()));
}

void AntiquaMenuBar::addViewsMenu() {
  QAction *ac_views = m_viewsMenu->addAction("TODO");
  ac_views->setIcon(defaultIcon);
}

void AntiquaMenuBar::addConfigMenu() {
  QAction *ac_config = m_configMenu->addAction("TODO");
  ac_config->setIcon(defaultIcon);
}

void AntiquaMenuBar::addAboutMenu() {
  QAction *ac_about = m_aboutMenu->addAction("TODO");
  ac_about->setIcon(defaultIcon);
}
