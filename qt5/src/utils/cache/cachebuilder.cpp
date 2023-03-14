// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cachebuilder.h"
#include "cachequery.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QStringList>

CacheBuilder::CacheBuilder(QObject *parent) : QObject{parent} {
  setObjectName("cache_preloader");
  m_sql = new AntiquaCRM::ASqlCore(this);
}

const QList<CacheConfig> CacheBuilder::configs() {
  QList<CacheConfig> list;
  CacheConfig storage("query_storage_location", "storagelocations",
                      tr("Storage locations"));
  list.append(storage);

  CacheConfig binding("query_book_binding", "bookbindings",
                      tr("Book bindings"));
  list.append(binding);

  CacheConfig designation("query_designation", "designations",
                          tr("Book designation"));
  list.append(designation);

  CacheConfig keywords("query_keywords", "keywords", tr("Keywordslist"));
  list.append(keywords);

  CacheConfig publisher("query_book_publisher_de", "publishers",
                        tr("Publisher"));
  list.append(publisher);
  return list;
}

bool CacheBuilder::createCacheTarget() {
  QFileInfo cti(AntiquaCRM::ASettings::getUserTempDir().path());
  return cti.isWritable();
}

bool CacheBuilder::createCaches() {
  if (!createCacheTarget()) {
    qWarning("CacheBuilder: Premission denied!");
    return false;
  }

  CacheQuery *m_cq = new CacheQuery(m_sql);
  QListIterator<CacheConfig> it(configs());
  while (it.hasNext()) {
    CacheConfig c = it.next();
    if (m_cq != nullptr) {
      emit statusMessage(tr("Create") + " " + c.message);
      if (m_cq->createCache(c))
        emit statusMessage("done");
    }
  }
  emit statusMessage(tr("Create Postalcode cache"));
  return (m_cq->postalCodes());
}
