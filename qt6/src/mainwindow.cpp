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

MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent} {
  setObjectName("antiqua_ui_mainwindow");
  setWindowTitle(QString(ANTIQUACRM_WINDOW_TITLE) + " [*]");
  setMinimumSize(QSize(800, 580));

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
  connect(m_menuBar, SIGNAL(sendApplicationQuit()),
          SIGNAL(sendApplicationQuit()));

  connect(m_menuBar->tabsMenu, SIGNAL(sendOpenTab(const QString &)),
          SLOT(setViewTab(const QString &)));
  // End:Menu:Signals

  connect(m_tabWidget, SIGNAL(sendMessage(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));
}

bool MainWindow::createSocketListener() {
  m_rx = new AntiquaCRM::AReceiver(this);
  connect(m_rx, SIGNAL(sendOperation(const QString &, const QJsonObject &)),
          SLOT(setAction(const QString &, const QJsonObject &)));
  connect(m_rx, SIGNAL(sendMessage(const QString &)), m_statusBar,
          SLOT(statusInfoMessage(const QString &)));

  return m_rx->listen(AntiquaCRM::AUtil::socketName());
}

void MainWindow::loadStaticTabs() {
  // Provider tab
  TabSellers *m_sellers = new TabSellers(this);
  m_menuBar->tabsMenu->addAction(m_sellers->menuEntry());
  AntiquaCRM::TabsIndex *m_tab0 = m_sellers->indexWidget(m_tabWidget);
  m_tabWidget->registerTab(m_tab0);
  connect(m_tab0, SIGNAL(sendModifiedStatus(bool)), SLOT(setChanges(bool)));

  // Orders tab
  TabOrders *m_orders = new TabOrders(this);
  m_menuBar->tabsMenu->addAction(m_orders->menuEntry());
  AntiquaCRM::TabsIndex *m_tab1 = m_orders->indexWidget(m_tabWidget);
  m_tabWidget->registerTab(m_tab1);
  connect(m_tab1, SIGNAL(sendModifiedStatus(bool)), SLOT(setChanges(bool)));

  // Customer tab
  TabCustomers *m_customers = new TabCustomers(this);
  m_menuBar->tabsMenu->addAction(m_customers->menuEntry());
  AntiquaCRM::TabsIndex *m_tab2 = m_customers->indexWidget(m_tabWidget);
  m_tabWidget->registerTab(m_tab2);
  connect(m_tab2, SIGNAL(sendModifiedStatus(bool)), SLOT(setChanges(bool)));
}

bool MainWindow::loadPluginTabs() {
  AntiquaCRM::TabsLoader loader(this);
  tabInterfaces = loader.interfaces(this);
  if (tabInterfaces.size() > 0) {
    QListIterator<AntiquaCRM::TabsInterface *> it(tabInterfaces);
    while (it.hasNext()) {
      AntiquaCRM::TabsInterface *_iface = it.next();
      if (_iface != nullptr) {
        m_menuBar->tabsMenu->addAction(_iface->menuEntry());
        bool _load =
            config->value("plugin/tabs/enable/" + _iface->getSerialId(), false)
                .toBool();
        if (!_iface->addIndexOnInit()) {
          if (!_load)
            continue;
        }

        AntiquaCRM::TabsIndex *_tab = _iface->indexWidget(m_tabWidget);
        if (_tab == nullptr) {
          qWarning("Plugin '%s' TabsIndex failed.",
                   qPrintable(_iface->displayName()));
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

void MainWindow::setChanges(bool b) { setWindowModified(b); }

bool MainWindow::tabViewAction(const QString &id) {
  if (!m_menuBar->tabsMenu->exists(id))
    return false;

  if (m_tabWidget->indexByName(id) >= 0)
    return true;

  QListIterator<AntiquaCRM::TabsInterface *> it(tabInterfaces);
  while (it.hasNext()) {
    AntiquaCRM::TabsInterface *_iface = it.next();
    if (_iface->menuEntry().value("id").toString() == id) {
      AntiquaCRM::TabsIndex *m_tab = _iface->indexWidget(m_tabWidget);
      m_tabWidget->registerTab(m_tab);
      return true;
    }
  }
  return false;
}

void MainWindow::setViewTab(const QString &name) {
  if (tabViewAction(name))
    m_tabWidget->setCurrentTab(name);
}

void MainWindow::setAction(const QString &name, const QJsonObject &data) {
  if (name == "plugin") {
    const QString _action = data.value("ACTION").toString();
    if (_action.startsWith("provider")) {
      QString _provider = data.value("NAME").toString();
      qDebug() << Q_FUNC_INFO << "TODO" << name << _provider;
    }
    return;
  }

  if (!tabViewAction(name) || !data.contains("TARGET")) {
    qWarning("Window: Invalid Target call!");
    return;
  }

  if (!data.contains("ACTION")) {
    qWarning("Window: Missing Custom Action parameter!");
    return;
  }

  int _index = m_tabWidget->indexByName(name);
  AntiquaCRM::TabsIndex *_tab = m_tabWidget->tabIndex(_index);
  // qDebug() << Q_FUNC_INFO << name << _index;
  if (_tab == nullptr) {
    qWarning("Window: Required Tab '%s' not open!", qPrintable(name));
    m_statusBar->showMessage("The required Tab is not opened!");
    return;
  }

  const QString _action = data.value("ACTION").toString().toLower();
  if (_tab->acceptsCustomActions().contains(_action) &&
      _tab->customAction(data)) {
    m_tabWidget->setCurrentIndex(_index);
    return;
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << "REJECT" << _index << name << data;
#endif
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
  config = new AntiquaCRM::ASettings(this);
  createSocketListener();
  loadStaticTabs();

  if (!loadPluginTabs())
    m_statusBar->showMessage(tr("No tabs available"));

  if (config->contains("window/geometry"))
    restoreGeometry(config->value("window/geometry").toByteArray());

  showNormal();
  m_statusBar->showMessage(tr("Window opened"), 5000);

  return true;
}

bool MainWindow::closeWindow() {
  if (isWindowModified()) {
    AntiquaCRM::APopUpMessage *d = new AntiquaCRM::APopUpMessage(this);
    d->setUnsavedMessage(tr("Do you really want to close the application?</p>"),
                         true);

    if (d->exec() == QDialog::Rejected) {
      return false;
    }
    d->deleteLater();
  }

  QByteArray _geometry = saveGeometry();
  if (!_geometry.isNull())
    config->setValue("window/geometry", _geometry);

  _geometry.clear();

#ifdef ANTIQUA_DEVELOPEMENT
  return close();
#else
  return (m_tabWidget->unloadTabs() && close());
#endif
}

MainWindow::~MainWindow() {
  if (config != nullptr)
    config->deleteLater();

  // Destroy tab interfaces, tabs already closed in closeWindow().
  if (tabInterfaces.size() > 0) {
    for (int i = 0; i < tabInterfaces.size(); i++) {
      tabInterfaces.takeAt(i)->deleteLater();
    }
    tabInterfaces.clear();
  }
}
