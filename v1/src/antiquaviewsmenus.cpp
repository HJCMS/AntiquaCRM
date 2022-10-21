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
}

void AntiquaViewsMenus::aboutToShowViews() {
  if (m_tabViews->actions().size() > 0)
    return;

  if (m_tabViews->loadDataset())
    m_tableViews->addActions(m_tabViews->actions());
}
