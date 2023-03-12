// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUA_CACHEBUILDER_H
#define ANTIQUA_CACHEBUILDER_H

#include <AntiquaCRM>
#include <QList>
#include <QObject>
#include <QString>

#include "cacheconfig.h"

/**
 * @ingroup _cache
 * @brief The CacheBuilder class
 */
class CacheBuilder : public QObject {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;
  const QList<CacheConfig> configs();

Q_SIGNALS:
  void statusMessage(const QString &);

public:
  explicit CacheBuilder(QObject *parent = nullptr);
  bool createCacheTarget();
  bool createCaches();
};

#endif // ANTIQUA_CACHEBUILDER_H
