/** @COPYRIGHT_HOLDER@ */

#include "mapplication.h"
#include "applsettings.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QLibraryInfo>
#include <QtCore/QTranslator>
#include <QtCore/QLocale>

MApplication::MApplication(int &argc, char **argv)
    : QApplication(argc, argv), sessionLock(lockFilePath()) {
  setObjectName("MApplicationApplication");
  setQuitOnLastWindowClosed(true);
  setApplicationName(ANTIQUACRM_NAME);
  setApplicationVersion(ANTIQUACRM_VERSION_STRING);
  setApplicationDisplayName(ANTIQUACRM_NAME);
  setDesktopFileName(ANTIQUACRM_NAME);
  setOrganizationDomain(ANTIQUACRM_CONFIG_DOMAIN);

  Q_INIT_RESOURCE(icons);

  QString d(applicationDirPath());
  QTranslator *transl = new QTranslator(this);
  if (transl->load(QString("%1/i18n/antiquacrm_%2").arg(d, QLocale().bcp47Name())))
  {
    qDebug("Translation loaded.");
    installTranslator(transl);
  }
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
