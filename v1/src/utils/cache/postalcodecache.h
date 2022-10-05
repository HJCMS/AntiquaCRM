// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_POSTALCODECACHE_H
#define ANTIQUA_POSTALCODECACHE_H

#include "workload.h"

class PostalcodeCache final : public Workload {
protected:
  const QList<QPair<QString, QString>> tableList() override;
  const QJsonArray createTable(const QString &query) override;

public Q_SLOTS:
  void run() override;

public:
  explicit PostalcodeCache(AntiquaCRM::ASqlCore *pgsql);
};

#endif // ANTIQUA_POSTALCODECACHE_H
