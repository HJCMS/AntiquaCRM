// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUA_QUERYCACHE_H
#define ANTIQUA_QUERYCACHE_H

#include <AntiquaCRM>
#include <QJsonDocument>
#include <QJsonArray>
#include <QList>
#include <QObject>
#include <QPair>

#include "cacheconfig.h"

/**
 * @brief The CacheQuery class
 * @ingroup _cache
 */
class CacheQuery final : public QObject {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;
  bool isCacheUpdateRequired(const QString &name);
  bool saveDocument(const QString &key, const QJsonDocument &json) const;
  const QList<QPair<QString, QString>> tableList();
  const QJsonArray createTable(const QString &query);

public:
  explicit CacheQuery(AntiquaCRM::ASqlCore *pgsql);
  bool createCache(const CacheConfig &config);
  bool postalCodes();
};

#endif // ANTIQUA_QUERYCACHE_H
