// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storagelocationcache.h"

#include <AntiquaCRM>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

StorageLocationCache::StorageLocationCache(AntiquaCRM::ASqlCore *pgsql)
    : Workload{pgsql} {
  setObjectName("StorageLocations");
}

const QList<QPair<QString, QString>> StorageLocationCache::tableList() {
  QList<QPair<QString, QString>> list;
  return list;
}

const QJsonArray StorageLocationCache::createTable(const QString &query) {
  QJsonArray array;
  QSqlQuery q = m_sql->query(query);
  if (q.size() > 0) {
    while (q.next()) {
      QByteArray row = q.value(0).toByteArray();
      QJsonObject jr = QJsonDocument::fromJson(row).object();
      if (!jr.isEmpty())
        array.append(jr);
    }
  }
  return array;
}

void StorageLocationCache::run() {
  emit statusNotify(tr("Build Storage locations") + " ...");
  QJsonObject main;
  QString file("query_storage_location");
  QString sql = AntiquaCRM::ASqlFiles::queryStatement(file);
  QJsonArray array = createTable(sql);
  main.insert("storagelocations", array);
  AntiquaCRM::ASharedDataFiles p_store;
  if (p_store.storeJson("storagelocations", QJsonDocument(main))) {
    emit statusNotify(tr("Storage locations created."));
  } else {
    emit statusNotify(tr("Storage locations failed!"));
  }
}
