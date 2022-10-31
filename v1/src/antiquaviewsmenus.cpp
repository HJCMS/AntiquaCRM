// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquaviewsmenus.h"
#include "viewsactiongroup.h"

#include <QDebug>
#include <QIcon>
#include <QKeySequence>

AntiquaViewsMenus::AntiquaViewsMenus(QWidget *parent) : QMenu{parent} {
  setObjectName("views_menue");
  setTitle(tr("Views"));
  QIcon icon(":icons/view_log.png");
  m_tabViews = nullptr;

  m_tableVisit = addMenu(QIcon(":icons/tab.png"), tr("Show tab"));
  addMenu(m_tableVisit);

  addSeparator();

  m_tableReports = addMenu(icon, tr("Database Reports"));
  addMenu(m_tableReports);

  addSeparator();

  m_tableViews = addMenu(icon, tr("Database Views"));
  m_tabViews = new ViewsActionGroup(m_tableViews);

  addSeparator();

  ac_fullScreen = addAction(tr("Fullscreen"));
  ac_fullScreen->setIcon(QIcon(":icons/window_fullscreen.png"));
  ac_fullScreen->setShortcut(QKeySequence::FullScreen);

  connect(m_tableViews, SIGNAL(aboutToShow()), SLOT(aboutToShowViews()));
  connect(m_tabViews, SIGNAL(sendSelectView(const QString &)),
          SIGNAL(sendOpenView(const QString &)));
  connect(ac_fullScreen, SIGNAL(triggered()), SIGNAL(sendToggleFullscreen()));

  setShowTabActions();
}

void AntiquaViewsMenus::setShowTabActions() {
  QIcon icon(":icons/tab.png");
  m_showTabsMapper = new QSignalMapper(m_tableVisit);
  m_showTabsMapper->setObjectName("tab_views_actions");
  connect(m_showTabsMapper, SIGNAL(mappedString(const QString &)),
          SIGNAL(sendShowTab(const QString &)));

  // Books
  QAction *ac_books = m_tableVisit->addAction(icon, tr("Books"));
  m_showTabsMapper->setMapping(ac_books, "books");
  connect(ac_books, SIGNAL(triggered()), m_showTabsMapper, SLOT(map()));
  // Customers
  QAction *ac_customers = m_tableVisit->addAction(icon, tr("Customers"));
  m_showTabsMapper->setMapping(ac_customers, "customers");
  connect(ac_customers, SIGNAL(triggered()), m_showTabsMapper, SLOT(map()));
  // Orders
  QAction *ac_orders = m_tableVisit->addAction(icon, tr("Orders"));
  m_showTabsMapper->setMapping(ac_orders, "orders");
  connect(ac_orders, SIGNAL(triggered()), m_showTabsMapper, SLOT(map()));
  // Providers
  QAction *ac_providers = m_tableVisit->addAction(icon, tr("Providers"));
  m_showTabsMapper->setMapping(ac_providers, "providers");
  connect(ac_providers, SIGNAL(triggered()), m_showTabsMapper, SLOT(map()));
}

void AntiquaViewsMenus::aboutToShowViews() {
  if (m_tabViews->actions().size() > 0)
    return;

  if (m_tabViews->loadDataset())
    m_tableViews->addActions(m_tabViews->actions());
}
