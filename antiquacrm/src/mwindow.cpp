// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

/* Project */
#include "version.h"
#include "statusbar.h"
#include "adockwidget.h"
#include "applsettings.h"
#include "workspace.h"
#include "mwindow.h"
/* Sub Modules */
#include "configdialog.h"

#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtWidgets/QTabWidget>

MWindow::MWindow(QWidget *parent) : QMainWindow(parent)
{
  setObjectName("MainWindow");
  setWindowTitle(ANTIQUACRM_DISPLAYNAME);
  setMinimumSize(QSize(500, 350));
  setWindowIcon(myIcon("database"));

  // Settings
  m_Settings = new ApplSettings();

  // Main Widget
  m_workSpace = new Workspace(this);
  setCentralWidget(m_workSpace);

  // Menues
  m_menuBar = menuBar();
  createMenuBars();

  // DockWidget
  m_adockWidget = new ADockWidget(m_mainWidget);
  QString dockPos = m_Settings->value("dockarea/position", "left").toString();
  if (dockPos.contains("left")) {
    addDockWidget(Qt::LeftDockWidgetArea, m_adockWidget);
  } else {
    addDockWidget(Qt::RightDockWidgetArea, m_adockWidget);
  }

  m_statusBar = new StatusBar(statusBar());
  setStatusBar(m_statusBar);

  if (m_Settings->contains("window/geometry"))
    restoreGeometry(m_Settings->value("window/geometry").toByteArray());

  if (m_Settings->contains("window/windowState"))
    restoreState(m_Settings->value("window/windowState").toByteArray());

  connect(this, SIGNAL(setStatusMessage(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));
}

/**
 * @brief MWindow::createMenuBars
 */
void MWindow::createMenuBars()
{
  m_applicationMenu = m_menuBar->addMenu(tr("Application"));
  m_applicationMenu->setObjectName(QLatin1String("ApplicationMenu"));

  QAction *a_dbc = m_applicationMenu->addAction(tr("DB Connect"));
  a_dbc->setObjectName("db_connect_action");
  a_dbc->setIcon(myIcon("database"));
  connect(a_dbc, SIGNAL(triggered(bool)), this, SIGNAL(sqlconnect(bool)));

  m_applicationMenu->addSeparator();

  m_quitAction = m_applicationMenu->addAction(tr("Quit"));
  m_quitAction->setObjectName("close_action");
  m_quitAction->setIcon(myIcon("close_mini"));
  connect(m_quitAction, SIGNAL(triggered(bool)), this, SLOT(close()));

  m_viewsMenu = m_menuBar->addMenu(tr("Views"));
  m_viewsMenu->setObjectName(QLatin1String("ViewsMenu"));

  /* TODO */
  m_viewsMenu->addAction(tr("Books"));
  m_viewsMenu->addAction(tr("Prints"));
  m_viewsMenu->addAction(tr("Custom"));
  m_viewsMenu->addSeparator();
  QAction *a_fs = m_viewsMenu->addAction(tr("Fullscreen"));
  connect(a_fs, SIGNAL(triggered(bool)), this, SLOT(toggleFullScreen(bool)));

  m_settingsMenu = m_menuBar->addMenu(tr("Settings"));
  m_settingsMenu->setObjectName(QLatin1String("SettingsMenu"));

  QAction *a_cfg = m_settingsMenu->addAction(tr("Configuration"));
  connect(a_cfg, SIGNAL(triggered(bool)), this, SLOT(openConfiguration(bool)));
}

/**
 * @brief MWindow::openConfiguration
 */
void MWindow::openConfiguration(bool b)
{
  Q_UNUSED(b);
  ConfigDialog *m_dialog = new ConfigDialog(this);
  m_dialog->exec();
}

/**
* Zwischen Vollansicht und Normaler Ansicht wechseln.
*/
void MWindow::toggleFullScreen(bool b)
{
  Q_UNUSED(b);
  if (isFullScreen())
    setWindowState(windowState() & ~Qt::WindowFullScreen);
  else
    setWindowState(windowState() ^ Qt::WindowFullScreen);
}

void MWindow::closeEvent(QCloseEvent *event)
{
  if (isFullScreen()) // Keine Vollansicht Speichern!
    setWindowState(windowState() & ~Qt::WindowFullScreen);

  m_Settings->setValue("window/geometry", saveGeometry());
  m_Settings->setValue("window/windowState", saveState());
  QMainWindow::closeEvent(event);
}

MWindow::~MWindow()
{}
