// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SQLPSQL_H
#define ANTIQUACRM_SQLPSQL_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlField>
#include <QSqlQuery>
#include <QSqlRecord>

class SqlPsql final : public QObject {
  Q_OBJECT

private:
  QSqlDatabase *database;
  bool initDatabase();
  bool isConnected();

public:
  explicit SqlPsql(QObject *parent = nullptr);
  bool open();
  const QSqlQuery query(const QString &statement);
  const QString lastError();
  void close();
};

#endif // SQLPSQL_H
