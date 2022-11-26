// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquaappl.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[]) {
  AntiquaAppl *antiqua = new AntiquaAppl(argc, argv);
  antiqua->setQuitOnLastWindowClosed(false);
  antiqua->initDefaultTheme();

  if (QTextCodec::codecForLocale()->name() != "UTF-8") {
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  }

  if (argc > 0) {
    for(int i = 0; i <= argc; i++) {
      QString arg = QString(argv[i]).toLower();
      arg.replace("-", "");
      arg.replace(" ", "");
      if (arg.trimmed() == "assistant") {
        antiqua->startAssistant();
        return 0;
      }
    }
  }

  if (antiqua->isRunning()) {
    qWarning("AntiquaCRM is already up!");
    return 0;
  }

  Q_INIT_RESOURCE(resources);

  return antiqua->exec();
}
