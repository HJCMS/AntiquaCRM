// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquamenubar.h"
#include "aboutdialogs.h"
#include "antiquaconfigmenus.h"
#include "antiquaviewsmenus.h"

#include <QApplication>

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

  connect(m_viewsMenu, SIGNAL(sendShowTab(const QString &)),
          SIGNAL(sendViewTab(const QString &)));

  connect(m_viewsMenu, SIGNAL(sendOpenReport(const QString &)),
          SIGNAL(sendOpenTabReports(const QString &)));

  connect(m_viewsMenu, SIGNAL(sendOpenStats(const QString &)),
          SIGNAL(sendOpenTabStatistics(const QString &)));

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
  m_aboutDialogs = new AboutDialogs(this);

  QAction *ac_about_crm = m_aboutMenu->addAction("Antiqua CRM");
  ac_about_crm->setIcon(defaultIcon);
  connect(ac_about_crm, SIGNAL(triggered()), SLOT(openAboutCRM()));

  // https://www.qt.io
  QAction *ac_about_qt = m_aboutMenu->addAction("Qt Framework");
  ac_about_qt->setIcon(QIcon("://icons/qtlogo-64.png"));
  connect(ac_about_qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  QAction *ac_about_qr = m_aboutMenu->addAction("QRencode");
  ac_about_qr->setIcon(QIcon("://icons/filetypes.png"));
  connect(ac_about_qr, SIGNAL(triggered()), SLOT(openAboutQRE()));

  QAction *ac_about_sql = m_aboutMenu->addAction("PostgreSQL");
  ac_about_sql->setIcon(QIcon("://icons/postgresql.png"));
  connect(ac_about_sql, SIGNAL(triggered()), SLOT(openAboutPgSQL()));
}

void AntiquaMenuBar::openAboutCRM() {
  m_aboutDialogs->exec(AboutDialogs::ANTIQUACRM);
}

void AntiquaMenuBar::openAboutQRE() {
  m_aboutDialogs->exec(AboutDialogs::QRENCODE);
}

void AntiquaMenuBar::openAboutPgSQL() {
  m_aboutDialogs->exec(AboutDialogs::POSTGRESQL);
}

AntiquaMenuBar::~AntiquaMenuBar() { m_aboutDialogs->deleteLater(); }
