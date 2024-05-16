// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_DATACACHEQUERY_H
#define ANTIQUACRM_UTILS_DATACACHEQUERY_H

#include <AntiquaCRM>
#include <QList>
#include <QObject>
#include <QPair>
#include <QString>

class DataCacheConfig;

/**
 * @class DataCacheQuery
 */
class DataCacheQuery final : public QObject {
  Q_OBJECT

private:
  int p_days = 7;
  AntiquaCRM::ASqlCore *m_sql;
  bool isCacheUpdateRequired(const QString &name);
  bool saveDocument(const QString &key, const QJsonDocument &json) const;
  const QList<QPair<QString, QString>> tableList();
  const QJsonArray createTable(const QString &query);

public:
  explicit DataCacheQuery(AntiquaCRM::ASqlCore *pgsql);
  bool createCache(const DataCacheConfig &config);
  bool postalCodes();
};

#endif // ANTIQUACRM_UTILS_DATACACHEQUERY_H
