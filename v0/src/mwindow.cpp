// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

/* Project */
#include "mwindow.h"
#include "antiquasystemtray.h"
#include "categoryedit.h"
#include "completerdialog.h"
#include "configdialog.h"
#include "deliverservicedialog.h"
#include "keywordedit.h"
#include "messagetemplates.h"
#include "myicontheme.h"
#include "reportingdialog.h"
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
  setMinimumSize(QSize(800, 580));
  setWindowIcon(myIcon("database"));

  // Settings
  m_Settings = new ApplSettings();

  // Main Widget
  m_workSpace = new Workspace(this);
  setCentralWidget(m_workSpace);

  // Registerkarten Signale
  m_signalMapper = new QSignalMapper(this);
  m_signalMapper->setObjectName("open_tab_signals");

  // Standard Menüs
  m_menuBar = menuBar();
  setupActions();

  // Statusleiste
  m_statusBar = new StatusBar(statusBar());
  setStatusBar(m_statusBar);

  // Benachrichtigungsfeld
  m_systemTray = new AntiquaSystemTray(myIcon("database"), this);

  // Registerkarten öffnen
  connect(m_signalMapper, SIGNAL(mappedInt(int)), m_workSpace,
          SLOT(openTab(int)));

  // Nachrichten für Benachrichtigungsfeld
  connect(m_workSpace,
          SIGNAL(sendPostMessage(Antiqua::ErrorStatus, const QString &)), this,
          SLOT(systemMessage(Antiqua::ErrorStatus, const QString &)));

  // Änderungen registrieren
  connect(m_workSpace, SIGNAL(s_windowModified(bool)), this,
          SLOT(setWindowModified(bool)));
}

void MWindow::setupActions() {
  m_applicationMenu = m_menuBar->addMenu(tr("Application"));
  m_applicationMenu->setObjectName(QLatin1String("ApplicationMenu"));

  QIcon dbIcon = myIcon("database");
  // BEGIN Hauptmenü
  QMenu *menu_db = m_applicationMenu->addMenu(dbIcon, tr("Database"));
  menu_db->setObjectName("menu_database");

  QAction *ac_dbConnect = menu_db->addAction(dbIcon, tr("Reconnect"));
  ac_dbConnect->setObjectName("ac_dbconnect");
  connect(ac_dbConnect, SIGNAL(triggered(bool)), this,
          SIGNAL(sendDatabaseConnect(bool)));

  QAction *ac_reports = menu_db->addAction(dbIcon, tr("Monthly Report"));
  ac_reports->setObjectName("ac_monthly_reports");
  connect(ac_reports, SIGNAL(triggered(bool)), this,
          SLOT(openReportDialog(bool)));

  m_applicationMenu->addSeparator();

  m_quitAction = m_applicationMenu->addAction(tr("Quit"));
  m_quitAction->setObjectName("ac_closeApp");
  m_quitAction->setIcon(myIcon("close_mini"));
  connect(m_quitAction, SIGNAL(triggered(bool)), this, SLOT(quitApplication()));
  // END Hauptmenü

  // BEGIN Ansichten
  QMenu *m_viewsMenu = m_menuBar->addMenu(tr("Views"));
  m_viewsMenu->setObjectName(QLatin1String("ViewsMenu"));
  setupTabMenu(m_viewsMenu);

  m_viewsMenu->addSeparator();
  m_toggleFullScreen = m_viewsMenu->addAction(tr("Fullscreen"));
  m_toggleFullScreen->setIcon(myIcon("window_fullscreen"));
  m_toggleFullScreen->setShortcut(QKeySequence::FullScreen);
  connect(m_toggleFullScreen, SIGNAL(triggered(bool)), this,
          SLOT(toggleFullScreen(bool)));
  // END Ansichten

  // BEGIN Konfigurations Huptmenü
  QMenu *m_settingsMenu = m_menuBar->addMenu(tr("Settings"));
  m_settingsMenu->setObjectName("settings_menu");

  QMenu *m_tablesMenu = m_settingsMenu->addMenu(tr("Various settings"));
  m_tablesMenu->setObjectName("top_table_menu");
  m_tablesMenu->setIcon(myIcon("database"));

  m_settingsMenu->addSeparator();

  QAction *a_cfg = m_settingsMenu->addAction(tr("Applicationsettings"));
  a_cfg->setIcon(myIcon("configure"));
  a_cfg->setShortcut(QKeySequence::Preferences);
  connect(a_cfg, SIGNAL(triggered(bool)), this, SLOT(openConfiguration(bool)));
  // END Konfigurations Huptmenü

  // BEGIN Konfigurations Untermenü Tabellen
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

  QAction *a_tpl = m_tablesMenu->addAction(tr("Message Templates"));
  a_tpl->setIcon(myIcon("spreadsheet"));
  connect(a_tpl, SIGNAL(triggered(bool)), this, SLOT(openMessagesEdit(bool)));

  QAction *a_bdt = m_tablesMenu->addAction(tr("Designation and Binding"));
  a_bdt->setIcon(myIcon("spreadsheet"));
  connect(a_bdt, SIGNAL(triggered(bool)), this, SLOT(openDesignation(bool)));
  // END Konfigurations Untermenü Tabellen
}

