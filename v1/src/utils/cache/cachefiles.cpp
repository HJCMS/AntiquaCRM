// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cachefiles.h"
#include "bookbindingcache.h"
#include "postalcodecache.h"
#include "storagelocationcache.h"
#include "publishercache.h"

#include <AntiquaCRM>

AntiquaCacheFiles::AntiquaCacheFiles(QObject *parent) : QObject{parent} {
  setObjectName("cache_preloader");
}

const QList<Workload *> AntiquaCacheFiles::getTasks() {
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QList<Workload *> list;
  list.append(new BookBindingCache(m_sql));
  list.append(new StorageLocationCache(m_sql));
  list.append(new PostalcodeCache(m_sql));
  list.append(new PublisherCache(m_sql));
  return list;
}

bool AntiquaCacheFiles::createCaches() {
  foreach (Workload *wl, getTasks()) {
    if (wl != nullptr) {
      emit statusMessage(wl->info());
      if (wl->run())
        emit statusMessage("done");
    }
  }
  return true;
}
