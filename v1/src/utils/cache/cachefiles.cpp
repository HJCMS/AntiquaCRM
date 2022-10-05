// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cachefiles.h"
#include "postalcodetables.h"
#include "bookbinding.h"

#include <AntiquaCRM>

AntiquaCacheFiles::AntiquaCacheFiles(QObject *parent) : QObject{parent} {
  setObjectName("cache_preloader");
}

const QList<Workload *> AntiquaCacheFiles::getTasks() {
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QList<Workload *> list;
  list.append(new PostalcodeTables(m_sql));
  list.append(new BookBinding(m_sql));
  return list;
}

void AntiquaCacheFiles::createCaches() {
  foreach (Workload *wl, getTasks()) {
    if (wl != nullptr) {
      connect(wl, SIGNAL(statusNotify(const QString &)), this,
              SIGNAL(statusMessage(const QString &)));

      wl->moveToThread(&p_thread);
      connect(&p_thread, SIGNAL(finished()), wl, SLOT(deleteLater()));
      connect(&p_thread, SIGNAL(started()), wl, SLOT(run()));
      p_thread.start();
    }
  }
}
