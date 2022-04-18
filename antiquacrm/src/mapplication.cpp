/** @COPYRIGHT_HOLDER@ */
/* Project */
#include "mwindow.h"
#include "mapplication.h"
#include "applsettings.h"
#include "sqlcore.h"
#include "version.h"

#include <QtCore/QDebug>

MApplication::MApplication(int &argc, char **argv)
    : QApplication(argc, argv) {
  setObjectName("MApplicationApplication");
  setQuitOnLastWindowClosed(true);

  m_settings = new ApplSettings(this);
}

/**
 * @brief MApplication::exec
 * @return int
 */
int MApplication::exec() {
  // SQL Database
  HJCMS::SqlCore *m_postgreSQL = new HJCMS::SqlCore(this);
  if (!m_postgreSQL->sqlDriversExists()) {
    qInfo("TODO QWizzard Dialog first Access");
    return 0;
  }

  qInfo("Open Database connection ...");
  if (!m_postgreSQL->initialDatabase()) {
    qWarning("Database connection failed ...");
    return 0;
  }

  m_mainWindow = new MWindow();
  if (m_mainWindow == nullptr) {
    qFatal("Application error");
    return 0;
  }

  m_mainWindow->show();
  m_mainWindow->setFocus(Qt::ActiveWindowFocusReason);
  m_mainWindow->activateWindow();

  return QApplication::exec();
}

MApplication::~MApplication() {}
