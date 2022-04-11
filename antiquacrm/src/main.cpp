// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "mapplication.h"
#include "version.h"

/* QtCore */
#include <QtCore/QDebug>
#include <QtCore/QString>

int main(int argc, char *argv[]) {
  /**
   * @TODO Create Sinlge Application

  if (QFile::exists(lockFilePath())) {
    qInfo("Allready Running.");
    return 0;
  }
   */
  MApplication app(argc, argv);
  return app.exec();
}
