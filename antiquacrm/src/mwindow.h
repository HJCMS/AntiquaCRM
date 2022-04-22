// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MWINDOW_H
#define ANTIQUACRM_MWINDOW_H

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtCore/QSignalMapper>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QAction>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>

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

public Q_SLOTS:
  void postStatusBarMessage(const QString &);

public:
  explicit MWindow(QWidget *parent = nullptr);
  virtual ~MWindow();
};
#endif // ANTIQUACRM_MWINDOW_H
