// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquaappl.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[]) {
  AntiquaAppl *antiqua = new AntiquaAppl(argc, argv);
  antiqua->setApplicationName("antiquacrm");
  antiqua->setApplicationVersion("1.0.0");
  antiqua->setOrganizationDomain("de.hjcms.antiquacrm");
  antiqua->setQuitOnLastWindowClosed(true);
  antiqua->initDefaultTheme();

  if (QTextCodec::codecForLocale()->name() != "UTF-8") {
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  }

  if (antiqua->isRunning()) {
    qWarning("AntiquaCRM is already up!");
    return 0;
  }

  Q_INIT_RESOURCE(resources);

  return antiqua->exec();
}
