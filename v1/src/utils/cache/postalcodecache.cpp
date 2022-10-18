// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "postalcodecache.h"
#include <AntiquaCRM>

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

PostalcodeCache::PostalcodeCache(AntiquaCRM::ASqlCore *pgsql)
    : Workload{pgsql} {
  setObjectName("Postcalcodes");
}

const QList<QPair<QString, QString>> PostalcodeCache::tableList() {
  QList<QPair<QString, QString>> list;
  QString sql = AntiquaCRM::ASqlFiles::queryStatement("query_postal_codes");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      QPair<QString, QString> pair;
      pair.first = q.value("p_country").toString();
      pair.second = q.value("p_table").toString();
      list.append(pair);
    }
  }
  return list;
}

const QJsonArray PostalcodeCache::createTable(const QString &query) {
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

bool PostalcodeCache::run() {
  QJsonObject main;
  QString file("select_statement_postalcode_tables");
  QString select = AntiquaCRM::ASqlFiles::selectStatement(file);
  QList<QPair<QString, QString>> list = tableList();
  for (int i = 0; i < list.count(); i++) {
    QPair<QString, QString> plz = list.at(i);
    QString sql(select + " FROM " + plz.second + " ORDER BY p_plz ASC;");
    main.insert(plz.second, createTable(sql));
  }
  AntiquaCRM::ASharedDataFiles p_store;
  if (p_store.storeJson("postalcodes", QJsonDocument(main)))
    return true;

  return false;
}

const QString PostalcodeCache::info() const {
  return tr("Build Postalcodes") + " ...";
}
