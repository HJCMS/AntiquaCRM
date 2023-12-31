// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquacmd.h"
#include "acmdproviders.h"

#include <QDebug>
#include <QEventLoop>
#include <QMutexLocker>
#include <QTimer>

QMutex AntiquaCMD::s_mutex;

AntiquaCMD::AntiquaCMD(int &argc, char **argv) : QCoreApplication{argc, argv} {
  setApplicationName("antiquacmd");
  setApplicationVersion(ANTIQUACRM_VERSION);
  setOrganizationDomain(ANTIQUACRM_CONNECTION_DOMAIN);
  m_cfg = new AntiquaCRM::ASettings(this);
}

int AntiquaCMD::update(ACmdProviders *provider) {
  QMutexLocker locker(&s_mutex);
  QTimer timer;
  QEventLoop loop;
  connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
  connect(provider, SIGNAL(sendFinished()), &loop, SLOT(quit()));
  connect(provider, SIGNAL(sendDisjointed()), &loop, SLOT(quit()));
  if (!provider->init())
    return 1;

  provider->start();
  timer.start((timeout * 1000));
  if (loop.exec() == 0) {
    return 0;
  }

  return 1;
}

void AntiquaCMD::queryAll() {
  // update(new AbeBooks(this));
  // update(new BookLooker(this));
  // update(new Buchfreund(this));
}

int AntiquaCMD::exec() {
  queryAll();
  return 0;
}
