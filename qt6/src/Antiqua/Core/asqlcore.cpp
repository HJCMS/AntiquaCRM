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
    qWarning("Network interface is down!");
    return false;
  }

  if ((database != nullptr) && (database->isValid()))
    return true;

  // Initial DataBaseConnection
  // https://www.postgresql.org/docs/current/libpq-connect.html
  QSqlDatabase _dbc = QSqlDatabase::addDatabase("QPSQL", config->connectionName());
  ASqlProfile _cfg = config->connectionProfile();
  _dbc.setHostName(_cfg.getHostname());
  _dbc.setPort(_cfg.getPort());
  _dbc.setDatabaseName(_cfg.getDatabaseName());
  _dbc.setUserName(_cfg.getUsername());
  _dbc.setPassword(_cfg.getPassword());

  QStringList _opts;
  _opts << QString("connect_timeout=%1").arg(_cfg.getTimeout());
  _opts << QString("application_name=%1").arg(identifier());
  _opts << QString("target_session_attrs=read-write");

  if (_cfg.getEnableSSL()) {
    _opts << QString("sslsni=1");
    _opts << QString("sslmode=%1").arg(_cfg.getSslMode());
    QString _crt = _cfg.getSslRootCert();
    if (_crt.isEmpty())
      _opts << QString("sslrootcert=%1").arg("system");
    else
      _opts << QString("sslrootcert=%1").arg(_crt);
  }
  _dbc.setConnectOptions(_opts.join(";"));

  if (!_dbc.isValid())
    return false;

  if (database == nullptr)
    database = new QSqlDatabase(_dbc);

  if (database->isOpenError()) {
    qWarning("Database open errors ...");
    prepareSqlError(database->lastError());
    return false;
  }

  qInfo("Database connected to '%s'.", qPrintable(database->hostName()));
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

  const QSqlError _err = database->lastError();
  if (_err.isValid())
    prepareSqlError(_err);

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
#ifdef ANTIQUA_DEVELOPMENT
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
  _select.append(" application_name='" + identifier() + "' AND state IS NOT NULL;");
  return (ASqlCore::query(_select).size() > 0);
}

bool ASqlCore::networkStatus() {
  AntiquaCRM::ANetworkIface iface;
  if (iface.connectedIfaceExists())
    return true;

  return false;
}

const QString ASqlCore::getYearByDays(qint8 days) {
  QString _str("EXTRACT(YEAR FROM ");
  if (days < 0) {
    _str.append(QString::asprintf("(CURRENT_DATE %d)", days));
  } else if (days > 0) {
    _str.append(QString::asprintf("(CURRENT_DATE +%d)", days));
  } else {
    _str.append("CURRENT_DATE");
  }
  _str.append(")");
  return _str;
}

const QString ASqlCore::identifier() {
  QString _name("AntiquaCRM_v");
  _name.append(ANTIQUACRM_VERSION);
  _name.append("_");
  _name.append(QSysInfo::machineHostName());
  return _name.trimmed();
}

qint64 ASqlCore::getQueryLimit() {
  int _limit = config->getParam("querylimit").toInt();
  if (_limit > 0)
    return _limit;

  // fallback
  return config->value("database/SqlQueryLimit", 999).toInt();
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

  return database->record(table);
}

const QStringList ASqlCore::fieldNames(const QString& table) {
  QStringList _l;

  if (!isConnected())
    return _l;

  const QSqlRecord _r = ASqlCore::record(table);
  if (_r.isEmpty())
    return _l;

  for (int i = 0; i < _r.count(); i++) {
    if (_r.field(i).isValid())
      _l.append(_r.field(i).name());
  }

  return _l;
}

const QSqlQuery ASqlCore::query(const QString& statement) {
  if (!isConnected() || statement.isEmpty())
    return QSqlQuery();

  QSqlQuery _query(db());
  _query.exec(statement);

  if (_query.lastError().isValid())
    prepareSqlError(_query.lastError());

  return _query;
}

const QString ASqlCore::lastError() {
  const QSqlError _error = database->lastError();
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

} // namespace AntiquaCRM
