// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mainwindow.h"
#include "menubar.h"
#include "statusbar.h"
#include "tabswidget.h"
// Static Tab Interfaces
#include "tabs/customers/tabcustomers.h"
#include "tabs/orders/taborders.h"
#include "tabs/sellers/tabsellers.h"

#include <QApplication>

MainWindow::MainWindow(AntiquaCRM::ASettings* cfg, QWidget* parent)
  : QMainWindow{parent}, m_cfg{cfg} {
  setObjectName("antiqua_ui_mainwindow");
  setWindowTitle(QString(ANTIQUACRM_WINDOW_TITLE) + " [*]");
  setMinimumSize(QSize(800, 580));
  setTabletTracking(false);

  // Main Window Menubar
  m_menuBar = new MenuBar(this);
  setMenuBar(m_menuBar);

  // AntiquaTabs
  m_tabWidget = new AntiquaCRM::TabsWidget(this);
  setCentralWidget(m_tabWidget);

  // Main Statusbar
  m_statusBar = new StatusBar(this);
  setStatusBar(m_statusBar);

  // Begin:Menu:Signals
  connect(m_menuBar, SIGNAL(sendApplicationQuit()), SIGNAL(sendApplicationQuit()));
  connect(m_menuBar->tabsMenu, SIGNAL(sendOpenTab(QString)), SLOT(setViewTab(QString)));
  // End:Menu:Signals
  connect(m_tabWidget, SIGNAL(sendStatusMessage(QString)), m_statusBar,
          SLOT(statusInfoMessage(QString)));
}

bool MainWindow::createSocketListener() {
  m_rx = new AntiquaCRM::AReceiver(this);
  connect(m_rx, SIGNAL(sendOperation(QString, QJsonObject)), SLOT(setAction(QString, QJsonObject)));
  connect(m_rx, SIGNAL(sendMessage(QString)), m_statusBar, SLOT(statusInfoMessage(QString)));
  return m_rx->open();
}

void MainWindow::loadStaticTabs() {
  // Provider tab
  TabSellers* m_sellers = new TabSellers(this);
  m_menuBar->tabsMenu->addAction(m_sellers->menuEntry());
  AntiquaCRM::TabsIndex* m_tab0 = m_sellers->indexWidget(m_tabWidget);
  m_tabWidget->registerTab(m_tab0);
  connect(m_tab0, SIGNAL(sendModifiedStatus(bool)), SLOT(setChanges(bool)));

  // Orders tab
  TabOrders* m_orders = new TabOrders(this);
  m_menuBar->tabsMenu->addAction(m_orders->menuEntry());
  AntiquaCRM::TabsIndex* m_tab1 = m_orders->indexWidget(m_tabWidget);
  m_tabWidget->registerTab(m_tab1);
  connect(m_tab1, SIGNAL(sendModifiedStatus(bool)), SLOT(setChanges(bool)));

  // Customer tab
  TabCustomers* m_customers = new TabCustomers(this);
  m_menuBar->tabsMenu->addAction(m_customers->menuEntry());
  AntiquaCRM::TabsIndex* m_tab2 = m_customers->indexWidget(m_tabWidget);
  m_tabWidget->registerTab(m_tab2);
  connect(m_tab2, SIGNAL(sendModifiedStatus(bool)), SLOT(setChanges(bool)));
}

bool MainWindow::loadPluginTabs() {
  AntiquaCRM::TabsLoader loader(this);
  tabInterfaces = loader.interfaces(this);
  if (tabInterfaces.size() > 0) {
    QListIterator<AntiquaCRM::TabsInterface*> it(tabInterfaces);
    while (it.hasNext()) {
      AntiquaCRM::TabsInterface* _iface = it.next();
      if (_iface != nullptr) {
        m_menuBar->tabsMenu->addAction(_iface->menuEntry());
        bool _load = m_cfg->value("plugin/tabs/enable/" + _iface->getSerialId(), false).toBool();
        if (!_iface->addIndexOnInit()) {
          if (!_load)
            continue;
        }

        AntiquaCRM::TabsIndex* _tab = _iface->indexWidget(m_tabWidget);
        if (_tab == nullptr) {
          qWarning("Plugin '%s' TabsIndex failed.", qPrintable(_iface->displayName()));
          qWarning("Unload '%s'", qPrintable(_iface->interfaceName()));
          _iface->deleteLater();
          continue;
        }
        m_tabWidget->registerTab(_tab);
        // windowModified
        connect(_tab, SIGNAL(sendModifiedStatus(bool)), SLOT(setChanges(bool)));
      }
    }
    m_tabWidget->sortTabs();
    return true;
  }
  return false;
}

void MainWindow::setChanges(bool b) {
  setWindowModified(b);
}

bool MainWindow::tabViewAction(const QString& id) {
  if (!m_menuBar->tabsMenu->exists(id))
    return false;

  if (m_tabWidget->indexByName(id) >= 0)
    return true;

  QListIterator<AntiquaCRM::TabsInterface*> it(tabInterfaces);
  while (it.hasNext()) {
    AntiquaCRM::TabsInterface* _iface = it.next();
    // qDebug() << Q_FUNC_INFO << _iface->menuEntry().value("id").toString();
    if (_iface->menuEntry().value("id").toString() == id) {
      AntiquaCRM::TabsIndex* m_tab = _iface->indexWidget(m_tabWidget);
      m_tabWidget->registerTab(m_tab);
      return true;
    }
  }
  return false;
}

void MainWindow::setViewTab(const QString& name) {
  if (tabViewAction(name))
    m_tabWidget->setCurrentTab(name);
}

