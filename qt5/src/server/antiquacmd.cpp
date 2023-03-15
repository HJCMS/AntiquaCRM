// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquacmd.h"
#include "abebooks.h"
#include "booklooker.h"
#include "buchfreund.h"

#include <QDebug>
#include <QEventLoop>
#include <QMutex>
#include <QTimer>

AntiquaCMD::AntiquaCMD(int &argc, char **argv) : QCoreApplication{argc, argv} {
  setApplicationName("antiquacmd");
  setApplicationVersion(ANTIQUACRM_VERSION);
  setOrganizationDomain(ANTIQUACRM_CONNECTION_DOMAIN);
  m_cfg = new AntiquaCRM::ASettings(applicationName(), this);
}

int AntiquaCMD::update(Provider *obj) {
  QMutex m(QMutex::NonRecursive);
  m.lock();

  QTimer timer;
  QEventLoop loop;
  connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
  connect(obj, SIGNAL(sendFinished()), &loop, SLOT(quit()));
  connect(obj, SIGNAL(sendDisjointed()), &loop, SLOT(quit()));
  if (!obj->init())
    return 1;

  obj->start();

  timer.start((timeout * 1000));
  if (loop.exec() == 0) {
    m.unlock();
    return 0;
  }

  m.unlock();
  return 1;
}

void AntiquaCMD::queryAll() {
  update(new AbeBooks(this));
  update(new BookLooker(this));
  update(new Buchfreund(this));
}

int AntiquaCMD::exec() {
  queryAll();
  return 0;
}
