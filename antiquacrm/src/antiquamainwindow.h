#ifndef ANTIQUAMAINWINDOW_H
#define ANTIQUAMAINWINDOW_H

/* QtCore */
#include <QtCore/QObject>
#include <QtCore/QEvent>
/* QtWidgets */
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QStatusBar>
/* Project */
#include "searchtoolbar.h"

class AntiquaMainWindow : public QMainWindow
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

private Q_SLOTS:
    void initMenuBar();
    void initStatusBar();
    void initSearchDockWidget();
    void action_connect(bool);
    void toggleWindowFullScreen();
    void action_closeandquit(bool);

Q_SIGNALS:
    void psqlconnect();

public:
    AntiquaMainWindow(QWidget *parent = nullptr);
    ~AntiquaMainWindow();

};
#endif // ANTIQUAMAINWINDOW_H
