// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MWINDOW_H
#define ANTIQUACRM_MWINDOW_H

#include <QtCore/QEvent>
#include <QtCore/QObject>
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

namespace HJCMS {
class SqlCore;
}

class MWindow : public QMainWindow {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  HJCMS::SqlCore *m_postgreSQL;
  QMenuBar *m_menuBar;
  QMenu *m_applicationMenu;
  QMenu *m_viewsMenu;
  QMenu *m_settingsMenu;
  QMenu *m_tablesMenu;
  QAction *m_quitAction;
  StatusBar *m_statusBar;
  Workspace *m_workSpace;
  DockBarWidget *m_dockBarWidget;
  ApplSettings *m_Settings;
  void setupActions();

private Q_SLOTS:
  void openFileDialog(bool);
  void openEditCondition(bool);
  void openEditDesignation(bool);
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
  void connectSqlDatabase();
  virtual ~MWindow();
};
#endif // ANTIQUACRM_MWINDOW_H
