// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquacrm.h"
#include "antiquawindow.h"
#include "systemtray.h"

#include <QIcon>

static const QIcon applIcon() {
  QIcon icon(QString(":icons/antiqua.png"));
  return icon;
}

AntiquaCRM::AntiquaCRM(int &argc, char **argv) : QApplication{argc, argv} {
  m_mainWindow = new AntiquaWindow();
  m_systemTray = new SystemTray(applIcon(), this);
}

void AntiquaCRM::initDefaultTheme() {}

bool AntiquaCRM::isRunning() { return false; }

int AntiquaCRM::exec() {
  if (m_mainWindow != nullptr) {
    m_mainWindow->show();
    m_systemTray->show();
  }
  return QCoreApplication::exec();
}

AntiquaCRM::~AntiquaCRM() {}
