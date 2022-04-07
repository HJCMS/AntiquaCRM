// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sqlcore.h"
#include "sqlconfig.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtNetwork/QTcpSocket>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

namespace HJCMS {

int timerInterval = 15000; /**< Timer Interval 15s (15 * 1000) */

SqlCore::SqlCore(QObject *parent) : QObject{parent} {
  setObjectName("PostgreSqlCoreDriver");
  statusTimerID = 0;
  // dummy
  p_db = QSqlDatabase();
  // load nativ settings class
  m_cfg = new SqlConfig();
  // load driver
  initSQLDriver();
}

const QString SqlCore::prepareErrorMessage() {
  QString message("Unknown SQL error:");

  if (database == nullptr) {
    return tr("SQL Driver initialization error");
  }

  QSqlError err = database->lastError();
  switch (err.type()) {
  case QSqlError::ConnectionError:
    message = "SqlCore::ConnectionError:";
    message.append(err.driverText());
    break;

  case QSqlError::StatementError:
    message = "SqlCore::StatementError:";
    message.append(err.databaseText());
    break;

  case QSqlError::TransactionError:
    message = "SqlCore::TransactionError:";
    message.append(err.databaseText());
    break;

  default:
    break;
  };
  return message;
}

bool SqlCore::initSQLDriver() {
  p_db = QSqlDatabase::addDatabase("QPSQL", m_cfg->getConnectioName());
  if (!p_db.isValid()) {
    qFatal("PostgreSQL Driver int error ...");
    return false;
  }
  database = new QSqlDatabase(p_db);
  qInfo("PostgreSQL Driver QPSQL:%s loaded ...",
        qPrintable(database->connectionName()));
  return true;
}

bool SqlCore::socketConnectionTest() {
  QTcpSocket *m_socket = new QTcpSocket(this);
  m_socket->connectToHost(m_cfg->getAddress(), m_cfg->getPort());
  if (!m_socket->waitForConnected(5)) {
    QString err = QString("Server not responding on Port:");
    err.append(QString::number(database->port()));
    err.append(" and Address:'");
    err.append(database->hostName());
    err.append("' - ");
    err.append(m_socket->errorString());
    emit s_databaseMessage(err);
    return false;
  }
  m_socket->close();
  return true;
}

/**
   @brief Initialisiere und öffne die Datenbank.
   Prüfen ob der Datenbank Treiber noch nicht initialisiert
   werden konnte. Wenn dies der fall ist, noch mal den Treiber
   mit @ref initSQLDriver() aufrufen.
   @li Bei der ersten Verbindung wird die Datenbank geöffnet.
   @li Bei weiteren abfragen wird nur auf isValid() geprüft!
   @return QSqlDatabase::isValid()
 */
bool SqlCore::initDatabase() {
  if (!p_db.isValid()) {
    if (!initSQLDriver())
      return false;
  }

  if (database != nullptr) {
    QString connName = database->connectionName();
    if (connName.isEmpty())
      return false;

    if (statusTimerID > 0)
      killTimer(statusTimerID);

    database->setHostName(m_cfg->getAddress());
    database->setPort(m_cfg->getPort());
    database->setDatabaseName(m_cfg->getDatabase());
    database->setUserName(m_cfg->getUserName());
    database->setPassword(m_cfg->getPassword());
    database->setConnectOptions("connect_timeout=5");

    if (!socketConnectionTest())
      return false;

    if (database->isOpenError()) {
      prepareErrorMessage();
    }

    if (database->isValid()) {
      statusTimerID = startTimer(timerInterval);
      return true;
    }
  }
  return false;
}

void SqlCore::timerEvent(QTimerEvent *ev) {
  if (ev->timerId() > 0) {
    bool tcp = socketConnectionTest();
    /*
    qDebug() << "Timer Start ID:" << statusTimerID
             << "Timer Event ID:" << ev->timerId()
             << "sql query connections exists:" << database->isOpen()
             << "Socket:" << tcp;
    */
    emit s_connectionStatus(tcp);
  }
}

void SqlCore::openDatabase(bool b) {
  Q_UNUSED(b);
  if (initDatabase()) {
    emit s_databaseMessage(tr("PostgreSQL connected successfully!"));
    emit s_connectionStatus(true);
  } else {
    emit s_connectionStatus(false);
  }
}

const QString SqlCore::getConnectionName() {
  return database->connectionName();
}

/**
   @brief Statusabfrage ob die Datenbank offen ist!
   @note
    Wenn die Datenbank nicht Initialisiert werden konnte.
    Wird die Funktion auch \b false zurück geben!
    Das verhindert Abstürtze wegen fehlerhaften Zeiger.
   @return bool
*/
bool SqlCore::sqlDriversExists() {
  if (database != nullptr) {
    return database->isValid();
  } else {
    return false;
  }
}

SqlCore::~SqlCore() {
  if (database != nullptr) {
    if (database->isOpen()) {
      qInfo("Database '%s' closed", qPrintable(database->connectionName()));
      database->close();
    }
  }
  if (statusTimerID > 0)
    killTimer(statusTimerID);
}

}; // namespace HJCMS
