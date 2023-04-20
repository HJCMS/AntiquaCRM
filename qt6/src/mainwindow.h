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
class TabsWidget;
class StatusBar;

class MainWindow final : public QMainWindow {
  Q_OBJECT

private:
  MenuBar *m_menuBar;
  AntiquaCRM::ASettings *config = nullptr;
  AntiquaCRM::TabsWidget *m_tabWidget;
  StatusBar *m_statusBar;
  AntiquaCRM::AReceiver *m_rx;

  QList<AntiquaCRM::TabsIndex *> p_tabs;

  /**
   * @brief Der lokalen Socketserver starten.
   */
  bool createSocketListener();

  bool loadTabInterfaces();

private Q_SLOTS:
  void setTabsModified(bool);
  void setAction(const QJsonObject &obj);

Q_SIGNALS:
  void sendApplicationQuit();

public Q_SLOTS:
  Q_INVOKABLE void setToggleWindow();
  Q_INVOKABLE void setToggleFullScreen();

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  void openWindow();

  bool closeWindow();
};

#endif // ANTIQUA_UI_MAINWINDOW_H
