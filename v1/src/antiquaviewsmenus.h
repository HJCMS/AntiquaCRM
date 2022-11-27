// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_VIEWSMENUS_H
#define ANTIQUACRM_VIEWSMENUS_H

#include <QMenu>
#include <QObject>
#include <QSignalMapper>
#include <QWidget>

class ViewsActionGroup;

class AntiquaViewsMenus final : public QMenu {
  Q_OBJECT

private:
  QMenu *m_tableVisit;
  QMenu *m_tableReports;
  QMenu *m_tableViews;
  ViewsActionGroup *m_tabViews;
  QAction *ac_fullScreen;

  QSignalMapper *m_showTabsMapper;
  void setShowTabActions();

private Q_SLOTS:
  void aboutToShowViews();

Q_SIGNALS:
  void sendToggleFullscreen();
  void sendShowTab(const QString &name);
  void sendOpenView(const QString &view);

public:
  explicit AntiquaViewsMenus(QWidget *parent);
};

#endif // ANTIQUACRM_VIEWSMENUS_H