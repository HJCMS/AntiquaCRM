// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACMD_CUSTOMERS_H
#define ANTIQUACMD_CUSTOMERS_H

#include <AntiquaCRM>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>

class Customers final : public QJsonObject {

private:
  AntiquaCRM::ASqlCore *m_sql;

  qint64 sqlInsert();

  qint64 sqlQuery();

public:
  explicit Customers(AntiquaCRM::ASqlCore *db, const QJsonObject &obj);
  const QString buyerName();
  qint64 getId();
};

#endif // CUSTOMERS_H
