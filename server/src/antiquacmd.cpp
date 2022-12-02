// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquacmd.h"
#include "abebooks.h"
#include "booklooker.h"
#include "buchfreund.h"
#include "settings.h"
#include "sqlpsql.h"

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
  if (m->isAccessible())
    m->queryOrders();
  else
    return 1;

  getTimer.start((timeout * 1000));
  return loop.exec();
}

void AntiquaCMD::queryAll() {
  qInfo("ANTIQUACRM::queryAll()");
  qDebug() << "Abebooks:" << abebooks();

  qDebug() << "Booklooker:" << booklooker();

  qDebug() << "Buchfreund:" << buchfreund();
}

int AntiquaCMD::exec() {
  qDebug() << "AntiquaCMD::exec()";
  queryAll();
  return 0;
}
