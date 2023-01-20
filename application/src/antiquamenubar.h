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
class AboutDialogs;

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

  AboutDialogs *m_aboutDialogs;

private Q_SLOTS:
  void openAboutCRM();
  void openAboutQRE();
  void openAboutPgSQL();

Q_SIGNALS:
  void sendToggleFullscreen();
  void sendViewTab(const QString &);
  void sendOpenTabStatistics(const QString &);
  void sendOpenTabViews(const QString &);
  void sendOpenTabReports(const QString &);
  void sendApplQuit();

public:
  explicit AntiquaMenuBar(QMainWindow *parent = nullptr);
  ~AntiquaMenuBar();
};

#endif // ANTIQUACRM_MENUBAR_H
