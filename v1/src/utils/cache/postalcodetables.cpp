// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "postalcodetables.h"
#include <AntiquaCRM>

#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

PostalcodeTables::PostalcodeTables(AntiquaCRM::ASqlCore *pgsql)
    : Workload{pgsql} {
  setObjectName("Postcalcodes");
}

const QList<QPair<QString, QString>> PostalcodeTables::tableList() {
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

const QJsonArray PostalcodeTables::createTable(const QString &query) {
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

void PostalcodeTables::run() {
  emit statusNotify(tr("Build Postalcodes") + " ...");
  QJsonObject main;
  QString file("select_statement_postalcode_tables");
  QString select = AntiquaCRM::ASqlFiles::selectStatement(file);
  QList<QPair<QString, QString>> list = tableList();
  for (int i = 0; i < list.count(); i++) {
    QPair<QString, QString> plz = list.at(i);
    QString sql(select + " FROM " + plz.second + " ORDER BY p_plz ASC;");
    main.insert(plz.first, createTable(sql));
  }
  AntiquaCRM::ASharedDataFiles p_store;
  if (p_store.storeJson("postalcodes", QJsonDocument(main))) {
    emit statusNotify(tr("Postalcode created."));
  } else {
    emit statusNotify(tr("Postalcode build failed!"));
  }
}
