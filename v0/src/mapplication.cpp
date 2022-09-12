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
#include <QMessageBox>
#include <QNetworkInterface>
#include <QStyleFactory>
#include <QTimer>
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
  QDir p = ApplSettings::getDataTarget();
  QTranslator *transl = new QTranslator(this);
  if (transl->load(QLocale::system(), "antiquacrm", "_", p.filePath("i18n"),
                   ".qm")) {
    installTranslator(transl);
    return true;
  }
  qDebug() << Q_FUNC_INFO << p.filePath("i18n");
  return false;
}

void MApplication::openAssistant() {
  Assistant assistant;
  assistant.exec();
}

void MApplication::shutdown() {
  if (m_window == nullptr)
    return;

  m_window->deleteLater();
  quit();
}

bool MApplication::isRunning() {
  QLocalSocket socket(this);
  socket.setServerName(SocketServer::name());
  if (socket.open(QLocalSocket::ReadWrite)) {
    QByteArray data("showAntiquaWindow");
    socket.write(data);
    socket.waitForBytesWritten(2000);
    return true;
  }
  return false;
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

  int ipcheck = 0;
  foreach (QNetworkInterface net, QNetworkInterface::allInterfaces()) {
    if (net.flags() & !(QNetworkInterface::IsUp | QNetworkInterface::IsRunning))
      continue;

    if (net.flags() & QNetworkInterface::IsLoopBack)
      continue;

    if (net.type() & (QNetworkInterface::Ethernet | QNetworkInterface::Wifi)) {
      foreach (QNetworkAddressEntry a, net.addressEntries()) {
        if (a.ip().isGlobal() && !a.ip().isLinkLocal())
          ipcheck++;
      }
    }
  }

  if (ipcheck < 1) {
    QStringList info;
    info << tr("No valid Network Interfaces found!");
    info << tr("Please check your Network configuration.");
    QMessageBox::critical(nullptr, tr("Network"), info.join("\n"));
    return 0;
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
  connect(m_window, SIGNAL(sendShutdown()), this, SLOT(shutdown()));
  connect(m_socket, SIGNAL(showWindow()), m_window, SLOT(show()));

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
