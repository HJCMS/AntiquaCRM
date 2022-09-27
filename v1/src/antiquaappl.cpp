// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquaappl.h"
#include "antiquasystemtray.h"
#include "antiquawindow.h"

#include <QDebug>
#include <QIcon>

static const QIcon applIcon() {
  QIcon icon(QString(":icons/antiqua.png"));
  return icon;
}

AntiquaAppl::AntiquaAppl(int &argc, char **argv) : QApplication{argc, argv} {
  m_mainWindow = new AntiquaWindow();
  m_systemTray = new AntiquaSystemTray(applIcon(), this);
}

void AntiquaAppl::initDefaultTheme() {}

bool AntiquaAppl::isRunning() { return false; }

int AntiquaAppl::exec() {
  QMutex mutex;
  mutex.lock();
  m_sql = new AntiquaCRM::ASqlCore(this);
  qInfo("Open Database connection");
  if (!m_sql->open()) {
    qFatal("no database connection");
    mutex.unlock();
    return 134;
  }
  mutex.unlock();

  // QString test("SELECT count(i_id) FROM inventory;");
  qDebug() << m_sql->record("inventory");

  if (m_mainWindow != nullptr) {
    m_mainWindow->show();
    m_systemTray->show();
  }

  return QCoreApplication::exec();
}

AntiquaAppl::~AntiquaAppl() {
  if (m_sql != nullptr)
    m_sql->close();
}
