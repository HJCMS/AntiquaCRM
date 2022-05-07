// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiqua_global.h"
#include "mapplication.h"

/* QtCore */
#include <QDebug>
#include <QLibraryInfo>
#include <QString>

int main(int argc, char *argv[]) {
  MApplication app(argc, argv);
  app.setApplicationName(ANTIQUACRM_NAME);
  app.setApplicationVersion(ANTIQUACRM_VERSION);
  app.setApplicationDisplayName(ANTIQUACRM_NAME);
  app.setDesktopFileName(ANTIQUACRM_NAME);
  app.setOrganizationDomain(HJCMS_CONFIG_DOMAIN);

  if (app.isRunning()) {
    qWarning("Application is already started!");
    return 0;
  }

  Q_INIT_RESOURCE(icons);

  return app.exec();
}
