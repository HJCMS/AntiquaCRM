// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sqlcore.h"
#include "sqlconfig.h"

#include <QtCore/QDebug>
#include <QtCore/QSettings>
#include <QtSql/QSqlError>

namespace HJCMS {

SqlCore::SqlCore(QObject *parent) : QObject{parent} {
  setObjectName("PostgreSqlCoreDriver");
  m_cfg = new SqlConfig();
  p_db = QSqlDatabase::addDatabase("QPSQL", m_cfg->getConnectioName());
  if (p_db.isValid()) {
    initDatabase();
  } else {
    qFatal("Can not load SQL::Driver Module!");
  }
}

const QString SqlCore::prepareErrorMessage() {
  QString simple("Unknown error.");

  if (database == nullptr)
    return simple;

  QSqlError err = database->lastError();
  qDebug() << "ERROR:" << err.databaseText();

  switch (err.type()) {
  case QSqlError::ConnectionError:
    return err.driverText();

  case QSqlError::StatementError:
    return err.databaseText();

  case QSqlError::TransactionError:
    return err.databaseText();

  default:
    return simple;
  };
  return simple;
}

void SqlCore::initDatabase() {
  database = new QSqlDatabase(p_db);
  database->setHostName(m_cfg->getAddress());
  database->setPort(m_cfg->getPort());
  database->setDatabaseName(m_cfg->getDatabase());
  database->setUserName(m_cfg->getUserName());
  database->setPassword(m_cfg->getPassword());
  database->setConnectOptions();
  if (!database->open()) {
    QString err(prepareErrorMessage());
    emit s_connectionStatus(false);
    emit s_sendMessage(err);
    qWarning("'%s' PostgerSQL DB connected", qPrintable(err));
    return;
  }
  emit s_connectionStatus(true);
  emit s_sendMessage(tr("PostgerSQL DB connected"));
}

void SqlCore::statusTrigger() {}

SqlCore::~SqlCore() {
//   if (database->isOpen()) {
//     qDebug("SQL '%s' Disonnected", qPrintable(database->connectionName()));
//     database->close();
//   }
}

}; // namespace HJCMS
