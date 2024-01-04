// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "datacachequery.h"
#include "datacacheconfig.h"

DataCacheQuery::DataCacheQuery(AntiquaCRM::ASqlCore *pgsql)
    : QObject{}, m_sql{pgsql} {}

bool DataCacheQuery::isCacheUpdateRequired(const QString &name) {
  AntiquaCRM::ASharedDataFiles p_store;
  return p_store.needsUpdate(name);
}

bool DataCacheQuery::saveDocument(const QString &key,
                                  const QJsonDocument &json) const {
  AntiquaCRM::ASharedDataFiles p_store;
  return p_store.storeJson(key, json);
}

const QJsonArray DataCacheQuery::createTable(const QString &query) {
  QJsonArray _array;
  QSqlQuery _q = m_sql->query(query);
  if (_q.size() > 0) {
    while (_q.next()) {
      const QByteArray _row = _q.value(0).toByteArray();
      const QJsonObject _object = QJsonDocument::fromJson(_row).object();
      if (!_object.isEmpty())
        _array.append(_object);
    }
    _q.clear();
  }
  return _array;
}

bool DataCacheQuery::createCache(const DataCacheConfig &config) {
  if (!isCacheUpdateRequired(config.indicator))
    return false;

  QString _sql = AntiquaCRM::ASqlFiles::queryStatement(config.file);
  if (_sql.isEmpty())
    return false;

  QJsonObject _object;
  _object.insert(config.indicator, createTable(_sql));
  return saveDocument(config.indicator, QJsonDocument(_object));
}

bool DataCacheQuery::postalCodes() {
  if (!isCacheUpdateRequired("postalcodes"))
    return false;

  QString sql = AntiquaCRM::ASqlFiles::queryStatement("query_postal_codes");
  if (sql.isEmpty())
    return false;

  const QString _sql_select = AntiquaCRM::ASqlFiles::queryStatement(
      "select_statement_postalcode_tables");
  const QString _sql_order(" ORDER BY p_plz ASC;");

  QSqlQuery _q = m_sql->query(sql);
  if (_q.size() > 0) {
    QJsonObject _object;
    QJsonObject _countries;
    while (_q.next()) {
      QString _table = _q.value("p_table").toString();
      _countries.insert(_table, _q.value("p_country").toString());
      const QString _query(_sql_select + " FROM " + _table + _sql_order);
      _object.insert(_table, createTable(_query));
    }
    _object.insert("tables", _countries);
    _q.clear();

    if (!saveDocument("postalcodes", QJsonDocument(_object))) {
      qWarning("Syntax errors in Json Postalcode!");
      return false;
    }
  }
  return true;
}
