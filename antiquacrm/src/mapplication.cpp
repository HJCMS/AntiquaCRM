// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

/* Project */
#include "mapplication.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "assistant.h"
#include "mwindow.h"
#include "socketserver.h"
#include "sqlcore.h"

#include <QDebug>
#include <QDir>
#include <QFont>
#include <QLocalSocket>
#include <QLocale>
#include <QStyleFactory>
#include <QTranslator>

MApplication::MApplication(int &argc, char **argv) : QApplication(argc, argv) {
  setObjectName("antiquacrm_application");
  setQuitOnLastWindowClosed(true);

  m_settings = new ApplSettings(this);
}

bool MApplication::initialSocketServer() {
  m_socket = new SocketServer(this);
  return m_socket->listen(m_socket->name());
}

bool MApplication::initTranslations() {
  QString d(applicationDirPath());
  d.append(QDir::separator());
  d.append("i18n");
  d.append(QDir::separator());

  QTranslator *transl = new QTranslator(this);
  if (transl->load(d + "antiquacrm_de")) {
    installTranslator(transl);
    return true;
  }
  return false;
}

bool MApplication::isRunning() {
  QLocalSocket socket(this);
  socket.setServerName(SocketServer::name());
  return socket.open(QLocalSocket::ReadOnly);
}

void MApplication::initThemeStyle() {
  setStyle(QStyleFactory::create("Fusion"));
  QFont font = qApp->font();
  QString fontdef = m_settings->value("application/font", QString()).toString();
  if (!fontdef.isEmpty() && font.fromString(fontdef)) {
    qApp->setFont(font);
  }
}

/**
 * @brief MApplication::exec
 * @return int
 */
int MApplication::exec() {
  if (!initTranslations()) {
    qWarning("Translation not loaded!");
  }

  // First Run ?
  if (m_settings->value("postgresql/hostname").toString().isEmpty()) {
    Assistant assistant;
    assistant.exec();
    qDebug() << "assistant";
    return 0;
  }

  // SQL Database
  m_sqlDB = new HJCMS::SqlCore(this);
  if (!m_sqlDB->sqlDriversExists()) {
    qInfo("TODO QWizzard Dialog first Access");
    return 0;
  }

  if (!initialSocketServer()) {
    qFatal("socket server is down ...");
    return 1;
  }

  if (!m_sqlDB->initialDatabase()) {
    qWarning("Database connection failed ...");
    return 0;
  }

  m_mainWindow = new MWindow();
  connect(m_socket, SIGNAL(statusMessage(const QString &)), m_mainWindow,
          SLOT(statusMessage(const QString &)));
  if (m_mainWindow == nullptr) {
    qFatal("Application error");
    return 0;
  }

  m_mainWindow->show();
  m_mainWindow->setFocus(Qt::ActiveWindowFocusReason);
  m_mainWindow->activateWindow();
  m_mainWindow->initDefaults();

  return QApplication::exec();
}

MApplication::~MApplication() {
  SocketServer::removeServer(SocketServer::name());
  if (m_sqlDB != nullptr) {
    m_sqlDB->close();
  }
}
