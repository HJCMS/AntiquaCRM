/** @COPYRIGHT_HOLDER@ */

#include "mapplication.h"
#include "applsettings.h"
#include "version.h"
#include <QtCore/QDebug>

MApplication::MApplication(int &argc, char **argv)
    : QApplication(argc, argv), sessionLock(lockFilePath()) {
  setObjectName("MApplicationApplication");
  setQuitOnLastWindowClosed(true);
  setApplicationName(ANTIQUACRM_NAME);
  setApplicationVersion(ANTIQUACRM_VERSION_STRING);
  setApplicationDisplayName(ANTIQUACRM_NAME);
  setDesktopFileName(ANTIQUACRM_NAME);
  setOrganizationDomain(HJCMSFQDN);
}

/**
 * @brief MApplication::exec
 * @return int
 */
int MApplication::exec() {
  if (sessionLock.isLocked()) {
    qInfo("The application is already running.");
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
  m_mainWindow->connectSqlDatabase();

  sessionLock.lock();
  return QApplication::exec();
}

MApplication::~MApplication() { sessionLock.unlock(); }
