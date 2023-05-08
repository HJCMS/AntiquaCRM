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

  m_tabWidget = new AntiquaCRM::TabsWidget(this);
  setCentralWidget(m_tabWidget);

  m_statusBar = new StatusBar(this);
  setStatusBar(m_statusBar);

  connect(m_menuBar, SIGNAL(sendApplicationQuit()),
          SIGNAL(sendApplicationQuit()));

  connect(m_tabWidget, SIGNAL(sendMessage(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));
}

MainWindow::~MainWindow() {
  if (config != nullptr)
    config->deleteLater();
}

bool MainWindow::createSocketListener() {
  m_rx = new AntiquaCRM::AReceiver(this);
  connect(m_rx, SIGNAL(sendWindowOperation(const QJsonObject &)),
          SLOT(setAction(const QJsonObject &)));
  connect(m_rx, SIGNAL(sendInfoMessage(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));
  connect(m_rx, SIGNAL(sendWarnMessage(const QString &)), m_statusBar,
          SLOT(showMessage(const QString &)));

  return m_rx->listen(AntiquaCRM::AUtil::socketName());
}

bool MainWindow::loadTabInterfaces() {
  AntiquaCRM::TabsLoader _loader(this);
  QList<AntiquaCRM::TabsInterface *> _list = _loader.interfaces(this);
  if (_list.size() > 0) {
    QMenu *_viewMenu = new QMenu(tr("Tabs"), m_menuBar);
    _viewMenu->setIcon(m_menuBar->tabIcon());

    QListIterator<AntiquaCRM::TabsInterface *> it(_list);
    while (it.hasNext()) {
      AntiquaCRM::TabsInterface *_iface = it.next();
      if (_iface != nullptr) {
        QString _name = _iface->displayName();
        // QString _id = _iface->interfaceName();
        AntiquaCRM::TabsIndex *_tab = _iface->indexWidget(m_tabWidget);
        Q_CHECK_PTR(_tab);
        if (_iface->addIndexOnInit())
          m_tabWidget->registerTab(_tab, _name);

        // SIGNALS
        connect(_tab, SIGNAL(sendModifiedStatus(bool)),
                SLOT(setTabsModified(bool)));

        QAction *_ac = _viewMenu->addAction(_tab->windowIcon(), _name);
        _ac->setObjectName(_tab->tabIndexId());
        connect(_ac, SIGNAL(triggered()), m_tabWidget, SLOT(setViewTab()));
        p_tabs.append(_tab);
      }
    }
    m_menuBar->setViewsMenu(_viewMenu);
    _list.clear();
    return true;
  }
  return false;
}

void MainWindow::setTabsModified(bool b) { setWindowModified(b); }

void MainWindow::setAction(const QJsonObject &obj) {
  if (obj.contains("window_operation")) {
    QString _name = obj.value("tab").toString();
    int _index = m_tabWidget->indexByName(_name);
    AntiquaCRM::TabsIndex *_widget = m_tabWidget->tabWithIndex(_index);
    if (_widget == nullptr)
      return;

    if (_widget->customAction(obj))
      m_tabWidget->setCurrentIndex(_index);
  }
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

void MainWindow::openWindow() {
  p_tabs.clear();
  config = new AntiquaCRM::ASettings(this);

  createSocketListener();

  if (!loadTabInterfaces())
    m_statusBar->showMessage(tr("No tabs available"));

  if (config->contains("window/geometry"))
    restoreGeometry(config->value("window/geometry").toByteArray());

  showNormal();
  m_statusBar->showMessage(tr("Window opened"));
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
