// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "datacachequery.h"
#include "datacacheconfig.h"

DataCacheQuery::DataCacheQuery(AntiquaCRM::ASqlCore* pgsql)
    : QObject{pgsql}, p_curDateTime{QDateTime::currentDateTime()}, m_sql{pgsql} {
}

bool DataCacheQuery::isCacheUpdateRequired(const DataCacheConfig& config, int days) {
  AntiquaCRM::ASharedDataFiles p_store;
  return p_store.needsUpdate(config.indicator, config.timeStamp, days);
}

bool DataCacheQuery::saveDocument(const QString& key, const QJsonDocument& json) const {
  AntiquaCRM::ASharedDataFiles p_store;
  return p_store.storeJson(key, json);
}

const QJsonArray DataCacheQuery::createTable(const QString& query) {
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

bool DataCacheQuery::createCache(const DataCacheConfig& config) {
  if (!isCacheUpdateRequired(config))
    return false;

  QString _sql = AntiquaCRM::ASqlFiles::queryStatement(config.file);
  if (_sql.isEmpty())
    return false;

  QJsonObject _object;
  _object.insert(config.indicator, createTable(_sql));
  return saveDocument(config.indicator, QJsonDocument(_object));
}

bool DataCacheQuery::postalCodes() {
  const QString _basename("postalcodes");
  QString _sql("SELECT cache_timestamp FROM antiquacrm_cacheconf WHERE cache_basename='");
  _sql.append(_basename + "';");

  QDateTime _dt = p_curDateTime.addDays(-30);
  QSqlQuery _fq = m_sql->query(_sql);
  if (_fq.size() > 0) {
    _fq.next();
    _dt = _fq.value(0).toDateTime();
  }

  DataCacheConfig cfg("query_postal_codes", _basename, tr("Postalcode"), _dt);
  if (!isCacheUpdateRequired(cfg))
    return false;

  _sql = AntiquaCRM::ASqlFiles::queryStatement("query_postal_codes");
  if (_sql.isEmpty())
    return false;

  const QString _sql_select =
      AntiquaCRM::ASqlFiles::queryStatement("select_statement_postalcode_tables");
  const QString _sql_order(" ORDER BY p_plz ASC;");

  QSqlQuery _q = m_sql->query(_sql);
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

    if (!saveDocument(_basename, QJsonDocument(_object))) {
      qWarning("Syntax errors in Json Postalcode!");
      return false;
    }
  }
  return true;
}
