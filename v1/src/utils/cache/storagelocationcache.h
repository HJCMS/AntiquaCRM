// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_STORAGELOCATIONCACHE_H
#define ANTIQUA_STORAGELOCATIONCACHE_H

#include "workload.h"

class StorageLocationCache final : public Workload {
  Q_OBJECT

protected:
  const QList<QPair<QString, QString>> tableList() override;
  const QJsonArray createTable(const QString &query) override;

public Q_SLOTS:
  void run() override;

public:
  explicit StorageLocationCache(AntiquaCRM::ASqlCore *pgsql);
};

#endif // ANTIQUA_STORAGELOCATIONCACHE_H
