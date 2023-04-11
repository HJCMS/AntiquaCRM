// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_UI_MAINWINDOW_H
#define ANTIQUA_UI_MAINWINDOW_H

#include <AntiquaTabs>
#include <QList>
#include <QMainWindow>
#include <QObject>
#include <QPair>

class MenuBar;
class TabWidget;
class StatusBar;

class MainWindow final : public QMainWindow {
  Q_OBJECT

private:
  MenuBar *m_menuBar;
  AntiquaCRM::TabWidget *m_tabWidget;
  StatusBar *m_statusBar;

  bool loadTabInterfaces();

  void debugContent();

Q_SIGNALS:
  void sendApplicationQuit();

public:
  explicit MainWindow(QWidget *parent = nullptr);
  void openWindow();
  bool checkClose();
};

#endif // ANTIQUA_UI_MAINWINDOW_H
