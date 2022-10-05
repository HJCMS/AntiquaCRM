// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookbinding.h"

#include <AntiquaCRM>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

BookBinding::BookBinding(AntiquaCRM::ASqlCore *pgsql) : Workload{pgsql} {
  setObjectName("BookBinding");
}

const QList<QPair<QString, QString>> BookBinding::tableList() {
  QList<QPair<QString, QString>> list;
  return list;
}

const QJsonArray BookBinding::createTable(const QString &query) {
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

void BookBinding::run() {
  emit statusNotify(tr("Build Book binding") + " ...");
  QJsonObject main;
  QString file("query_book_binding");
  QString sql = AntiquaCRM::ASqlFiles::queryStatement(file);
  QJsonArray array = createTable(sql);
  main.insert("bookbindings", array);
  AntiquaCRM::ASharedDataFiles p_store;
  if (p_store.storeJson("bookbindings", QJsonDocument(main))) {
    emit statusNotify(tr("Book binding created."));
  } else {
    emit statusNotify(tr("Book binding failed!"));
  }
}
