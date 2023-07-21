// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mainwindow.h"
#include "menubar.h"
#include "statusbar.h"
#include "tabswidget.h"

#include <AntiquaCRM>
#include <AntiquaWidgets>

MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent} {
  setObjectName("antiqua_ui_mainwindow");
  setWindowTitle(QString(ANTIQUACRM_WINDOW_TITLE) + " [*]");
#ifdef ANTIQUA_DEVELOPEMENT
  setMinimumSize(QSize(1230, 800));
#else
  setMinimumSize(QSize(800, 600));
#endif

  m_menuBar = new MenuBar(this);
  setMenuBar(m_menuBar);

  m_viewsMenu = new AntiquaCRM::TabsMenu(m_menuBar);
  m_viewsMenu->setIcon(m_menuBar->tabIcon());
  m_menuBar->setViewsMenu(m_viewsMenu);

  m_tabWidget = new AntiquaCRM::TabsWidget(this);
  setCentralWidget(m_tabWidget);

  m_statusBar = new StatusBar(this);
  setStatusBar(m_statusBar);

  // Begin:Menu:Signals
  connect(m_menuBar, SIGNAL(sendApplicationQuit()),
          SIGNAL(sendApplicationQuit()));

  connect(m_viewsMenu, SIGNAL(sendOpenTab(const QString &)),
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
          SLOT(showMessage(const QString &)));

  return m_rx->listen(AntiquaCRM::AUtil::socketName());
}

bool MainWindow::loadTabInterfaces() {
  AntiquaCRM::TabsLoader loader(this);
  tabInterfaces = loader.interfaces(this);
  if (tabInterfaces.size() > 0) {
    QListIterator<AntiquaCRM::TabsInterface *> it(tabInterfaces);
    while (it.hasNext()) {
      AntiquaCRM::TabsInterface *_iface = it.next();
      if (_iface != nullptr) {
        QString _name = _iface->displayName();
        QString _id = _iface->interfaceName();
        m_viewsMenu->addAction(_iface->menuEntry());
        if (!_iface->addIndexOnInit())
          continue;

        AntiquaCRM::TabsIndex *_tab = _iface->indexWidget(m_tabWidget);
        if (_tab == nullptr) {
          qWarning("Plugin '%s' TabsIndex failed.", qPrintable(_name));
          qWarning("Unload '%s'", qPrintable(_id));
          _iface->deleteLater();
          continue;
        }
        m_tabWidget->registerTab(_tab);
        // windowModified
        connect(_tab, SIGNAL(sendModifiedStatus(bool)), SLOT(setChanges(bool)));
      }
    }
    return true;
  }
  return false;
}

void MainWindow::setChanges(bool b) { setWindowModified(b); }

bool MainWindow::tabViewAction(const QString &id) {
  if (!m_viewsMenu->exists(id))
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
  if (!tabViewAction(name)) {
    qWarning("Window: Invalid Action call!");
    return;
  }

  int _index = m_tabWidget->indexByName(name);
  AntiquaCRM::TabsIndex *_tab = m_tabWidget->tabIndex(_index);
  if (_tab == nullptr) {
    m_statusBar->showMessage("The required Tab is not opened!");
    return;
  }

  if (_tab->customAction(data)) {
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

  if (!loadTabInterfaces())
    m_statusBar->showMessage(tr("No tabs available"));

  if (config->contains("window/geometry"))
    restoreGeometry(config->value("window/geometry").toByteArray());

  showNormal();
  m_statusBar->showMessage(tr("Window opened"));
  return true;
}

bool MainWindow::closeWindow() {
  if (isWindowModified()) {
    QString _title = tr("Save request");
    QStringList _warn(tr("<b>You have unsaved changes.</b>"));
    _warn << tr("<p>Do you really want to close the application?</p>");
    int _ret = QMessageBox::question(this, _title, _warn.join("\n"));
    if (_ret == QMessageBox::No) {
      return false;
    }
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
