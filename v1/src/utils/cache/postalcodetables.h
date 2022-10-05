// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_POSTALCODETABLES_H
#define ANTIQUA_POSTALCODETABLES_H

#include "workload.h"

class PostalcodeTables final : public Workload {
protected:
  const QList<QPair<QString, QString>> tableList() override;
  const QJsonArray createTable(const QString &query) override;

public Q_SLOTS:
  void run() override;

public:
  explicit PostalcodeTables(AntiquaCRM::ASqlCore *pgsql);
};

#endif // ANTIQUA_POSTALCODETABLES_H
