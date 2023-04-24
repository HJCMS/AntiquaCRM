// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_UI_MENUBAR_H
#define ANTIQUA_UI_MENUBAR_H

#include <QMenuBar>
#include <QIcon>
#include <QObject>
#include <QWidget>

class MenuBar final : public QMenuBar {
  Q_OBJECT

private:
  QMenu *m_applMenu;
  QMenu *m_viewsMenu;
  QMenu *m_configMenu;
  QMenu *m_aboutMenu;

Q_SIGNALS:
  void sendApplicationQuit();

public Q_SLOTS:
  void openConfiguration();

public:
  explicit MenuBar(QWidget *parent = nullptr);
  void setViewsMenu(QMenu *);

  static const QIcon tabIcon(const QString &name = QString());
};

#endif // ANTIQUA_UI_MENUBAR_H
