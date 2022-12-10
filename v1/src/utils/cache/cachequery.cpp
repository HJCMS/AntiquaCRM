// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cachequery.h"

CacheQuery::CacheQuery(AntiquaCRM::ASqlCore *pgsql) : QObject{}, m_sql{pgsql} {}

bool CacheQuery::isCacheUpdateRequired(const QString &name) {
  AntiquaCRM::ASharedDataFiles p_store;
  return p_store.needsUpdate(name);
}

bool CacheQuery::saveDocument(const QString &key,
                              const QJsonDocument &json) const {
  AntiquaCRM::ASharedDataFiles p_store;
  return p_store.storeJson(key, json);
}

const QJsonArray CacheQuery::createTable(const QString &query) {
  QJsonArray array;
  QSqlQuery q = m_sql->query(query);
  if (q.size() > 0) {
    while (q.next()) {
      QByteArray row = q.value(0).toByteArray();
      QJsonObject jr = QJsonDocument::fromJson(row).object();
      if (!jr.isEmpty())
        array.append(jr);
    }
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
  }
  return array;
}

bool CacheQuery::createCache(const CacheConfig &config) {
  QString sql = AntiquaCRM::ASqlFiles::queryStatement(config.file);
  if (sql.isEmpty())
    return false;

  QJsonObject main;
  QJsonArray array = createTable(sql);
  main.insert(config.indicator, array);
  return saveDocument(config.indicator, QJsonDocument(main));
}

bool CacheQuery::postalCodes() {
  if (!isCacheUpdateRequired("postalcodes"))
    return false;

  QString sql = AntiquaCRM::ASqlFiles::queryStatement("query_postal_codes");
  if (sql.isEmpty())
    return false;

  QString statement("select_statement_postalcode_tables");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    QJsonObject main;
    QJsonObject countries;
    QStringList queries;
    while (q.next()) {
      QString table = q.value("p_table").toString();
      countries.insert(table, q.value("p_country").toString());
      QString select = AntiquaCRM::ASqlFiles::selectStatement(statement);
      select.append(" FROM " + table + " ORDER BY p_plz ASC;");
      QJsonArray section = createTable(select);
      main.insert(table, section);
    }
    main.insert("tables", countries);

    if (!saveDocument("postalcodes", QJsonDocument(main))) {
      qWarning("Errors in create Postalcode!");
      return false;
    }
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
  }
  return true;
}
