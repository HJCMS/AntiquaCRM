// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquamenubar.h"
#include "antiquaconfigmenus.h"
#include "antiquaviewsmenus.h"

AntiquaMenuBar::AntiquaMenuBar(QMainWindow *parent)
    : QMenuBar{parent}, defaultIcon(QString(":icons/antiqua.png")) {
  setNativeMenuBar(true);
  setStyleSheet("");

  m_applMenu = addMenu("Antiqua CRM");
  addApplMenu();

  m_viewsMenu = new AntiquaViewsMenus(this);
  addMenu(m_viewsMenu);

  m_configMenu = new AntiquaConfigMenus(this);
  addMenu(m_configMenu);

  m_aboutMenu = addMenu(tr("About"));
  addAboutMenu();

  connect(m_viewsMenu, SIGNAL(sendOpenView(const QString &)),
          SIGNAL(sendOpenTabViews(const QString &)));

  connect(m_viewsMenu, SIGNAL(sendToggleFullscreen()),
          SIGNAL(sendToggleFullscreen()));
}

void AntiquaMenuBar::addApplMenu() {
  QAction *ac_quit = m_applMenu->addAction(tr("Application Quit"));
  ac_quit->setIcon(QIcon(":icons/action_quit.png"));
  ac_quit->setShortcut(QKeySequence::Quit);
  connect(ac_quit, SIGNAL(triggered()), this, SIGNAL(sendApplQuit()));
}

void AntiquaMenuBar::addAboutMenu() {
  QAction *ac_about = m_aboutMenu->addAction("TODO");
  ac_about->setIcon(defaultIcon);
}
