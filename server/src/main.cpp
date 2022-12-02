// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquacmd.h"

#include <QDebug>

int main(int argc, char *argv[]) {
  AntiquaCMD *m_appl = new AntiquaCMD(argc, argv);
  return m_appl->exec();
}
