// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sqlcore.h"
#include "sqlconfig.h"
#include "sqlconnection.h"
// DateTime Macros
#include "antiqua_global.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QLocale>
#include <QMutex>
#include <QSettings>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSslConfiguration>
#include <QTcpSocket>
#include <QtGlobal>

namespace HJCMS {

int timerInterval = 15000; /**< Timer Interval 15s (15 * 1000) */

SqlCore::SqlCore(QObject *parent) : QObject{parent} {
  setObjectName("PostgreSqlCoreDriver");
  statusTimerID = 0;
  m_cfg = new SqlConfig();
  m_socket = new SqlConnection(this);

  p_sqlOptions = m_cfg->getOptions().join(";");

  /**
   @note Wir können nur einmal die Datenbank Verbindung festlegen!
   Damit es bei weiteren Klassenaufrufen nicht Fehler hagelt.
   An dieser Stelle nach einer Exitierenden Verbindungen suchen
   und diese wieder aktivieren!
  */
  QSqlDatabase db = QSqlDatabase::database(m_cfg->getConnectioName());
  if (db.isValid()) {
    p_db = db;
    database = new QSqlDatabase(p_db);
  } else {
    p_db = QSqlDatabase(); /**< First try */
    initSQLDriver();
  }

  connect(m_socket, SIGNAL(warnMessage(const QString &)), this,
          SIGNAL(s_noticeMessage(const QString &)));
}

bool SqlCore::initSQLDriver() {
  p_db = QSqlDatabase::addDatabase("QPSQL", m_cfg->getConnectioName());
  p_db.setConnectOptions(p_sqlOptions);
  // qDebug() << Q_FUNC_INFO << p_sqlOptions;
  if (!p_db.isValid()) {
    qFatal("PostgreSQL Driver int error ...");
    return false;
  }
  database = new QSqlDatabase(p_db);
  if (database->isValid()) {
    /*
    qInfo("SqlCore::initSQLDriver QPSQL:%s loaded ...",
          qPrintable(database->connectionName()));
    qDebug() << Q_FUNC_INFO << database->connectOptions();
    */
    return true;
  }
  return false;
}

bool SqlCore::socketConnectionTest() {
  bool b = false;
  QMutex mutex;
  mutex.lock();
  b = m_socket->connect();
  mutex.unlock();
  return b;
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
    if (connName.isEmpty()) {
      qWarning("Missing SQL::ConnectionName");
      return false;
    }

    if (statusTimerID > 0)
      killTimer(statusTimerID);

    database->setHostName(m_cfg->getAddress());
    database->setPort(m_cfg->getPort());
    database->setDatabaseName(m_cfg->getDatabase());
    database->setUserName(m_cfg->getUserName());
    database->setPassword(m_cfg->getPassword());
    /**
     * Sind zwar bereits in initSQLDriver gesetzt!
     * Zur Sicherheit noch einmal.
     */
    database->setConnectOptions(p_sqlOptions);

    if (!socketConnectionTest())
      return false;

    if (database->isOpenError()) {
      return false;
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
    emit s_connectionStatus(tcp);
  }
}

const QString SqlCore::fetchErrors() {
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
    message.append(err.text());
    break;
  };
  return message;
}

void SqlCore::openDatabase(bool b) {
  Q_UNUSED(b);
  if (initDatabase()) {
    emit s_noticeMessage(tr("Database connected successfully!"));
    emit s_connectionStatus(true);
  } else {
    emit s_connectionStatus(false);
  }
}

bool SqlCore::initialDatabase() { return initDatabase(); }

const QString SqlCore::lastError() {
  QSqlError err = database->lastError();
  switch (err.type()) {
  case QSqlError::ConnectionError:
    return err.driverText();

  case QSqlError::StatementError:
    return err.databaseText();

  case QSqlError::TransactionError:
    return err.databaseText();

  default:
    return err.text();
  };
  return QString();
}

const QSqlDatabase SqlCore::db() { return p_db; }

const QSqlQuery SqlCore::query(const QString &statement) {
  if (!database->isOpen())
    database->open();

  QSqlQuery q;
  QTime t = QTime::currentTime();
  QMutex mutex;
  mutex.lock();
  q = database->exec(statement);
  mutex.unlock();
  queryTimeSpend = t.msec();
  return q;
}

const QSqlRecord SqlCore::record(const QString &table) {
  if (!database->isOpen())
    database->open();

  return database->record(table);
}

const QString SqlCore::foreignTable(const QString &table, const QString &name) {
  if (!database->isOpen())
    database->open();

  QString sql("SELECT DISTINCT b.table_name AS toTable");
  sql.append(" FROM information_schema.key_column_usage AS a");
  sql.append(" LEFT JOIN information_schema.constraint_column_usage AS b");
  sql.append(" ON b.constraint_name = a.constraint_name");
  sql.append(" WHERE (a.constraint_name LIKE '");
  sql.append(name);
  sql.append("' AND a.table_name LIKE '");
  sql.append(table);
  sql.append("');");

  QSqlQuery q = query(sql);
  if (q.next()) {
    return q.value("toTable").toString();
  }
  return QString();
}

const QStringList SqlCore::fields(const QString &table) {
  QStringList l;
  QSqlRecord r = record(table);
  if (!r.isEmpty()) {
    for (int i = 0; i < r.count(); i++) {
      if (!r.field(i).name().isEmpty())
        l.append(r.field(i).name());
    }
  }
  return l;
}

const QString SqlCore::getConnectionName() {
  return database->connectionName();
}

int SqlCore::timeSpend() { return queryTimeSpend; }

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

const QString SqlCore::whereDate(const QString &field, int past) const {
  // from Date
  QDateTime fDt(QDate::currentDate(), QTime(0, 0, 0));
  fDt = fDt.addDays(past);
  QString date_from = fDt.toString(ANTIQUA_DATETIME_FORMAT);
  // to Date
  QDateTime tDt = QDateTime::currentDateTime();
  QString date_now = tDt.toString(ANTIQUA_DATETIME_FORMAT);
  // Query
  QString clause(field + " BETWEEN '");
  clause.append(date_from + "' AND '");
  clause.append(date_now + "'");
  return clause;
}

void SqlCore::close() {
  if (database != nullptr) {
    if (database->isOpen()) {
      database->close();
      qInfo("Database '%s' closed", qPrintable(database->connectionName()));
    }
  }
}

SqlCore::~SqlCore() {
  if (statusTimerID > 0)
    killTimer(statusTimerID);
}

}; // namespace HJCMS
