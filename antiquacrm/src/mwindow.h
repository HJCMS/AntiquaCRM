// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MWINDOW_H
#define ANTIQUACRM_MWINDOW_H

/* QtCore */
#include <QtCore/QEvent>
#include <QtCore/QObject>
/* QtGui */
#include <QtGui/QCloseEvent>
/* QtWidgets */
#include <QtWidgets/QAction>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>

class StatusBar;
class Workspace;
class ApplSettings;
class ADockWidget;

namespace HJCMS {
 class SqlCore;
}

class MWindow : public QMainWindow {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  HJCMS::SqlCore *m_postgreSQL;
  QMenu *m_applicationMenu;
  QMenu *m_viewsMenu;
  QMenu *m_settingsMenu;
  QMenuBar *m_menuBar;
  QAction *m_quitAction;
  StatusBar *m_statusBar;
  QWidget *m_mainWidget;
  Workspace *m_workSpace;
  ADockWidget *m_adockWidget;
  ApplSettings *m_Settings;
  void createMenuTree();

private Q_SLOTS:
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
  ~MWindow();
};
#endif // ANTIQUACRM_MWINDOW_H
