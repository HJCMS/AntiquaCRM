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
  QList<AntiquaCRM::TabsInterface *> _list = loader.interfaces(this);
  if (_list.size() > 0) {
    QListIterator<AntiquaCRM::TabsInterface *> it(_list);
    while (it.hasNext()) {
      AntiquaCRM::TabsInterface *_iface = it.next();
      if (_iface != nullptr) {
        AntiquaCRM::TabsIndex *_tab = _iface->indexWidget(m_tabWidget);
        QString _name = _iface->displayName();
        QString _id = _iface->interfaceName();
        if (_tab == nullptr) {
          qWarning("Plugin '%s' TabsIndex failed.", qPrintable(_name));
          qWarning("Unload '%s'", qPrintable(_id));
          _iface->deleteLater();
          continue;
        }

        // create menu
        m_viewsMenu->addAction(_tab);

        // load tab on start
        if (_iface->addIndexOnInit())
          m_tabWidget->registerTab(_tab);

        // SIGNALS
        connect(_tab, SIGNAL(sendModifiedStatus(bool)), SLOT(setChanges(bool)));

        p_tabsIndexList.append(_tab);
      }
    }
    _list.clear();
    return true;
  }
  return false;
}

void MainWindow::setChanges(bool b) { setWindowModified(b); }

bool MainWindow::tabViewAction(const QString &id) {
  bool _status = false;
  QAction *m_ac = m_viewsMenu->findChild<QAction *>(id);
  if (m_ac == nullptr) {
    qWarning("Views tab %s not found in Menu!!", qPrintable(id));
    return _status;
  }

  if (m_tabWidget->indexByName(id) < 0) {
    QListIterator<AntiquaCRM::TabsIndex *> it(p_tabsIndexList);
    while (it.hasNext()) {
      AntiquaCRM::TabsIndex *_t = it.next();
      if (_t->objectName().startsWith(id)) {
        m_tabWidget->registerTab(_t);
        _status = true;
        break;
      }
    }
  } else {
    _status = true;
  }
  return _status;
}

void MainWindow::setViewTab(const QString &name) {
  if (tabViewAction(name))
    m_tabWidget->setCurrentTab(name);
}

void MainWindow::setAction(const QString &name, const QJsonObject &data) {
  if (!tabViewAction(name)) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "!!! UNKNOWN TAB CALL !!!" << data;
#endif
    return;
  }

  int _index = m_tabWidget->indexByName(name);
  AntiquaCRM::TabsIndex *_widget = m_tabWidget->tabIndex(_index);
  if (_widget == nullptr) {
    qInfo("TabsIndex not opened");
    return;
  }

  if (_widget->customAction(data))
    m_tabWidget->setCurrentIndex(_index);

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << _index << name << data;
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
  p_tabsIndexList.clear();
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

  // Destroy Tabs list, tabs already closed in closeWindow().
  if (p_tabsIndexList.size() > 0) {
    for (int i = 0; i < p_tabsIndexList.size(); i++) {
      p_tabsIndexList.takeAt(i)->deleteLater();
    }
    p_tabsIndexList.clear();
  }
}
