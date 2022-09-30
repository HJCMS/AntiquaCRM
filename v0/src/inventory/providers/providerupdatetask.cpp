// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerupdatetask.h"

#include <QDebug>

ProviderUpdateTask::ProviderUpdateTask() : QRunnable{} {}

bool ProviderUpdateTask::setHandle(Antiqua::Interface *handle) {
  if (handle == nullptr) {
    return false;
  }
  m_worker = handle;
  return true;
}

void ProviderUpdateTask::run() {
  if (m_worker == nullptr) {
    return;
  }
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << m_worker->objectName();
#endif
  m_worker->queryMenueEntries();
}
