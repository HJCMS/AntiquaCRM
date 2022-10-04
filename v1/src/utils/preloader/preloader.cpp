// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "preloader.h"
#include "postalcodetables.h"

AntiquaPreloader::AntiquaPreloader(QObject *parent) : QThreadPool{parent} {
  setObjectName("cache_preloader");
  waitForDone(5000);
  m_sql = new AntiquaCRM::ASqlCore(this);
}

const QList<QRunnable *> AntiquaPreloader::getThreads() {
  QList<QRunnable *> list;
  list.append(new PostalcodeTables(m_sql));
  return list;
}
