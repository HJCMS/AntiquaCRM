// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_PUBLISHERCACHE_H
#define ANTIQUA_PUBLISHERCACHE_H

#include "workload.h"

class PublisherCache final : public Workload {
  Q_OBJECT

protected:
  const QList<QPair<QString, QString>> tableList() override;
  const QJsonArray createTable(const QString &query) override;

public Q_SLOTS:
  bool run() override;

public:
  explicit PublisherCache(AntiquaCRM::ASqlCore *pgsql);
  const QString info() const override;
};

#endif // ANTIQUA_PUBLISHERCACHE_H
