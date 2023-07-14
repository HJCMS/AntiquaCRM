// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlcore.h"
#include "asqlprofile.h"
#include "asqlsettings.h"

#include <QDebug>
#include <QSysInfo>

namespace AntiquaCRM {

ASqlCore::ASqlCore(QObject *parent) : QObject{parent} {
  setObjectName("antiquacrm_sqlcore");
  m_cfg = new ASqlSettings(this);
  database = nullptr;

  QSqlDatabase db = QSqlDatabase::database(m_cfg->connectionName());
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
  QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", m_cfg->connectionName());
  ASqlProfile profile = m_cfg->connectionProfile();
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
    qWarning("Missing SQL::Connection");
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

  qWarning("Can't open Database Connection!");
  QSqlError err = database->lastError();
  if (err.isValid())
    prepareSqlError(err);

  return false;
}

void ASqlCore::prepareSqlError(const QSqlError &error) {
  const QSqlError::ErrorType _type = error.type();
  if (_type == QSqlError::NoError)
    return;

  // Only send statement errors
  if (_type == QSqlError::StatementError)
    emit sendStatementError(error);

// Developement verbose
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << Qt::endl
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

bool ASqlCore::open() {
  if (database->isOpen())
    return true;

  return initDatabase();
}

const QSqlDatabase ASqlCore::db() {
  return QSqlDatabase::database(m_cfg->connectionName(), isConnected());
}

const QSqlRecord ASqlCore::record(const QString &table) {
  if (!isConnected())
    return QSqlRecord();

  QSqlRecord re;
  p_mutex.lock();
  re = database->record(table);
  p_mutex.unlock();
  return re;
}

const QStringList ASqlCore::fieldNames(const QString &table) {
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
  if (!isConnected())
    return QSqlQuery();

  QSqlQuery _query;
  p_mutex.lock();
  _query = database->exec(statement);
  p_mutex.unlock();

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
  if (database != nullptr && database->isOpen()) {
    qInfo("Close Database to '%s'.", qPrintable(database->hostName()));
    database->close();
  }
}

}; // namespace AntiquaCRM
