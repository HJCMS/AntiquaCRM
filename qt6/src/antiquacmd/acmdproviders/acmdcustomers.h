// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACMD_CUSTOMERS_H
#define ANTIQUACMD_CUSTOMERS_H

#include <AntiquaCRM>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>

class ACmdCustomers final : public QJsonObject {
private:
  AntiquaCRM::ASqlCore *m_sql;
  qint64 sqlInsert();
  qint64 sqlQuery();

public:
  explicit ACmdCustomers(AntiquaCRM::ASqlCore *db, const QJsonObject &obj);
  const QString buyerName();
  qint64 getId();
};

#endif // ANTIQUACMD_CUSTOMERS_H
