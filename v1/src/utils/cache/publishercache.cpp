// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "publishercache.h"

#include <AntiquaCRM>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

PublisherCache::PublisherCache(AntiquaCRM::ASqlCore *pgsql) : Workload{pgsql} {
  setObjectName("book_publisher_de");
}

const QList<QPair<QString, QString>> PublisherCache::tableList() {
  QList<QPair<QString, QString>> list;
  return list;
}

const QJsonArray PublisherCache::createTable(const QString &query) {
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

bool PublisherCache::run() {
  QJsonObject main;
  QString file("query_book_publisher_de");
  QString sql = AntiquaCRM::ASqlFiles::queryStatement(file);
  QJsonArray array = createTable(sql);
  main.insert("publisher", array);
  AntiquaCRM::ASharedDataFiles p_store;
  if (p_store.storeJson("publisher", QJsonDocument(main)))
    return true;

  return false;
}

const QString PublisherCache::info() const {
  return tr("Publishers") + " ...";
}
