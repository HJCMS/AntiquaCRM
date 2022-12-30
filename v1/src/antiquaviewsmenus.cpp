// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquaviewsmenus.h"
#include "antiquatabwidget.h"
#include "reportsactiongroup.h"
#include "statisticsactiongroup.h"
#include "viewsactiongroup.h"

#include <QDebug>
#include <QIcon>
#include <QKeySequence>

AntiquaViewsMenus::AntiquaViewsMenus(QWidget *parent) : QMenu{parent} {
  setObjectName("views_menue");
  setTitle(tr("Views"));
  QIcon icon(":icons/view_log.png");

  m_tableVisit = addMenu(QIcon(":icons/tab.png"), tr("Show tab"));
  addMenu(m_tableVisit);
  addSeparator();

  m_tableReports = addMenu(icon, tr("Database Reports"));
  m_tabReports = new ReportsActionGroup(m_tableReports);
  addMenu(m_tableReports);
  addSeparator();

  // Statistics
  m_tableStats = addMenu(QIcon("://icons/kchart.png"), tr("Statistics"));
  m_tabStatistics = new StatisticsActionGroup(m_tableStats);
  addSeparator();

  // Views
  m_tableViews = addMenu(icon, tr("Database Views"));
  m_tabViews = new ViewsActionGroup(m_tableViews);
  addSeparator();

  ac_fullScreen = addAction(tr("Fullscreen"));
  ac_fullScreen->setIcon(QIcon(":icons/window_fullscreen.png"));
  ac_fullScreen->setShortcut(QKeySequence::FullScreen);

  connect(m_tableReports, SIGNAL(aboutToShow()), SLOT(uniqLoadReports()));
  connect(m_tabReports, SIGNAL(sendSelectView(const QString &)),
          SIGNAL(sendOpenReport(const QString &)));

  connect(m_tableStats, SIGNAL(aboutToShow()), SLOT(uniqLoadStatistics()));
  connect(m_tabStatistics, SIGNAL(sendSelectStats(const QString &)),
          SIGNAL(sendOpenStats(const QString &)));

  connect(m_tableViews, SIGNAL(aboutToShow()), SLOT(uniqLoadViews()));
  connect(m_tabViews, SIGNAL(sendSelectView(const QString &)),
          SIGNAL(sendOpenView(const QString &)));

  connect(ac_fullScreen, SIGNAL(triggered()), SIGNAL(sendToggleFullscreen()));

  setShowTabActions();
}

void AntiquaViewsMenus::setShowTabActions() {
  QIcon icon(":icons/tab.png");
  m_showTabsMapper = new QSignalMapper(m_tableVisit);
  m_showTabsMapper->setObjectName("tab_show_actions");
  connect(m_showTabsMapper, SIGNAL(mappedString(const QString &)),
          SIGNAL(sendShowTab(const QString &)));

  QMapIterator<QString, QString> it(AntiquaTabWidget::availableTabs());
  while (it.hasNext()) {
    it.next();
    QAction *ac = m_tableVisit->addAction(icon, it.value());
    m_showTabsMapper->setMapping(ac, it.key());
    connect(ac, SIGNAL(triggered()), m_showTabsMapper, SLOT(map()));
  }
}

void AntiquaViewsMenus::uniqLoadReports() {
  if (m_tabReports->actions().size() > 0)
    return;

  if (m_tabReports->loadDataset())
    m_tableReports->addActions(m_tabReports->actions());
}

void AntiquaViewsMenus::uniqLoadStatistics() {
  if (m_tabStatistics->actions().size() > 0)
    return;

  if (m_tabStatistics->loadDataset())
    m_tableStats->addActions(m_tabStatistics->actions());
}

void AntiquaViewsMenus::uniqLoadViews() {
  if (m_tabViews->actions().size() > 0)
    return;

  if (m_tabViews->loadDataset())
    m_tableViews->addActions(m_tabViews->actions());
}
