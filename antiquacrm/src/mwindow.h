// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MWINDOW_H
#define ANTIQUACRM_MWINDOW_H

#include <QEvent>
#include <QObject>
#include <QSignalMapper>
#include <QCloseEvent>
#include <QAction>
#include <QDockWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>

class StatusBar;
class Workspace;
class ApplSettings;
class DockBarWidget;

class MWindow : public QMainWindow {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  QMenuBar *m_menuBar;
  QMenu *m_applicationMenu;
  QAction *m_quitAction;
  QAction *m_toggleFullScreen;
  StatusBar *m_statusBar;
  Workspace *m_workSpace;
  DockBarWidget *m_dockBarWidget;
  ApplSettings *m_Settings;
  QSignalMapper *m_signalMapper;
  void setupTabMenu(QMenu *parent);
  void setupActions();
  void openEditCondition();
  void openEditDesignation(const QString &section);

private Q_SLOTS:
  void openCondition(bool);
  void openBookDesignation(bool);
  void openPrintsDesignation(bool);
  void openFileDialog(bool);
  void openConfiguration(bool);
  void sqlErrorMessageBox(const QString &);
  void toggleFullScreen(bool);
  void reconnectDatabase(bool);

protected Q_SLOTS:
  virtual void closeEvent(QCloseEvent *);

Q_SIGNALS:
  void setStatusMessage(const QString &);
  void s_connectDatabase(bool);

public Q_SLOTS:
  void statusMessage(const QString &);
  void connectionStatus(bool b);

public:
  explicit MWindow(QWidget *parent = nullptr);
  void initDefaults();
  virtual ~MWindow();
};
#endif // ANTIQUACRM_MWINDOW_H
