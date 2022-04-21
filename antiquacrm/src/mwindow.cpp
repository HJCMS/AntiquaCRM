// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

/* Project */
#include "mwindow.h"
#include "applsettings.h"
#include "completerdialog.h"
#include "configdialog.h"
#include "dockbarwidget.h"
#include "filedialog.h"
#include "statusbar.h"
#include "version.h"
#include "workspace.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QTabWidget>

MWindow::MWindow(QWidget *parent) : QMainWindow(parent) {
  setObjectName("MainWindow");
  setWindowTitle(ANTIQUACRM_DISPLAYNAME);
  setMinimumSize(QSize(1024, 700));
  setWindowIcon(myIcon("database"));

  // Settings
  m_Settings = new ApplSettings();

  // Main Widget
  m_workSpace = new Workspace(this);
  setCentralWidget(m_workSpace);

  // Menues
  m_menuBar = menuBar();
  setupActions();

  // DockWidget
  /*
  m_dockBarWidget = new DockBarWidget();
  QString dockPos = m_Settings->value("dockarea/position", "left").toString();
  if (dockPos.contains("left")) {
    addDockWidget(Qt::LeftDockWidgetArea, m_dockBarWidget);
  } else {
    addDockWidget(Qt::RightDockWidgetArea, m_dockBarWidget);
  }
  */

  m_statusBar = new StatusBar(statusBar());
  setStatusBar(m_statusBar);

  if (m_Settings->contains("window/geometry"))
    restoreGeometry(m_Settings->value("window/geometry").toByteArray());

  if (m_Settings->contains("window/windowState"))
    restoreState(m_Settings->value("window/windowState").toByteArray());

  connect(this, SIGNAL(setStatusMessage(const QString &)), this,
          SLOT(postStatusBarMessage(const QString &)));
}

/**
 * @brief MWindow::createMenuBars
 */
void MWindow::setupActions() {
  m_applicationMenu = m_menuBar->addMenu(tr("Application"));
  m_applicationMenu->setObjectName(QLatin1String("ApplicationMenu"));

  QIcon dbIcon = myIcon("database");
  QMenu *menu_db = m_applicationMenu->addMenu(dbIcon, tr("Database"));
  menu_db->setObjectName("menu_database");

  QAction *ac_dbConnect = menu_db->addAction(dbIcon, tr("Connect"));
  ac_dbConnect->setObjectName("ac_dbconnect");
  connect(ac_dbConnect, SIGNAL(triggered(bool)), this,
          SLOT(reconnectDatabase(bool)));

  QMenu *menu_files =
      m_applicationMenu->addMenu(myIcon("folder_green"), tr("Open"));
  menu_files->setObjectName("menu_filemenu");

  QAction *ac_openfile =
      menu_files->addAction(myIcon("folder_txt"), tr("Open file"));
  ac_openfile->setObjectName("ac_fileopen");
  ac_openfile->setEnabled(false);
  connect(ac_openfile, SIGNAL(triggered(bool)), this,
          SLOT(openFileDialog(bool)));

  m_applicationMenu->addSeparator();

  m_quitAction = m_applicationMenu->addAction(tr("Quit"));
  m_quitAction->setObjectName("ac_closeApp");
  m_quitAction->setIcon(myIcon("close_mini"));
  connect(m_quitAction, SIGNAL(triggered(bool)), this, SLOT(close()));

  QMenu *m_viewsMenu = m_menuBar->addMenu(tr("Views"));
  m_viewsMenu->setObjectName(QLatin1String("ViewsMenu"));

  /**
   @todo Open hidden Tabs
  */
  m_viewsMenu->addAction(tr("Open Tab"));

  m_viewsMenu->addSeparator();
  QAction *a_fs = m_viewsMenu->addAction(tr("Fullscreen"));
  connect(a_fs, SIGNAL(triggered(bool)), this, SLOT(toggleFullScreen(bool)));

  QMenu *m_settingsMenu = m_menuBar->addMenu(tr("Settings"));
  m_settingsMenu->setObjectName("settings_menu");

  QAction *a_cfg = m_settingsMenu->addAction(tr("Configuration"));
  a_cfg->setIcon(myIcon("configure"));
  connect(a_cfg, SIGNAL(triggered(bool)), this, SLOT(openConfiguration(bool)));

  QMenu *m_tablesMenu = m_settingsMenu->addMenu(tr("Edit tables"));
  m_tablesMenu->setObjectName("top_table_menu");
  m_tablesMenu->setIcon(myIcon("database"));

  QAction *a_bct = m_tablesMenu->addAction(tr("Conditions"));
  a_bct->setIcon(myIcon("spreadsheet"));
  connect(a_bct, SIGNAL(triggered(bool)), this, SLOT(openCondition(bool)));

  QAction *a_bdt = m_tablesMenu->addAction(tr("Book Designations"));
  a_bdt->setIcon(myIcon("spreadsheet"));
  connect(a_bdt, SIGNAL(triggered(bool)), this,
          SLOT(openBookDesignation(bool)));

  QAction *a_pdt = m_tablesMenu->addAction(tr("Prints Designation"));
  a_pdt->setIcon(myIcon("spreadsheet"));
  connect(a_pdt, SIGNAL(triggered(bool)), this,
          SLOT(openPrintsDesignation(bool)));
}

