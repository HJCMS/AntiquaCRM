// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MAINWINDOW_H
#define ANTIQUACRM_MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QMenuBar>
#include <QEvent>
#include <QWidget>

class AntiquaMenuBar;
class AntiquaTabWidget;
class AntiquaStatusBar;

class AntiquaWindow : public QMainWindow {
  Q_OBJECT

private:
  AntiquaMenuBar *m_menuBar;
  AntiquaStatusBar *m_statusBar;
  AntiquaTabWidget *m_centralWidget;

protected:
  void closeEvent(QCloseEvent *) override;

public:
  explicit AntiquaWindow(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_MAINWINDOW_H
