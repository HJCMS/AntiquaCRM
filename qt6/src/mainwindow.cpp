// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mainwindow.h"
#include "menubar.h"
#include "statusbar.h"
#include "tabwidget.h"

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

  m_tabWidget = new AntiquaCRM::TabWidget(this);
  setCentralWidget(m_tabWidget);

  m_statusBar = new StatusBar(this);
  setStatusBar(m_statusBar);

  connect(m_menuBar, SIGNAL(sendApplicationQuit()),
          SIGNAL(sendApplicationQuit()));
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

        QAction *_ac = _viewMenu->addAction(_tab->windowIcon(), _name);
        _ac->setObjectName(_tab->tabIndexId());
        connect(_ac, SIGNAL(triggered()), m_tabWidget, SLOT(setViewTab()));
      }
    }
    m_menuBar->setViewsMenu(_viewMenu);
    _list.clear();
    return true;
  }
  return false;
}

void MainWindow::debugContent() {
  // QRegularExpression pattern("^[a-z]{1,}_[a-z_]{2,}");
  // QList<AntiquaCRM::AbstractInput *> list =
  //     centralWidget()->findChildren<AntiquaCRM::AbstractInput *>(pattern);
  // for (int i = 0; i < list.size(); i++) {
  //   AntiquaCRM::AbstractInput *obj = list.at(i);
  //   if (obj != nullptr) {
  //     qDebug() << obj->objectName() << obj->getValue();
  //   }
  // }
}

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

void MainWindow::openWindow() {
  // TODO - load configurations
  showNormal();

  createSocketListener();

  // start plz inputs
  m_statusBar->showMessage(tr("Window opened"));

  if (!loadTabInterfaces())
    m_statusBar->showMessage(tr("No tabs available"));

#ifdef ANTIQUA_DEVELOPEMENT
  debugContent();
#endif
}

bool MainWindow::checkClose() {
  // TODO
  return true;
}
