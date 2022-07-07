// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiqua_global.h"
#include "mapplication.h"

#include <QApplication>
#include <QDebug>
#include <QTextCodec>

int main(int argc, char *argv[]) {
  MApplication *antiqua = new MApplication(argc, argv);
  antiqua->setApplicationName(ANTIQUACRM_NAME);
  antiqua->setApplicationVersion(ANTIQUACRM_VERSION);
  antiqua->setApplicationDisplayName(ANTIQUACRM_NAME);
  antiqua->setDesktopFileName(ANTIQUACRM_NAME);
  antiqua->setOrganizationDomain(HJCMS_CONFIG_DOMAIN);
  antiqua->initThemeStyle();

  if (QTextCodec::codecForLocale()->name() != "UTF-8") {
    qInfo("Setting utf8 for Textcodec ...");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  }

  if (antiqua->isRunning()) {
    qWarning("Application is already started!");
    return 0;
  }

  Q_INIT_RESOURCE(icons);

  return antiqua->exec();
}
