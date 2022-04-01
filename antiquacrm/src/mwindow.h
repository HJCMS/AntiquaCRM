// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_MWINDOW_H
#define ANTIQUACRM_MWINDOW_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QEvent>
/* QtGui */
#include <QtGui/QCloseEvent>
/* QtWidgets */
#include <QtWidgets/QAction>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QDockWidget>

class StatusBar;
class Workspace;
class ApplSettings;
class ADockWidget;

class MWindow : public QMainWindow
{
    Q_OBJECT
    Q_CLASSINFO ( "Author", "Jürgen Heinemann" )
    Q_CLASSINFO ( "URL", "http://www.hjcms.de" )

private:
    QMenu* m_applicationMenu;
    QMenu* m_viewsMenu;
    QMenu* m_settingsMenu;
    QMenuBar *m_menuBar;
    QAction *m_quitAction;
    StatusBar *m_statusBar;
    QWidget *m_mainWidget;
    Workspace *m_workSpace;
    ADockWidget *m_adockWidget;
    ApplSettings *m_Settings;
    void createMenuBars();

private Q_SLOTS:
    void openConfiguration(bool);
    void toggleFullScreen(bool);

protected Q_SLOTS:
    virtual void closeEvent(QCloseEvent *);

Q_SIGNALS:
    void sqlconnect(bool);
    void setStatusMessage(const QString &);

public:
    explicit MWindow(QWidget *parent = nullptr);
    ~MWindow();

};
#endif // ANTIQUACRM_MWINDOW_H
