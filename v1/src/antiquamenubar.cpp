// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquamenubar.h"

AntiquaMenuBar::AntiquaMenuBar(QMainWindow *parent) : QMenuBar{parent}
{
  setNativeMenuBar(true);
  setStyleSheet("");

  QIcon defaultIcon(QString(":icons/antiqua.png"));
  m_applMenu = addMenu("Antiqua CRM");
  QAction *ac_quit = m_applMenu->addAction("TEST");
  ac_quit->setIcon(defaultIcon);

  m_viewsMenu = addMenu(tr("Views"));
  QAction *ac_views = m_viewsMenu->addAction("TEST");
  ac_views->setIcon(defaultIcon);

  m_configMenu = addMenu(tr("Configuration"));
  QAction *ac_config = m_configMenu->addAction("TEST");
  ac_config->setIcon(defaultIcon);

  m_aboutMenu = addMenu(tr("About"));
  QAction *ac_about = m_aboutMenu->addAction("TEST");
  ac_about->setIcon(defaultIcon);
}
