/** @COPYRIGHT_HOLDER@ */

#ifndef ANTIQUACRM_MWINDOW_H
#define ANTIQUACRM_MWINDOW_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QEvent>
/* QtGui */
#include <QtGui/QCloseEvent>
/* QtWidgets */
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QStatusBar>
/* Project */
#include "searchtoolbar.h"
#include "applsettings.h"

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
    SearchToolBar *m_searchWidget;
    QStatusBar *m_statusBar;
    QWidget *m_mainWidget;
    QDockWidget *m_customSearchWidget;
    ApplSettings *m_Settings;

private Q_SLOTS:
    void initMenuBar();
    void initStatusBar();
    void initSearchDockWidget();
    void action_connect(bool);
    void openConfiguration(bool);
    void toggleFullScreen(bool);
    void closeEvent(QCloseEvent *);
    void action_closeandquit(bool);

Q_SIGNALS:
    void psqlconnect();

public:
    MWindow(QWidget *parent = nullptr);
    ~MWindow();

};
#endif // ANTIQUACRM_MWINDOW_H
