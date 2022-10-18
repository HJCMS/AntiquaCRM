// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUA_WORKLOAD_H
#define ANTIQUA_WORKLOAD_H

#include <AntiquaCRM>
#include <QJsonArray>
#include <QList>
#include <QObject>
#include <QPair>

class Workload : public QObject {
  Q_OBJECT

protected:
  AntiquaCRM::ASqlCore *m_sql;
  virtual const QList<QPair<QString, QString>> tableList() = 0;
  virtual const QJsonArray createTable(const QString &query) = 0;

public Q_SLOTS:
  virtual bool run() = 0;

public:
  explicit Workload(AntiquaCRM::ASqlCore *pgsql);
  virtual const QString info() const = 0;
};

#endif // ANTIQUA_WORKLOAD_H
