/** @COPYRIGHT_HOLDER@ */

#include "version.h"
#include "mapplication.h"

#include <QDebug>

MApplication::MApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
    setObjectName("MApplicationApplication");
    setQuitOnLastWindowClosed(true);
    // TODO setWindowIcon(const QIcon &icon);
    m_mainWindow = new MWindow();
    connect( m_mainWindow, SIGNAL ( psqlconnect() ), this, SLOT ( connectdb() ) );
}

/**
 * @brief MApplication::initSettings
 * @todo  Read Default Setting
 */
void MApplication::initSettings()
{

}

/**
 * @brief MApplication::initDatabase
 * @todo Options PostgreSQL Database:
 *      (connect_timeout,options,tty,requiressl,service)
 *      https://www.postgresql.org/docs/current/functions.html
 * @test SELECT schemaname,relname FROM pg_stat_user_tables WHERE relname IS NOT NULL;
 */
bool MApplication::initDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL",sqlConnectionName);
    if(db.isValid())
    {
        m_db = new QSqlDatabase(db);
        m_db->setHostName("pvserver.hjcms");
        m_db->setPort(5432);
        m_db->setDatabaseName("heinemann");
        m_db->setUserName("heinemann");
        m_db->setPassword("dg3pdjheinema");
        m_db->setConnectOptions("connect_timeout=2");
        if(m_db->open())
        {
            qInfo("'%s' Connected to PostgerSQL Database",
                  qPrintable(m_db->connectionName()));
            qDebug() << "Tables: " << m_db->tables(QSql::Tables);
            return true;
        } else {
           qWarning("Connect to PostgerSQL Database failed.");
           m_db->setConnectOptions();
        }
    } else {
        qFatal("Can not load SQL::Driver Module!");
    }
    return false;
}

/**
 * @brief MApplication::connectdb
 */
void MApplication::connectdb()
{
    if(m_db)
    {
        if(m_db->contains(sqlConnectionName)) {
            qInfo("Already connected to SQL Server ...");
            return;
        } else if ( initDatabase() ) {
            qInfo("Reconnected to SQL Server");
        }
    }
}

/**
 * @brief MApplication::exec
 * @return int
 */
int MApplication::exec()
{
    if(initDatabase())
    {
        m_mainWindow->show();
        return QApplication::exec();
    } else {
       qDebug("%s", qPrintable("No SQL Connection"));
    }
    return 0;
}

MApplication::~MApplication()
{
    if(m_db->contains(sqlConnectionName))
    {
        qDebug("SQL '%s' Disonnected", qPrintable(sqlConnectionName));
        m_db->close();
    }
}