void MWindow::setupTabMenu(QMenu *parent) {
  const QIcon tabIcon = myIcon("tab_new");

  // Siehe alte InventoryPrints
  QAction *ac_custom = parent->addAction(tabIcon, tr("Anything but books"));
  ac_custom->setObjectName("open_custom_tab");
  m_signalMapper->setMapping(ac_custom, Workspace::Prints);
  connect(ac_custom, SIGNAL(triggered()), m_signalMapper, SLOT(map()));

  // Ansichten
  QAction *ac_views = parent->addAction(tabIcon, tr("Views"));
  ac_views->setObjectName("open_views_tab");
  m_signalMapper->setMapping(ac_views, Workspace::Views);
  connect(ac_views, SIGNAL(triggered()), m_signalMapper, SLOT(map()));
}

void MWindow::quitApplication() {
  if (isWindowModified()) {
    QString t = tr("Save request");
    QStringList b("<b>You have unsaved changes.</b>");
    b << tr("<p>Do you really want to close the application?</p>");
    int ret = QMessageBox::question(this, t, b.join("<br/>"));
    if (ret == QMessageBox::No) {
      return;
    }
  }
  emit sendShutdown();
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

void MWindow::openDesignation(bool) {
  CompleterDialog *d = new CompleterDialog(this, CompleterDialog::DESIGNATION);
  d->setObjectName("completer_dialog");
  if (d->exec()) {
    d->deleteLater();
  }
}

void MWindow::openReportDialog(bool) {
  ReportingDialog *d = new ReportingDialog(this);
  d->setObjectName("reporting_dialog");
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

void MWindow::openMessagesEdit(bool) {
  MessageTemplates *d = new MessageTemplates(this);
  d->setObjectName("message_templates");
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

void MWindow::closeEvent(QCloseEvent *event) {
  if (isFullScreen()) // Keine Vollansicht Speichern!
    setWindowState(windowState() & ~Qt::WindowFullScreen);

  m_Settings->setValue("window/geometry", saveGeometry());
  m_Settings->setValue("window/windowState", saveState());

  /**
   * @note Damit die Timer weiterlaufen, hier nicht beenden!
   * Wenn Fensterleiste schließen aufgerufen wird, das Fenster in das Systemtray
   * verschieben. Beenden erfolgt über das SIGNAL sendShutdown() in
   * MApplication.
   */
  QHideEvent hide;
  hide.setAccepted(true);
  QMainWindow::hideEvent(&hide);
}

void MWindow::statusBarMessage(const QString &info) {
  if (isVisible()) {
    m_statusBar->sqlStatusMessage(info);
  } else {
    m_systemTray->notify(info);
  }
}

void MWindow::systemMessage(Antiqua::ErrorStatus status, const QString &info) {
  if (status == Antiqua::ErrorStatus::WARNING) {
    m_systemTray->warning(info);
    return;
  } else if (status == Antiqua::ErrorStatus::FATAL) {
    m_systemTray->fatal(info);
    return;
  }
  m_systemTray->notify(info);
}

bool MWindow::initDefaults() {
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
  // SystemTray anzeigen
  m_systemTray->show();

  return true;
}
