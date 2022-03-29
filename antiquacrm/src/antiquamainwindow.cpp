#include "antiquamainwindow.h"

AntiquaMainWindow::AntiquaMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setObjectName("AntiquaMainWindow");
    setWindowTitle(tr("Main Window"));
    setMinimumSize( QSize(500,350) );

    m_mainWidget = new QWidget(this);
    m_mainWidget->setObjectName("CentralWidget");
    setCentralWidget(m_mainWidget);

    initMenuBar();

    m_searchWidget = new SearchToolBar(this);
    addToolBar(m_searchWidget);

    initStatusBar();
    initSearchDockWidget();

    /*
    connect( ui->actionConnect, SIGNAL ( triggered(bool) ),
             this, SLOT ( action_connect(bool) ) );
    connect( ui->actionClose, SIGNAL ( triggered(bool) ),
             this, SLOT ( action_closeandquit(bool) ) );
    */
}

/**
 * @brief AntiquaMainWindow::initMenuBar
 */
void AntiquaMainWindow::initMenuBar()
{
    m_menuBar = menuBar();

    m_applicationMenu = m_menuBar->addMenu ( tr( "Application" ) );
    m_applicationMenu->setObjectName ( QLatin1String ( "ApplicationMenu" ) );

    m_viewsMenu = m_menuBar->addMenu ( tr( "Views" ) );
    m_viewsMenu->setObjectName ( QLatin1String ( "ViewsMenu" ) );

    m_settingsMenu = m_menuBar->addMenu ( tr( "Settings" ) );
    m_settingsMenu->setObjectName ( QLatin1String ( "SettingsMenu" ) );
}

void AntiquaMainWindow::initStatusBar()
{
    m_statusBar = statusBar();
    m_statusBar->setObjectName("StatusBar");
    setStatusBar(m_statusBar);
}

void AntiquaMainWindow::initSearchDockWidget()
{
    m_customSearchWidget = new QDockWidget(m_mainWidget);
    m_customSearchWidget->setObjectName("CustomSearchWidget");
    m_customSearchWidget->setWindowTitle(tr("Extendet Search"));
    m_customSearchWidget->setMinimumSize( QSize(150,150) );
    m_customSearchWidget->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    addDockWidget ( Qt::RightDockWidgetArea, m_customSearchWidget );
}

/**
 * @brief AntiquaMainWindow::action_connect
 * @param b
 * @short Wird von UI:QAction weitergeleited.
 */
void AntiquaMainWindow::action_connect(bool b)
{
    Q_UNUSED(b);
    emit psqlconnect();
}


/**
* Zwischen Vollansicht und Normaler Ansicht wechseln.
*/
void AntiquaMainWindow::toggleWindowFullScreen()
{
  if ( isFullScreen() )
    setWindowState ( windowState() & ~Qt::WindowFullScreen );
  else
    setWindowState ( windowState() ^ Qt::WindowFullScreen );
}

/**
 * @brief AntiquaMainWindow::action_closeandquit
 * @param b
 * @short Wird von UI:QAction weitergeleited.
 */
void AntiquaMainWindow::action_closeandquit(bool b)
{
    Q_UNUSED(b);
    qDebug("TODO Save Quit Application");
}

AntiquaMainWindow::~AntiquaMainWindow()
{
}
