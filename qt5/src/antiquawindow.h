// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_MAINWINDOW_H
#define ANTIQUACRM_MAINWINDOW_H

#include <AntiquaCRM>
#include <QCloseEvent>
#include <QHideEvent>
#include <QJsonObject>
#include <QMainWindow>
#include <QMenuBar>
#include <QObject>
#include <QWidget>
#ifdef ANTIQUACRM_DBUS_ENABLED
#include <QDBusAbstractAdaptor>
#endif

class AntiquaMenuBar;
class AntiquaTabWidget;
class AntiquaStatusBar;

class AntiquaWindow : public QMainWindow {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *m_cfg;
  AntiquaMenuBar *m_menuBar;
  AntiquaStatusBar *m_statusBar;
  AntiquaTabWidget *m_centralWidget;

protected:
  void hideEvent(QHideEvent *) override;
  void closeEvent(QCloseEvent *) override;

Q_SIGNALS:
  void sendPluginOperation(const QJsonObject &);
  void sendApplQuit();

public Q_SLOTS:
  Q_INVOKABLE void setToggleWindow();
  Q_INVOKABLE void setToggleFullScreen();

public:
  explicit AntiquaWindow(QWidget *parent = nullptr);
  void openWindow();
  bool checkBeforeClose();
};

#endif // ANTIQUACRM_MAINWINDOW_H
