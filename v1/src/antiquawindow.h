// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MAINWINDOW_H
#define ANTIQUACRM_MAINWINDOW_H

#include <AntiquaCRM>
#include <QCloseEvent>
#include <QHideEvent>
#include <QMainWindow>
#include <QMenuBar>
#include <QObject>
#include <QWidget>

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

private Q_SLOTS:
  void closeWindow();

protected:
  void hideEvent(QHideEvent *) override;
  void closeEvent(QCloseEvent *) override;

Q_SIGNALS:
  void sendApplQuit();

public Q_SLOTS:
  void setToggleWindow();
  void setToggleFullScreen();

public:
  explicit AntiquaWindow(QWidget *parent = nullptr);
  void openWindow();
};

#endif // ANTIQUACRM_MAINWINDOW_H
