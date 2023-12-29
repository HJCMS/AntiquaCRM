// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlcore.h"
#include "asqlprofile.h"
#include "asqlsettings.h"

#include <QDateTime>
#include <QDebug>
#include <QMutexLocker>
#include <QSysInfo>

namespace AntiquaCRM {

inline QMutex ASqlCore::s_mutex;

ASqlCore::ASqlCore(QObject *parent) : QObject{parent} {
  setObjectName("antiquacrm_sqlcore");
  config = new ASqlSettings(this);
  database = nullptr;

  QSqlDatabase db = QSqlDatabase::database(config->connectionName());
  if (db.isValid()) {
    database = new QSqlDatabase(db);
  } else {
    initDatabase();
  }
}

bool ASqlCore::initDatabase() {
  if (database != nullptr)
    return database->isValid();

  // https://www.postgresql.org/docs/current/libpq-connect.html
  QSqlDatabase db =
      QSqlDatabase::addDatabase("QPSQL", config->connectionName());
  ASqlProfile profile = config->connectionProfile();
  db.setHostName(profile.getHostname());
  db.setPort(profile.getPort());
  db.setDatabaseName(profile.getDatabaseName());
  db.setUserName(profile.getUsername());
  db.setPassword(profile.getPassword());

  QStringList options;
  options << QString("connect_timeout=%1").arg(profile.getTimeout());
  options << QString("application_name=%1").arg(identifier());
  options << QString("target_session_attrs=read-write");

  if (profile.getEnableSSL()) {
    options << QString("sslsni=1");
    options << QString("sslmode=%1").arg(profile.getSslMode());
    options << QString("sslrootcert=%1").arg(profile.getSslRootCert());
    options << QString("ssl_min_protocol_version=%1").arg("TLSv1.2");
  }
  db.setConnectOptions(options.join(";"));

  if (!db.isValid())
    return false;

  if (database == nullptr)
    database = new QSqlDatabase(db);

  if (database->isOpenError()) {
    prepareSqlError(database->lastError());
    return false;
  }
  qInfo("Database connected to Host '%s'.", qPrintable(database->hostName()));
  return database->isOpen();
}

bool ASqlCore::isConnected() {
  if (database->isOpen())
    return true;

  if (database->open())
    return true;

  QSqlError err = database->lastError();
  if (err.isValid())
    prepareSqlError(err);

  return false;
}

void ASqlCore::prepareSqlError(const QSqlError &error) {
  switch (error.type()) {
  case QSqlError::NoError:
    return;

  case QSqlError::StatementError:
    qWarning("SqlError::Statement");
    emit sendStatementError(error);
    break;

  case QSqlError::ConnectionError:
    qWarning("SqlError::Connection");
    emit sendConnectionError(error);
    break;

  // case QSqlError::TransactionError:
  // case QSqlError::UnknownError:
  default:
    break;
  };

// Developement verbose
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << "Type:" << error.type() << Qt::endl
           << "Database: " << error.databaseText() << Qt::endl
           << "Driver: " << error.driverText() << Qt::endl
           << "Code: " << error.nativeErrorCode() << Qt::endl
           << "Text: " << error.text();
#endif
}

bool ASqlCore::status() {
  if (!database->isOpen())
    return false;

  QString _select("SELECT state FROM pg_stat_activity WHERE");
  _select.append(" application_name='" + identifier() + "'");
  _select.append(" AND state IS NOT NULL;");
  return (query(_select).size() > 0);
}

const QString ASqlCore::identifier() {
  QString _name("AntiquaCRM_v");
  _name.append(ANTIQUACRM_VERSION);
  _name.append("_");
  _name.append(QSysInfo::machineHostName());
  return _name.trimmed();
}

const QString ASqlCore::getDateTime() const {
  return QDateTime::currentDateTime().toString();
}

const QString ASqlCore::getTimeStamp() {
  QMutexLocker locker(&s_mutex);
  QSqlQuery _q = ASqlCore::query("SELECT CURRENT_TIMESTAMP;");
  if (_q.size() == 1) {
    _q.next();
    return _q.value(0).toString();
  }
  qWarning("ASqlCore::getTimeStamp - fallback to QDateTime");
  return QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm.dd.ssss t");
}

bool ASqlCore::open() {
  if (database->isOpen())
    return true;

  return initDatabase();
}

const QSqlDatabase ASqlCore::db() {
  return QSqlDatabase::database(config->connectionName(), isConnected());
}

const QSqlRecord ASqlCore::record(const QString &table) {
  QMutexLocker locker(&s_mutex);
  if (!isConnected())
    return QSqlRecord();

  QSqlRecord re;
  re = database->record(table);
  return re;
}

const QStringList ASqlCore::fieldNames(const QString &table) {
  QMutexLocker locker(&s_mutex);
  if (!isConnected())
    return QStringList();

  QStringList l;
  QSqlRecord re = record(table);
  if (!re.isEmpty()) {
    for (int i = 0; i < re.count(); i++) {
      if (re.field(i).isValid())
        l.append(re.field(i).name());
    }
  }
  return l;
}

const QSqlQuery ASqlCore::query(const QString &statement) {
  QMutexLocker locker(&s_mutex);
  if (!isConnected() || statement.isEmpty())
    return QSqlQuery();

  QSqlQuery _query;
  _query = database->exec(statement);

  if (_query.lastError().isValid())
    prepareSqlError(_query.lastError());

  return _query;
}

const QString ASqlCore::lastError() {
  QSqlError _error = database->lastError();
  if (_error.isValid())
    return _error.text();

  return QString();
}

void ASqlCore::close() {
  QMutexLocker locker(&s_mutex);
  if (database != nullptr && database->isOpen()) {
    qInfo("Close Database to '%s'.", qPrintable(database->hostName()));
    database->close();
  }
}

}; // namespace AntiquaCRM
