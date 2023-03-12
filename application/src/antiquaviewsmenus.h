// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_VIEWSMENUS_H
#define ANTIQUACRM_VIEWSMENUS_H

#include <QMenu>
#include <QMenuBar>
#include <QObject>
#include <QSignalMapper>
#include <QWidget>

class ReportsActionGroup;
class ViewsActionGroup;
class StatisticsMenu;

/**
 * @ingroup ui
 * @brief Menu for all SQL Views @class ViewsActionGroup
 */
class AntiquaViewsMenus final : public QMenu {
  Q_OBJECT

private:
  QMenu *m_tableVisit;
  QMenu *m_tableReports;
  QMenu *m_tableViews;
  StatisticsMenu *m_tabStatsMenu;
  ReportsActionGroup *m_tabReports;
  ViewsActionGroup *m_tabViews;
  QAction *ac_fullScreen;

  QSignalMapper *m_showTabsMapper;
  void setShowTabActions();

private Q_SLOTS:
  void uniqLoadReports();
  void uniqLoadViews();

Q_SIGNALS:
  void sendToggleFullscreen();
  void sendShowTab(const QString &);
  void sendOpenView(const QString &);
  void sendOpenStats(const QString &);
  void sendOpenReport(const QString &);

public:
  explicit AntiquaViewsMenus(QMenuBar *parent);
};

#endif // ANTIQUACRM_VIEWSMENUS_H
