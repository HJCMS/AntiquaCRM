// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "datacache.h"
#include "datacachequery.h"
#include "datacacheconfig.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QStringList>

DataCache::DataCache(QObject *parent) : QObject{parent} {
  setObjectName("cache_preloader");
  m_sql = new AntiquaCRM::ASqlCore(this);
}

const QList<DataCacheConfig> DataCache::configs() {
  QList<DataCacheConfig> list;
  DataCacheConfig storage("query_storage_location", "storagelocations",
                      tr("Storage locations"));
  list.append(storage);

  DataCacheConfig binding("query_book_binding", "bookbindings",
                      tr("Book bindings"));
  list.append(binding);

  DataCacheConfig designation("query_designation", "designations",
                          tr("Book designation"));
  list.append(designation);

  DataCacheConfig keywords("query_keywords", "keywords", tr("Keywordslist"));
  list.append(keywords);

  DataCacheConfig publisher("query_book_publisher_de", "publishers",
                        tr("Publisher"));
  list.append(publisher);
  return list;
}

bool DataCache::createCacheTarget() {
  QFileInfo cti(AntiquaCRM::ASettings::getUserTempDir().path());
  return cti.isWritable();
}

bool DataCache::createCaches() {
  if (!createCacheTarget()) {
    qWarning("DataCache: Premission denied!");
    return false;
  }

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


