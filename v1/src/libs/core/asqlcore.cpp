// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlcore.h"
#include "asqlprofile.h"
#include "asqlsettings.h"

#include <QDebug>
#include <QMutex>

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
  options << QString("application_name=AntiquaCRM");
  if (profile.getEnableSSL()) {
    options << QString("sslmode=%1").arg(profile.getSslMode());
    options << QString("sslrootcert=%1").arg(profile.getSslRootCert());
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
  return database->isValid();
}

bool ASqlCore::open() { return initDatabase(); }

const QSqlRecord ASqlCore::record(const QString &table) {
  if (!database->isOpen())
    database->open();

  QSqlRecord re;
  QMutex mutex;
  mutex.lock();
  re = database->record(table);
  mutex.unlock();
  return re;
}

const QSqlQuery ASqlCore::query(const QString &statement) {
  if (!database->isOpen())
    database->open();

  QSqlQuery qr;
  QMutex mutex;
  mutex.lock();
  qr = database->exec(statement);
  mutex.unlock();
  return qr;
}

void ASqlCore::close() {
  if (database != nullptr) {
    if (database->isOpen()) {
      qInfo("Database '%s' closed.", qPrintable(database->connectionName()));
      database->close();
    }
  }
}

}; // namespace AntiquaCRM
