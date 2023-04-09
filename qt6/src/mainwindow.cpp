// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "mainwindow.h"
#include "menubar.h"
#include "statusbar.h"
#include "tabwidget.h"

#include "test_widget.h"

#include <AntiquaCRM>
#include <AntiquaWidgets>

MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent} {
  setObjectName("antiqua_ui_mainwindow");
  setWindowTitle(QString(ANTIQUACRM_WINDOW_TITLE) + " [*]");
  setMinimumSize(QSize(800, 550));

  m_menubar = new MenuBar(this);
  setMenuBar(m_menubar);

  m_tabwidget = new TabWidget(this);
  setCentralWidget(m_tabwidget);

  TestWidget *_test = new TestWidget(m_tabwidget);
  m_tabwidget->addTab(_test, QString("TEST"));

  m_statusbar = new StatusBar(this);
  setStatusBar(m_statusbar);
}

bool MainWindow::loadTabInterfaces() {
  AntiquaCRM::TabsLoader _loader(this);
  QList<AntiquaCRM::TabsInterface *> _list = _loader.interfaces(this);
  if (_list.size() > 0) {
    QMenu *_viewMenu = new QMenu(tr("Tabs"), m_menubar);
    _viewMenu->setIcon(m_menubar->tabIcon());

    QListIterator<AntiquaCRM::TabsInterface *> it(_list);
    while (it.hasNext()) {
      AntiquaCRM::TabsInterface *_iface = it.next();
      if (_iface != nullptr) {
        QString _name = _iface->displayName();
        QString _id = _iface->interfaceName();
        AntiquaCRM::TabsIndex *_tab = _iface->indexWidget(m_tabwidget);
        Q_CHECK_PTR(_tab);
        _tab->setObjectName("tab_" + _id);
        QIcon _icon = _tab->windowIcon();
        m_tabwidget->addTab(_tab, _icon, _name);
        QAction *_ac = _viewMenu->addAction(_icon, _name);
        _ac->setObjectName(_id);
        connect(_ac, SIGNAL(triggered()), m_tabwidget, SLOT(viewTab()));
      }
    }
    m_menubar->setViewsMenu(_viewMenu);
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

void MainWindow::openWindow() {
  // TODO - load configurations
  showNormal();

  // start plz inputs
  m_statusbar->showMessage(tr("Window opened"));

  if (!loadTabInterfaces())
    m_statusbar->showMessage(tr("No tabs available"));

#ifdef ANTIQUA_DEVELOPEMENT
  debugContent();
#endif
}

bool MainWindow::checkClose() {
  // TODO
  return true;
}
