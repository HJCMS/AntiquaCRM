// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_CACHEBUILDER_H
#define ANTIQUA_CACHEBUILDER_H

#include <AntiquaCRM>
#include <QList>
#include <QObject>
#include <QString>

#include "cacheconfig.h"

/**
 * @brief The CacheBuilder class
 * @section AntiquaCache
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
