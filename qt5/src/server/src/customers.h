// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACMD_CUSTOMERS_H
#define ANTIQUACMD_CUSTOMERS_H

#include <QJsonObject>
#include <QJsonValue>
#include <QString>

#include "sqlpsql.h"

class Customers final : public QJsonObject {

private:
  SqlPsql *m_sql;

  qint64 sqlInsert();

  qint64 sqlQuery();

public:
  explicit Customers(SqlPsql *db, const QJsonObject &obj);
  const QString buyerName();
  qint64 getId();
};

#endif // CUSTOMERS_H
