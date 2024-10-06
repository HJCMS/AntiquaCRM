// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include "assistant.h"
#include <QApplication>

int main(int argc, char* argv[]) {
  QApplication appl(argc, argv);
  Q_INIT_RESOURCE(antiqua_assistant);
  Assistant assistant;
  return assistant.exec();
}
