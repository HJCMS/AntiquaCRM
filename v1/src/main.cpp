// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquacrm.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[]) {
  AntiquaCRM *antiqua = new AntiquaCRM(argc, argv);
  antiqua->setApplicationName("antiquacrm");
  antiqua->setApplicationVersion("1.0.0");
  antiqua->setOrganizationDomain("de.hjcms.antiquacrm");
  antiqua->initDefaultTheme();

  if (QTextCodec::codecForLocale()->name() != "UTF-8") {
    qInfo("Setting utf8 for Textcodec ...");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  }

  if (antiqua->isRunning()) {
    qWarning("AntiquaCRM is already up!");
    return 0;
  }

  Q_INIT_RESOURCE(resources);

  return antiqua->exec();
}
