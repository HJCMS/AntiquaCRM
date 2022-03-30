/** @COPYRIGHT_HOLDER@ */

#include "version.h"
#include "mwindow.h"
/* Sub Modules */
#include "configdialog.h"

#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtWidgets/QAction>

MWindow::MWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setObjectName("MainWindow");
    setWindowTitle(ANTIQUACRM_DISPLAYNAME);
    setMinimumSize( QSize(500,350) );
    // Settings
    m_Settings = new ApplSettings();

    // Main Widget
    m_mainWidget = new QWidget(this);
    m_mainWidget->setObjectName("CentralWidget");
    setCentralWidget(m_mainWidget);

    initMenuBar();

    m_searchWidget = new SearchToolBar(this);
    addToolBar(m_searchWidget);

    initStatusBar();
    initSearchDockWidget();

    if (m_Settings)
    {
        if(m_Settings->contains("window/geometry"))
            restoreGeometry(m_Settings->value("window/geometry").toByteArray());

        if(m_Settings->contains("window/windowState"))
            restoreState(m_Settings->value("window/windowState").toByteArray());
    }
}

/**
 * @brief MWindow::initMenuBar
 */
void MWindow::initMenuBar()
{
    m_menuBar = menuBar();

    m_applicationMenu = m_menuBar->addMenu ( tr( "Application" ) );
    m_applicationMenu->setObjectName ( QLatin1String ( "ApplicationMenu" ) );

    QAction *a_dbc = m_applicationMenu->addAction(tr("DB Connect"));
    connect( a_dbc, SIGNAL ( triggered(bool) ), this, SLOT ( action_connect(bool) ) );

    m_applicationMenu->addSeparator();

    QAction *a_quit = m_applicationMenu->addAction(tr("Quit"));
    connect( a_quit, SIGNAL ( triggered(bool) ), this, SLOT ( action_closeandquit(bool) ) );

    m_viewsMenu = m_menuBar->addMenu ( tr( "Views" ) );
    m_viewsMenu->setObjectName ( QLatin1String ( "ViewsMenu" ) );

    /* TODO */
    m_viewsMenu->addAction(tr("Books"));
    m_viewsMenu->addAction(tr("Prints"));
    m_viewsMenu->addAction(tr("Custom"));
    m_viewsMenu->addSeparator();
    QAction *a_fs = m_viewsMenu->addAction(tr("Fullscreen"));
    connect( a_fs, SIGNAL ( triggered(bool) ), this, SLOT ( toggleFullScreen(bool) ) );

    m_settingsMenu = m_menuBar->addMenu ( tr( "Settings" ) );
    m_settingsMenu->setObjectName ( QLatin1String ( "SettingsMenu" ) );

    QAction *a_cfg = m_settingsMenu->addAction(tr("Configuration"));
    connect( a_cfg, SIGNAL ( triggered(bool) ), this, SLOT ( openConfiguration(bool) ) );
}

void MWindow::initStatusBar()
{
    m_statusBar = statusBar();
    m_statusBar->setObjectName("StatusBar");
    setStatusBar(m_statusBar);
}

void MWindow::initSearchDockWidget()
{
    m_customSearchWidget = new QDockWidget(m_mainWidget);
    m_customSearchWidget->setObjectName("CustomSearchWidget");
    m_customSearchWidget->setWindowTitle(tr("Extendet Search"));
    m_customSearchWidget->setMinimumSize( QSize(150,150) );
    m_customSearchWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    addDockWidget ( Qt::RightDockWidgetArea, m_customSearchWidget );
}

/**
 * @brief MWindow::action_connect
 * @param b
 * @short Wird von UI:QAction weitergeleited.
 */
void MWindow::action_connect(bool b)
{
    Q_UNUSED(b);
    emit psqlconnect();
}

/**
 * @brief MWindow::openConfiguration
 */
void MWindow::openConfiguration(bool b)
{
    Q_UNUSED(b);
    ConfigDialog *m_dialog = new ConfigDialog(this);
    m_dialog->exec();
}

/**
* Zwischen Vollansicht und Normaler Ansicht wechseln.
*/
void MWindow::toggleFullScreen(bool b)
{
  Q_UNUSED(b);
  if ( isFullScreen() )
    setWindowState ( windowState() & ~Qt::WindowFullScreen );
  else
    setWindowState ( windowState() ^ Qt::WindowFullScreen );
}

void MWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << __FUNCTION__ ;
    if ( isFullScreen() ) // Keine Vollansicht Speichern!
      setWindowState ( windowState() & ~Qt::WindowFullScreen );

    m_Settings->setValue("window/geometry", saveGeometry());
    m_Settings->setValue("window/windowState", saveState());

    QMainWindow::closeEvent(event);
}

/**
 * @brief MWindow::action_closeandquit
 * @param b
 * @short Wird von UI:QAction weitergeleited.
 */
void MWindow::action_closeandquit(bool b)
{
    Q_UNUSED(b);
    qDebug("TODO Save Quit Application");
    QCoreApplication::quit();
}

MWindow::~MWindow()
{
}
