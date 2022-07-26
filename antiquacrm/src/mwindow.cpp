// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

/* Project */
#include "mwindow.h"
#include "categoryedit.h"
#include "configdialog.h"
#include "deliverservicedialog.h"
// #include "dockbarwidget.h"
#include "filedialog.h"
#include "keywordedit.h"
#include "myicontheme.h"
#include "statusbar.h"
#include "storagelocation.h"
#include "workspace.h"

#include <QJsonDocument>
#include <QJsonObject>

#include <QCoreApplication>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QTabWidget>

MWindow::MWindow(QWidget *parent) : QMainWindow(parent) {
  setObjectName("MainWindow");
  setWindowTitle(QString(ANTIQUACRM_WINDOW_TITLE) + " [*]");
  setMinimumSize(QSize(1024, 650));
  setWindowIcon(myIcon("database"));

  // Settings
  m_Settings = new ApplSettings();

  // Main Widget
  m_workSpace = new Workspace(this);
  setCentralWidget(m_workSpace);

  m_signalMapper = new QSignalMapper(this);
  m_signalMapper->setObjectName("open_tab_signals");

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
  connect(m_signalMapper, SIGNAL(mappedInt(int)), m_workSpace,
          SLOT(openTab(int)));
  connect(m_workSpace, SIGNAL(s_postMessage(const QString &)), this,
          SLOT(statusMessage(const QString &)));
  connect(m_workSpace, SIGNAL(s_windowModified(bool)), this,
          SLOT(setWindowModified(bool)));
}

