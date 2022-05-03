// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "mapplication.h"
#include "antiqua_global.h"

/* QtCore */
#include <QDebug>
#include <QLibraryInfo>
#include <QLocale>
#include <QString>
#include <QTranslator>

int main(int argc, char *argv[]) {
  MApplication app(argc, argv);
  app.setApplicationName(ANTIQUACRM_NAME);
  app.setApplicationVersion(ANTIQUACRM_VERSION_STRING);
  app.setApplicationDisplayName(ANTIQUACRM_NAME);
  app.setDesktopFileName(ANTIQUACRM_NAME);
  app.setOrganizationDomain(HJCMS_CONFIG_DOMAIN);

  if (app.isRunning()) {
    qWarning("Application is already started!");
    return 0;
  }

  Q_INIT_RESOURCE(icons);

  QString d(app.applicationDirPath());
  QTranslator *transl = new QTranslator(&app);
  if (transl->load(
          QString("%1/i18n/antiquacrm_%2").arg(d, QLocale().bcp47Name()))) {
    qDebug("Translation loaded.");
    app.installTranslator(transl);
  }

  return app.exec();
}