void MWindow::openEditCondition() {
  CompleterDialog *m_dialog = new CompleterDialog(this, "condition");
  m_dialog->setObjectName("condition_dialog");
  if (m_dialog->exec()) {
    qInfo("Editing finished");
  }
}

void MWindow::openEditDesignation(const QString &section) {
  QString key("designation");
  if (section.contains("ib_")) {
    key.prepend("ib_");
  } else if (section.contains("ip_")) {
    key.prepend("ip_");
  } else {
    return;
  }
  CompleterDialog *m_dialog = new CompleterDialog(this, key);
  m_dialog->setObjectName("designation_dialog");
  if (m_dialog->exec()) {
    qInfo("Editing finished");
  }
}

void MWindow::openCondition(bool) { openEditCondition(); };

void MWindow::openBookDesignation(bool) { openEditDesignation("ib_"); };

void MWindow::openPrintsDesignation(bool) { openEditDesignation("ip_"); };

void MWindow::openFileDialog(bool) {
  FileDialog *m_dialog = new FileDialog(this);
  m_dialog->setObjectName("m_file_dialog");
  if (m_dialog->exec()) {
    qInfo("Filedialog finished");
    // m_dialog->selectedFiles();
  }
}

/**
 * @brief Öffne das Konfigurationen Dialog Fenster
 */
void MWindow::openConfiguration(bool) {
  ConfigDialog *m_dialog = new ConfigDialog(this);
  m_dialog->exec();
}

void MWindow::sqlErrorMessageBox(const QString &err) {
  QMessageBox *m_box = new QMessageBox(this);
  m_box->setTextFormat(Qt::AutoText);
  m_box->setIcon(QMessageBox::Critical);
  m_box->setText(tr("SQL Server Error"));
  m_box->setDetailedText(err);
  m_box->exec();
}

/**
 * @brief Zwischen Vollansicht und Normaler Ansicht wechseln.
 */
void MWindow::toggleFullScreen(bool b) {
  Q_UNUSED(b);
  if (isFullScreen())
    setWindowState(windowState() & ~Qt::WindowFullScreen);
  else
    setWindowState(windowState() ^ Qt::WindowFullScreen);
}

/**
   @brief Neu mit der Datenbank Verbinden
   @param b UNUSED
 */
void MWindow::reconnectDatabase(bool b) {
  Q_UNUSED(b);
  // connectSqlDatabase();
}

void MWindow::closeEvent(QCloseEvent *event) {
  if (isFullScreen()) // Keine Vollansicht Speichern!
    setWindowState(windowState() & ~Qt::WindowFullScreen);

  m_Settings->setValue("window/geometry", saveGeometry());
  m_Settings->setValue("window/windowState", saveState());
  QMainWindow::closeEvent(event);
}

void MWindow::postStatusBarMessage(const QString &info) {
  m_statusBar->sqlStatusMessage(info);
}

MWindow::~MWindow() { qInfo("Mainwindow onload"); }
