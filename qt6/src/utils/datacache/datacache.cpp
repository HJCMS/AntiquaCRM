// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "datacache.h"
#include "datacacheconfig.h"
#include "datacachequery.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QStringList>

DataCache::DataCache(AntiquaCRM::ASettings *cfg, AntiquaCRM::ASqlCore *pgsql,
                     QObject *parent)
    : QObject{parent}, m_cfg{cfg}, m_sql{pgsql} {
  setObjectName("cache_preloader");
}

const QList<DataCacheConfig> DataCache::configs() {
  QList<DataCacheConfig> _l;
  QString _sql =
      AntiquaCRM::ASqlFiles::queryStatement("query_antiquacrm_cacheconf");
  if (_sql.isEmpty())
    return _l;

  QSqlQuery _q = m_sql->query(_sql);
  if (_q.size() > 0) {
    while (_q.next()) {
      DataCacheConfig _dcc(_q.value("cache_table").toString(),
                           _q.value("cache_basename").toString(),
                           _q.value("cache_display").toString(),
                           _q.value("cache_runtime").toInt());
      _l.append(_dcc);
    }
    _q.clear();
  }
  return _l;
}

bool DataCache::createCaches() {
  DataCacheQuery *m_cq = new DataCacheQuery(m_sql);
  QListIterator<DataCacheConfig> it(configs());
  while (it.hasNext()) {
    DataCacheConfig c = it.next();
    if (m_cq != nullptr) {
      emit statusMessage(tr("Create") + " " + c.message);
      if (m_cq->createCache(c))
        emit statusMessage("done");
    }
  }
  emit statusMessage(tr("Create Postalcode cache"));
  return (m_cq->postalCodes());
}
