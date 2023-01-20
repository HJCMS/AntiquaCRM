// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "assistant.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[]) {
  QApplication appl(argc, argv);
  if (QTextCodec::codecForLocale()->name() != "UTF-8") {
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  }

  Q_INIT_RESOURCE(assistant);

  Assistant assistant;
  return assistant.exec();
}
