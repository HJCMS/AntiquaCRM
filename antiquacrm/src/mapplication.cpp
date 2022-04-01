/** @COPYRIGHT_HOLDER@ */

#include "mapplication.h"
#include "applsettings.h"
#include "sqlcore.h"
#include "version.h"
#include <QtCore/QByteArray>
#include <QtCore/QDebug>

MApplication::MApplication(int &argc, char **argv)
    : QApplication(argc, argv), sessionLock(lockFilePath())
{
  setObjectName("MApplicationApplication");
  setQuitOnLastWindowClosed(true);
  setApplicationName(ANTIQUACRM_NAME);
  setApplicationVersion(ANTIQUACRM_VERSION_STRING);
  setApplicationDisplayName(ANTIQUACRM_NAME);
  setDesktopFileName(ANTIQUACRM_NAME);
  setOrganizationDomain(HJCMSFQDN);

  // Start QMainWindow init
  m_mainWindow = new MWindow();
  connect(this, SIGNAL(postMessage(const QString &)),
    m_mainWindow, SIGNAL(setStatusMessage(const QString &)));
}

/**
 * @brief MApplication::initDatabase
 */
bool MApplication::initDatabase() {
  m_db = new HJCMS::SqlCore(this);
  if(m_db->database->isOpen())
  {
      qInfo("PostgreSQL connection exists: %s",
            qPrintable(m_db->database->connectionName()));
      return true;
  }
  return false;
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
  if (!m_mainWindow)
    return 0;

  if (!initDatabase()) {
    // TODO First Access Client Helper
    qWarning("%s", qPrintable("No SQL Connection"));
  }

  m_mainWindow->show();
  m_mainWindow->setFocus(Qt::ActiveWindowFocusReason);
  m_mainWindow->activateWindow();
  sessionLock.lock();
  return QApplication::exec();
}

MApplication::~MApplication() { sessionLock.unlock(); }
