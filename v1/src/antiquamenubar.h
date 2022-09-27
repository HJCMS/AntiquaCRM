// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MENUBAR_H
#define ANTIQUACRM_MENUBAR_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QObject>

class AntiquaMenuBar final : public QMenuBar {
  Q_OBJECT

private:
  QMenu *m_applMenu;
  QMenu *m_viewsMenu;
  QMenu *m_configMenu;
  QMenu *m_aboutMenu;

public:
  explicit AntiquaMenuBar(QMainWindow *parent = nullptr);
};

#endif // ANTIQUACRM_MENUBAR_H
