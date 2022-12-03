// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquacmd.h"
#include "abebooks.h"
#include "booklooker.h"
#include "buchfreund.h"
#include "settings.h"

// #include "sysexits.h"
#include <QDebug>
#include <QEventLoop>
#include <QTimer>

AntiquaCMD::AntiquaCMD(int &argc, char **argv) : QCoreApplication{argc, argv} {
  setApplicationName("antiquacmd");
  setApplicationVersion(ANTIQUACRM_VERSION);
  setOrganizationDomain(ANTIQUACRM_CONNECTION_DOMAIN);
  m_cfg = new Settings(this);
}

int AntiquaCMD::abebooks() {
  QEventLoop loop;
  QTimer getTimer;
  AbeBooks *m = new AbeBooks(this);
  connect(&getTimer, SIGNAL(timeout()), &loop, SLOT(quit()));
  connect(m, SIGNAL(sendFinished()), &loop, SLOT(quit()));
  connect(m, SIGNAL(sendDisjointed()), &loop, SLOT(quit()));
  if (m->isAccessible())
    m->queryOrders();
  else
    return 1;

  getTimer.start((timeout * 1000));
  return loop.exec();
}

int AntiquaCMD::booklooker() {
  QEventLoop loop;
  QTimer getTimer;
  BookLooker *m = new BookLooker(this);
  connect(&getTimer, SIGNAL(timeout()), &loop, SLOT(quit()));
  connect(m, SIGNAL(sendFinished()), &loop, SLOT(quit()));
  connect(m, SIGNAL(sendDisjointed()), &loop, SLOT(quit()));
  if (m->isAccessible())
    m->queryOrders();
  else
    return 1;

  getTimer.start((timeout * 1000));
  return loop.exec();
}

int AntiquaCMD::buchfreund() {
  QEventLoop loop;
  QTimer getTimer;
  Buchfreund *m = new Buchfreund(this);
  connect(&getTimer, SIGNAL(timeout()), &loop, SLOT(quit()));
  connect(m, SIGNAL(sendFinished()), &loop, SLOT(quit()));
  connect(m, SIGNAL(sendDisjointed()), &loop, SLOT(quit()));
  if (m->isAccessible())
    m->queryOrders();
  else
    return 1;

  getTimer.start((timeout * 1000));
  return loop.exec();
}

void AntiquaCMD::queryAll() {
  qInfo("AntiquaCRM create requeset to Abebooks:");
  abebooks();
  qInfo("AntiquaCRM create requeset to Booklooker:");
  booklooker();
  qInfo("AntiquaCRM create requeset to Buchfreund:");
  buchfreund();
}

int AntiquaCMD::exec() {
  queryAll();
  return 0;
}
