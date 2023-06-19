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
  connect(m_rx, SIGNAL(sendOperation(const QString &, const QJsonObject &)),
          SLOT(setAction(const QString &, const QJsonObject &)));
  connect(m_rx, SIGNAL(sendMessage(const QString &)), m_statusBar,
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
        AntiquaCRM::TabsIndex *_tab = _iface->indexWidget(m_tabWidget);
        QString _name = _iface->displayName();
        QString _id = _iface->interfaceName();
        if (_tab == nullptr) {
          qWarning("Plugin '%s' TabsIndex failed.", qPrintable(_name));
          qWarning("Unload '%s'", qPrintable(_id));
          _iface->deleteLater();
          continue;
        }

        if (_iface->addIndexOnInit())
          m_tabWidget->registerTab(_tab, _name);

        // SIGNALS
        connect(_tab, SIGNAL(sendModifiedStatus(bool)),
                SLOT(setTabsModified(bool)));

        // WARNING - action::objectName() must equal to tab->tabIndexId()!
        QAction *_ac = _viewMenu->addAction(_tab->windowIcon(), _name);
        _ac->setObjectName(_tab->tabIndexId());
        connect(_ac, SIGNAL(triggered()), m_tabWidget, SLOT(setCurrentTab()));
#ifdef ANTIQUA_DEVELOPEMENT
        qDebug() << Q_FUNC_INFO << _name;
#endif
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

void MainWindow::setAction(const QString &target, const QJsonObject &data) {
  int _index = m_tabWidget->indexByName(target);
  if (_index < 0) {
    m_statusBar->showMessage(tr("Operation for unknown target canceled!"));
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "!!! TODO FIND TABS !!!" << Qt::endl << data;
#endif
    return;
  }

  AntiquaCRM::TabsIndex *_widget = m_tabWidget->tabWithIndex(_index);
  if (_widget == nullptr)
    return;

  if (_widget->customAction(data))
    m_tabWidget->setCurrentIndex(_index);

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << _index << target << data;
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