void MainWindow::setAction(const QString& name, const QJsonObject& data) {
  if (name == "plugin") {
    const QString _action = data.value("ACTION").toString();
    if (_action.startsWith("provider")) {
      QString _provider = data.value("NAME").toString();
#ifdef ANTIQUA_DEVELOPMENT
      qDebug() << Q_FUNC_INFO << "TODO" << name << _provider;
#endif
    }
    return;
  }

  if (!tabViewAction(name) || !data.contains("TARGET")) {
    qWarning("Window: Invalid Target call!");
#ifdef ANTIQUA_DEVELOPMENT
    qDebug() << Q_FUNC_INFO << "REJECTED" << name << data;
#endif
    return;
  }

  if (!data.contains("ACTION")) {
    qWarning("Window: Missing Custom Action parameter!");
#ifdef ANTIQUA_DEVELOPMENT
    qDebug() << Q_FUNC_INFO << "REJECTED" << name << data;
#endif
    return;
  }

  int _index = m_tabWidget->indexByName(name);
  AntiquaCRM::TabsIndex* _tab = m_tabWidget->tabIndex(_index);
  if (_tab == nullptr) {
    qWarning("Window: Required Tab '%s' not open!", qPrintable(name));
    m_statusBar->showMessage("The required Tab is not opened!");
#ifdef ANTIQUA_DEVELOPMENT
    qDebug() << Q_FUNC_INFO << "Tab" << name << _index << data;
#endif
    return;
  }

  const QString _action = data.value("ACTION").toString().toLower();
  if (_tab->acceptsCustomActions().contains(_action, Qt::CaseInsensitive)
      && _tab->customAction(data)) {
    m_tabWidget->setCurrentIndex(_index);
    return;
  }

#ifdef ANTIQUA_DEVELOPMENT
  qDebug() << Q_FUNC_INFO << "REJECTED" << _index << name << data;
#endif
}

void MainWindow::showEvent(QShowEvent* event) {
  // Refresh seller widget if it's not on application launch.
  // On application start, this is interrupted by AntiquaCRM::TabsIndex::onEnterChange
  // The variable firstShown is set by the openWindow function.
  // It is intended to prevent too many database queries from being initiated at startup.
  if (event->isAccepted() && firstShown && (m_tabWidget->currentIndex() == 0)) {
    // SELLERS_INTERFACE_TABID
    const QString _target("sellers_tab");
    QJsonObject _obj;
    _obj.insert("TARGET", _target);
    _obj.insert("ACTION", "updateSellersTree");
    setAction(_target, _obj);
  }
  QMainWindow::showEvent(event);
}

void MainWindow::hideEvent(QHideEvent* event) {
  if (isVisible() && event->isAccepted()) {
    m_cfg->setValue("window/geometry", saveGeometry());
    if (isFullScreen()) // do not save fullscreen window
      setWindowState(windowState() & ~Qt::WindowFullScreen);

    m_cfg->setValue("window/windowState", saveState());
  }
  QMainWindow::hideEvent(event);
}

void MainWindow::closeEvent(QCloseEvent*) {
  // Prevent window close events from window decoration.
  QHideEvent hide;
  hide.setAccepted(true);
  hideEvent(&hide);
  return;
}

void MainWindow::setToggleWindow() {
  if (isVisible()) {
    hide();
  } else {
    showNormal();
  }
}

void MainWindow::setToggleFullScreen() {
  if (isFullScreen()) {
    setWindowState(windowState() & ~Qt::WindowFullScreen);
  } else {
    setWindowState(windowState() ^ Qt::WindowFullScreen);
  }
}

bool MainWindow::openWindow() {
  tabInterfaces.clear();

  if (!createSocketListener()) {
    qWarning("MainWindow::createSocketListener() failed!");
    return false;
  }

  // if changed by user, disable NativeMenuBar
  m_menuBar->setNativeMenuBar(m_cfg->value("window_behavior/native_window_support",true).toBool());

  loadStaticTabs();

  if (!loadPluginTabs())
    m_statusBar->showMessage(tr("No tabs available"));

  if (m_cfg->contains("window/geometry"))
    restoreGeometry(m_cfg->value("window/geometry").toByteArray());

  if (m_cfg->contains("window/windowState")) {
    QByteArray state = m_cfg->value("window/windowState").toByteArray();
    if (state.isNull()) {
      setWindowState((windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
    } else {
      restoreState(state);
    }
  }
  // Always show on application start
  setVisible(true);

  m_statusBar->showMessage(tr("Window opened"), 5000);
  // @see also showEvent handle
  firstShown = true;
  return true;
}

bool MainWindow::closeWindow() {
  if (isWindowModified()) {
    bool _reject = false;
    AntiquaCRM::APopUpMessage* d = new AntiquaCRM::APopUpMessage(this);
    d->setUnsavedMessage(tr("<p>Do you really want to quit the program?</p>"), true);
    _reject = (d->exec() == QDialog::Rejected);
    d->deleteLater();
    if (_reject)
      return false;
  }

  QByteArray _geometry = saveGeometry();
  if (!_geometry.isNull()) {
    m_cfg->setValue("window/geometry", _geometry);
    _geometry.clear();
  }

  return (m_tabWidget->unloadTabs() && close());
}

MainWindow::~MainWindow() {
  // Destroy tab interfaces, tabs already closed in closeWindow().
  if (tabInterfaces.size() > 0) {
    for (int i = 0; i < tabInterfaces.size(); i++) {
      tabInterfaces.takeAt(i)->deleteLater();
    }
    tabInterfaces.clear();
  }

  if (m_rx != nullptr)
    m_rx->deleteLater();
}
