// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MENUBAR_H
#define ANTIQUACRM_MENUBAR_H

#include <QKeySequence>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QObject>

class AntiquaViewsMenus;
class AntiquaConfigMenus;

class AntiquaMenuBar final : public QMenuBar {
  Q_OBJECT

private:
  const QIcon defaultIcon;
  QMenu *m_applMenu;
  AntiquaViewsMenus *m_viewsMenu;
  AntiquaConfigMenus *m_configMenu;
  QMenu *m_aboutMenu;
  void addApplMenu();
  void addAboutMenu();

Q_SIGNALS:
  void sendToggleFullscreen();
  void sendViewTab(const QString &);
  void sendOpenTabViews(const QString &);
  void sendApplQuit();

public:
  explicit AntiquaMenuBar(QMainWindow *parent = nullptr);
};

#endif // ANTIQUACRM_MENUBAR_H
