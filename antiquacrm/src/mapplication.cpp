// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

/* Project */
#include "mapplication.h"
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

  m_config = new ApplSettings(this);
}

bool MApplication::initialSocketServer() {
  m_socket = new SocketServer(this);
  return m_socket->listen(m_socket->name());
}

bool MApplication::initTranslations() {
  QString d(applicationDirPath());
  d.append(QDir::separator());

#ifndef ANTIQUA_DEVELOPEMENT
#ifdef ANTIQUA_LSB_DATADIR
  d = QString(ANTIQUA_LSB_DATADIR);
#endif
#endif

  d.append("i18n");
  d.append(QDir::separator());

  QTranslator *transl = new QTranslator(this);
  if (transl->load(d + "antiquacrm_de")) {
    installTranslator(transl);
    return true;
  }
  return false;
}

void MApplication::openAssistant() {
  Assistant assistant;
  assistant.exec();
}

bool MApplication::isRunning() {
  QLocalSocket socket(this);
  socket.setServerName(SocketServer::name());
  return socket.open(QLocalSocket::ReadOnly);
}

void MApplication::initThemeStyle() {
  setStyle(QStyleFactory::create("Fusion"));
  QFont font = qApp->font();
  QString fontdef = m_config->value("application/font", QString()).toString();
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

  if (m_config->needAssistant()) {
    openAssistant();
    return 0;
  }

  // SQL Database
  m_sql = new HJCMS::SqlCore(this);
  if (!m_sql->sqlDriversExists()) {
    openAssistant();
    return 0;
  }

  if (!initialSocketServer()) {
    qFatal("socket server is down ...");
    return 1;
  }

  if (!m_sql->initialDatabase()) {
    qWarning("Database connection failed ...");
    openAssistant();
    return 0;
  }

  m_window = new MWindow();
  connect(m_socket, SIGNAL(statusMessage(const QString &)), m_window,
          SLOT(statusMessage(const QString &)));
  connect(m_window, SIGNAL(s_connectDatabase(bool)), m_sql,
          SLOT(openDatabase(bool)));
  connect(m_sql, SIGNAL(s_connectionStatus(bool)), m_window,
          SLOT(connectionStatus(bool)));
  if (m_window == nullptr) {
    qFatal("Application error");
    return 0;
  }

  m_window->show();
  m_window->setFocus(Qt::ActiveWindowFocusReason);
  m_window->activateWindow();
  m_window->initDefaults();

  return QApplication::exec();
}

MApplication::~MApplication() {
  SocketServer::removeServer(SocketServer::name());
  if (m_sql != nullptr) {
    m_sql->close();
  }
}
