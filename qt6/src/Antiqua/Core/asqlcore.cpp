// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlcore.h"
#include "anetworkiface.h"
#include "asqlprofile.h"
#include "asqlsettings.h"

#include <QDateTime>
#include <QDebug>
#include <QSysInfo>

namespace AntiquaCRM
{

ASqlCore::ASqlCore(QObject* parent, const QString& profile) : QObject{parent} {
  setObjectName("antiquacrm_sqlcore");
  config = new ASqlSettings(this, profile);
  database = nullptr;

  QSqlDatabase db = QSqlDatabase::database(config->connectionName());
  if (db.isValid()) {
    database = new QSqlDatabase(db);
  } else {
    initDatabase();
  }
}

const QSqlError ASqlCore::sqlNetworkError() const {
  QSqlError _err(tr("Service is unreachable!"), // driver
                 tr("The database cannot be reached via the network.\n"
                    "Please check your network connection."),
                 QSqlError::ConnectionError, "503");
  return _err;
}

bool ASqlCore::initDatabase() {
  if (!networkStatus()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Network interface is down!";
#endif
    return false;
  }

  if (database != nullptr)
    return database->isValid();

  // https://www.postgresql.org/docs/current/libpq-connect.html
  QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", config->connectionName());
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
    QString _crt = profile.getSslRootCert();
    if (_crt.isEmpty())
      options << QString("sslrootcert=%1").arg("system");
    else
      options << QString("sslrootcert=%1").arg(_crt);
  }
  db.setConnectOptions(options.join(";"));

  if (!db.isValid())
    return false;

  if (database == nullptr)
    database = new QSqlDatabase(db);

  if (database->isOpenError()) {
    qWarning("Database open errors ...");
    prepareSqlError(database->lastError());
    return false;
  }

  qInfo("Database connected to Host '%s'.", qPrintable(database->hostName()));
  return isConnected();
}

bool ASqlCore::isConnected() {
  if (!networkStatus()) {
    emit sendStatementError(sqlNetworkError());
    return false;
  }

  if (database->isOpen())
    return true;

  if (database->open())
    return true;

  QSqlError err = database->lastError();
  if (err.isValid())
    prepareSqlError(err);

  return false;
}

void ASqlCore::prepareSqlError(const QSqlError& error) {
  switch (error.type()) {
    case QSqlError::StatementError:
      qWarning("ASqlCore::StatementError");
      break;

    case QSqlError::ConnectionError:
      qWarning("ASqlCore::ConnectionError");
      break;

    default:
      qWarning("ASqlCore::PostgreSQL::SqlError");
      break;
  };

// Developement verbose
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << Qt::endl
           << "Type:" << error.type() << Qt::endl
           << "Database: " << error.databaseText() << Qt::endl
           << "Driver: " << error.driverText() << Qt::endl
           << "Code: " << error.nativeErrorCode();
#endif
  emit sendStatementError(error);
}

bool ASqlCore::status() {
  if (!networkStatus()) {
    emit sendStatementError(sqlNetworkError());
    return false;
  }

  if (!database->isOpen())
    return false;

  QString _select("SELECT state FROM pg_stat_activity WHERE");
  _select.append(" application_name='" + identifier() + "'");
  _select.append(" AND state IS NOT NULL;");
  return (query(_select).size() > 0);
}

bool ASqlCore::networkStatus() {
  AntiquaCRM::ANetworkIface iface;
  if (iface.connectedIfaceExists())
    return true;

  return false;
}

qint64 ASqlCore::getQueryLimit() {
  return config->value("database/SqlQueryLimit", 900).toInt();
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
  const QString _format("dd.MM.yyyy hh:mm.dd.ssss t");
  QDateTime _dt = QDateTime::currentDateTime();
  QSqlQuery _q = ASqlCore::query("SELECT CURRENT_TIMESTAMP;");
  if (_q.size() == 1) {
    _q.next();
    _dt = _q.value(0).toDateTime();
  }
  return _dt.toString(_format);
}

const QDateTime ASqlCore::getDateTimeStamp() {
  QDateTime _dt = QDateTime::currentDateTime();
  QSqlQuery _q = ASqlCore::query("SELECT CURRENT_TIMESTAMP;");
  if (_q.size() == 1) {
    _q.next();
    _dt = _q.value(0).toDateTime();
  }
  return _dt;
}

bool ASqlCore::open() {
  if (networkStatus()) {
    if (database->isOpen())
      return true;

    return initDatabase();
  }

  emit sendStatementError(sqlNetworkError());
  return false;
}

const QSqlDatabase ASqlCore::db() {
  return QSqlDatabase::database(config->connectionName(), isConnected());
}

const QSqlRecord ASqlCore::record(const QString& table) {
  if (!isConnected())
    return QSqlRecord();

  QSqlRecord re;
  re = database->record(table);
  return re;
}

const QStringList ASqlCore::fieldNames(const QString& table) {
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

const QSqlQuery ASqlCore::query(const QString& statement) {
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
  if (database != nullptr && database->isOpen()) {
    qInfo("Close Database to '%s'.", qPrintable(database->hostName()));
    database->close();
  }
}

}; // namespace AntiquaCRM