void MWindow::setupTabMenu(QMenu *parent) {
  const QIcon tabIcon = myIcon("tab_new");

  QAction *ac_Prints = parent->addAction(tabIcon, tr("Prints"));
  ac_Prints->setObjectName("open_prints_tab");
  m_signalMapper->setMapping(ac_Prints, Workspace::Prints);
  connect(ac_Prints, SIGNAL(triggered()), m_signalMapper, SLOT(map()));

  QAction *ac_views = parent->addAction(tabIcon, tr("Views"));
  ac_views->setObjectName("open_views_tab");
  m_signalMapper->setMapping(ac_views, Workspace::Views);
  connect(ac_views, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
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

  bool enable_file = false;
  QMenu *menu_files =
      m_applicationMenu->addMenu(myIcon("folder_green"), tr("Open"));
  menu_files->setEnabled(enable_file);
  menu_files->setObjectName("menu_filemenu");

  QAction *ac_openfile =
      menu_files->addAction(myIcon("folder_txt"), tr("Open file"));
  ac_openfile->setObjectName("ac_fileopen");
  ac_openfile->setEnabled(enable_file);
  connect(ac_openfile, SIGNAL(triggered(bool)), this,
          SLOT(openFileDialog(bool)));

  m_applicationMenu->addSeparator();

  m_quitAction = m_applicationMenu->addAction(tr("Quit"));
  m_quitAction->setObjectName("ac_closeApp");
  m_quitAction->setIcon(myIcon("close_mini"));
  connect(m_quitAction, SIGNAL(triggered(bool)), this, SLOT(closeWindow()));

  QMenu *m_viewsMenu = m_menuBar->addMenu(tr("Views"));
  m_viewsMenu->setObjectName(QLatin1String("ViewsMenu"));

  /**
   @todo Open hidden Tabs
  */
  setupTabMenu(m_viewsMenu);

  m_viewsMenu->addSeparator();
  m_toggleFullScreen = m_viewsMenu->addAction(tr("Fullscreen"));
  m_toggleFullScreen->setIcon(myIcon("window_fullscreen"));
  m_toggleFullScreen->setShortcut(QKeySequence::FullScreen);
  connect(m_toggleFullScreen, SIGNAL(triggered(bool)), this,
          SLOT(toggleFullScreen(bool)));

  QMenu *m_settingsMenu = m_menuBar->addMenu(tr("Settings"));
  m_settingsMenu->setObjectName("settings_menu");

  QAction *a_cfg = m_settingsMenu->addAction(tr("Configuration"));
  a_cfg->setIcon(myIcon("configure"));
  a_cfg->setShortcut(QKeySequence::Preferences);
  connect(a_cfg, SIGNAL(triggered(bool)), this, SLOT(openConfiguration(bool)));

  QMenu *m_tablesMenu = m_settingsMenu->addMenu(tr("Edit tables"));
  m_tablesMenu->setObjectName("top_table_menu");
  m_tablesMenu->setIcon(myIcon("database"));

  QAction *a_bst = m_tablesMenu->addAction(tr("Storage Locations"));
  a_bst->setIcon(myIcon("spreadsheet"));
  connect(a_bst, SIGNAL(triggered(bool)), this,
          SLOT(openStorageLocation(bool)));

  QAction *a_ekw = m_tablesMenu->addAction(tr("Edit Keywords"));
  a_ekw->setIcon(myIcon("spreadsheet"));
  connect(a_ekw, SIGNAL(triggered(bool)), this, SLOT(openKeywordEdit(bool)));

  QAction *a_dsd = m_tablesMenu->addAction(tr("Delivery Services"));
  a_dsd->setIcon(myIcon("spreadsheet"));
  connect(a_dsd, SIGNAL(triggered(bool)), this,
          SLOT(openDeliveryService(bool)));

  QAction *a_bct = m_tablesMenu->addAction(tr("Condition"));
  a_bct->setIcon(myIcon("spreadsheet"));
  connect(a_bct, SIGNAL(triggered(bool)), this, SLOT(openCondition(bool)));

  QAction *a_bdt = m_tablesMenu->addAction(tr("Designation and Binding"));
  a_bdt->setIcon(myIcon("spreadsheet"));
  connect(a_bdt, SIGNAL(triggered(bool)), this, SLOT(openDesignation(bool)));

  QAction *a_ecs = m_tablesMenu->addAction(tr("Edit Catgerories"));
  a_ecs->setIcon(myIcon("spreadsheet"));
  connect(a_ecs, SIGNAL(triggered(bool)), this, SLOT(openCategoryEdit(bool)));
}

void MWindow::closeWindow() {
  if (isWindowModified()) {
    int ret =
        QMessageBox::question(this, tr("Save request"),
                              tr("<b>You have unsaved changes.</b><p>Do you "
                                 "really want to close the application?</p>"));
    if (ret == QMessageBox::No) {
      return;
    }
  }
  close();
}

void MWindow::openEditAutoFill(CompleterDialog::Filter t) {
  CompleterDialog *m_dialog = new CompleterDialog(this, t);
  m_dialog->setObjectName("completer_dialog");
  if (m_dialog->exec()) {
    m_dialog->deleteLater();
  }
}

void MWindow::openStorageLocation(bool) {
  StorageLocation *d = new StorageLocation(this);
  if (d->exec()) {
    d->deleteLater();
  }
}

void MWindow::openDeliveryService(bool) {
  DeliverServiceDialog *d = new DeliverServiceDialog(this);
  if (d->exec()) {
    d->deleteLater();
  }
}

void MWindow::openCondition(bool) {
  openEditAutoFill(CompleterDialog::CONDITION);
}

void MWindow::openDesignation(bool) {
  openEditAutoFill(CompleterDialog::DESIGNATION);
}

void MWindow::openFileDialog(bool) {
  FileDialog *d = new FileDialog(this);
  d->setObjectName("m_file_dialog");
  if (d->exec()) {
    d->deleteLater();
  }
}

void MWindow::openCategoryEdit(bool) {
  CategoryEdit *d = new CategoryEdit(this);
  d->setObjectName("category_edit");
  if (d->exec()) {
    d->deleteLater();
  }
}

void MWindow::openKeywordEdit(bool) {
  KeywordEdit *d = new KeywordEdit(this);
  d->setObjectName("keyword_edit");
  if (d->exec()) {
    d->deleteLater();
  }
}

/**
 * @brief Öffne das Konfigurationen Dialog Fenster
 */
void MWindow::openConfiguration(bool) {
  ConfigDialog *d = new ConfigDialog(this);
  d->setObjectName("configuration_dialog");
  if (d->exec()) {
    d->deleteLater();
  }
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
  if (isFullScreen()) {
    setWindowState(windowState() & ~Qt::WindowFullScreen);
    m_toggleFullScreen->setIcon(myIcon("window_fullscreen"));
    m_toggleFullScreen->setText(tr("Fullscreen"));
  } else {
    setWindowState(windowState() ^ Qt::WindowFullScreen);
    m_toggleFullScreen->setIcon(myIcon("window_nofullscreen"));
    m_toggleFullScreen->setText(tr("Disable Fullscreen"));
  }
}

/**
   @brief Neu mit der Datenbank Verbinden
   @param b UNUSED
 */
void MWindow::reconnectDatabase(bool b) { emit s_connectDatabase(b); }

void MWindow::closeEvent(QCloseEvent *event) {
  if (isFullScreen()) // Keine Vollansicht Speichern!
    setWindowState(windowState() & ~Qt::WindowFullScreen);

  m_Settings->setValue("window/geometry", saveGeometry());
  m_Settings->setValue("window/windowState", saveState());
  QMainWindow::closeEvent(event);
}

void MWindow::statusMessage(const QString &info) {
  m_statusBar->sqlStatusMessage(info);
}

void MWindow::connectionStatus(bool b) {
  m_statusBar->setDatabaseStatusIcon(b);
}

void MWindow::initDefaults() {
  if (m_Settings->contains("window/geometry"))
    restoreGeometry(m_Settings->value("window/geometry").toByteArray());

  if (m_Settings->contains("window/windowState"))
    restoreState(m_Settings->value("window/windowState").toByteArray());

  m_workSpace->openTab(Workspace::Orders);
  m_workSpace->openTab(Workspace::Providers);
  m_workSpace->openTab(Workspace::Customers);
  m_workSpace->openTab(Workspace::Books);
  // Nach Vorne holen
  m_workSpace->openTab(Workspace::Orders);

}

MWindow::~MWindow() { /* TODO */
}
