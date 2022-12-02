// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sqlpsql.h"
#include "settings.h"

#include <QMutex>
#include <QSqlError>

SqlPsql::SqlPsql(QObject *parent) : QObject{parent} {
  database = nullptr;
  QSqlDatabase db = QSqlDatabase::database("AntiquaCMD");
  if (db.isValid()) {
    database = new QSqlDatabase(db);
  } else {
    initDatabase();
  }
}

bool SqlPsql::initDatabase() {
  if (database != nullptr)
    return database->isValid();

  QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "AntiquaCMD");
  Settings cfg(this);
  cfg.beginGroup("database");
  db.setHostName(cfg.value("pg_hostname").toString());
  db.setPort(cfg.value("pg_port").toInt());
  db.setDatabaseName(cfg.value("pg_database").toString());
  db.setUserName(cfg.value("pg_username").toString());
  QByteArray array = cfg.value("pg_password").toByteArray();
  QByteArray buf = QByteArray::fromBase64(array, QByteArray::Base64UrlEncoding);
  db.setPassword(QString::fromLocal8Bit(buf));
  cfg.endGroup();

  QStringList options;
  options << QString("connect_timeout=%1").arg(3);
  options << QString("application_name=AntiquaCMD");
  db.setConnectOptions(options.join(";"));

  if (!db.isValid())
    return false;

  if (database == nullptr)
    database = new QSqlDatabase(db);

  if (database->isOpenError()) {
    qWarning("Missing SQL::Connection");
    return false;
  }
  return database->isOpen();
}

bool SqlPsql::isConnected() {
  if (database->isOpen())
    return true;

  if (database->open())
    return true;

  qWarning("Can't open Database Connection!");
  QSqlError err = database->lastError();
  return false;
}

bool SqlPsql::open() {
  if (initDatabase()) {
    qInfo("Database connected to Host '%s'.", qPrintable(database->hostName()));
    return true;
  }
  return false;
}

const QSqlQuery SqlPsql::query(const QString &statement) {
  if (!isConnected())
    return QSqlQuery();

  QSqlQuery qr;
  QMutex mutex;
  mutex.lock();
  qr = database->exec(statement);
  mutex.unlock();
  return qr;
}

const QString SqlPsql::lastError() {
  QSqlError err = database->lastError();
  if (err.isValid())
    return err.text();

  return QString();
}

void SqlPsql::close() {
  if (database != nullptr) {
    if (database->isOpen()) {
      qInfo("Database '%s' closed.", qPrintable(database->connectionName()));
      database->close();
    }
  }
}
