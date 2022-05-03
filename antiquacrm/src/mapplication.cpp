// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

/* Project */
#include "mapplication.h"
#include "applsettings.h"
#include "mwindow.h"
#include "socketserver.h"
#include "sqlcore.h"
#include "antiqua_global.h"
#include "assistant.h"

#include <QDebug>
#include <QLocalSocket>

MApplication::MApplication(int &argc, char **argv) : QApplication(argc, argv) {
  setObjectName("MApplicationApplication");
  setQuitOnLastWindowClosed(true);

  m_settings = new ApplSettings(this);
}

bool MApplication::initialSocketServer() {
  m_socket = new SocketServer(this);
  return m_socket->listen(m_socket->name());
}

bool MApplication::isRunning() {
  QLocalSocket socket(this);
  socket.setServerName(SocketServer::name());
  return socket.open();
}

/**
 * @brief MApplication::exec
 * @return int
 */
int MApplication::exec() {
  // First Run ?
  if(m_settings->value("postgresql/hostname").toString().isEmpty())
  {
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
  if(m_sqlDB != nullptr)
  {
    m_sqlDB->close();
  }
}
