// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquaappl.h"
#include "antiquawindow.h"
#include "systemtray.h"

#include <QIcon>

static const QIcon applIcon() {
  QIcon icon(QString(":icons/antiqua.png"));
  return icon;
}

AntiquaAppl::AntiquaAppl(int &argc, char **argv) : QApplication{argc, argv} {
  m_mainWindow = new AntiquaWindow();
  m_systemTray = new SystemTray(applIcon(), this);
}

void AntiquaAppl::initDefaultTheme() {

}

bool AntiquaAppl::isRunning() { return false; }

int AntiquaAppl::exec() {
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (!m_sql->open()) {
    qFatal("no database connection");
    return 134;
  }

  if (m_mainWindow != nullptr) {
    m_mainWindow->show();
    m_systemTray->show();
  }
  return QCoreApplication::exec();
}

AntiquaAppl::~AntiquaAppl() {}
