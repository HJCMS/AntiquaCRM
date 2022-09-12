// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquacrm.h"
#include "antiquawindow.h"

AntiquaCRM::AntiquaCRM(int &argc, char **argv) : QApplication{argc, argv} {
  m_mainWindow = new AntiquaWindow();
}

void AntiquaCRM::initDefaultTheme() {}

bool AntiquaCRM::isRunning() {
  return false;
}

int AntiquaCRM::exec() {
  if (m_mainWindow != nullptr) {
    m_mainWindow->show();
  }
  return QCoreApplication::exec();
}

AntiquaCRM::~AntiquaCRM() {}
