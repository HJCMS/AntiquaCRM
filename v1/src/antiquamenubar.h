// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MENUBAR_H
#define ANTIQUACRM_MENUBAR_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QObject>
#include <QKeySequence>

class AntiquaMenuBar final : public QMenuBar {
  Q_OBJECT

private:
  const QIcon defaultIcon;
  QMenu *m_applMenu;
  QMenu *m_viewsMenu;
  QMenu *m_configMenu;
  QMenu *m_aboutMenu;
  void addApplMenu();
  void addViewsMenu();
  void addConfigMenu();
  void addAboutMenu();

Q_SIGNALS:
  void sendApplQuit();

public:
  explicit AntiquaMenuBar(QMainWindow *parent = nullptr);
};

#endif // ANTIQUACRM_MENUBAR_H
