// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquaappl.h"
#include "antiquasocketserver.h"
#include "antiquasplashscreen.h"
#include "antiquasystemtray.h"
#include "antiquawindow.h"
// AntiquaPreloader
#include "preloader.h"

#include <QDebug>
#ifdef Q_OS_WIN
#include <QColor>
#include <QPalette>
#endif
#include <QIcon>
#include <QLocalSocket>
#include <QStyleFactory>

static const QIcon applIcon() {
  QIcon icon(QString(":icons/antiqua.png"));
  return icon;
}

AntiquaAppl::AntiquaAppl(int &argc, char **argv) : QApplication{argc, argv} {
  m_cfg = new AntiquaCRM::ASettings(this);
  m_cfg->setObjectName("MainSettings");

  m_mainWindow = new AntiquaWindow();
  m_mainWindow->setObjectName("MainWindow");

  m_systemTray = new AntiquaSystemTray(applIcon(), this);
  m_systemTray->setObjectName("SystemTray");

  m_splash = new AntiquaSplashScreen(m_mainWindow);

  connect(m_mainWindow, SIGNAL(sendApplicationQuit()), this,
          SLOT(applicationQuit()));
}

bool AntiquaAppl::checkInterfaces() {
  m_splash->setMessage(tr("Search Networkconnection!"));
  AntiquaCRM::ANetworkIface iface;
  if (iface.connectedIfaceExists()) {
    m_splash->setMessage(tr("Valid Networkconnection found!"));
    qInfo("Networkconnection found!");
    return true;
  }
  m_splash->setMessage(tr("No Networkconnection found!"));
  qWarning("No Network connection found!");
  return false;
}

bool AntiquaAppl::checkRemotePort() {
  m_splash->setMessage(tr("Check Server Networkconnection!"));
  AntiquaCRM::ASqlSettings sqlConfig(this);
  QString host = sqlConfig.getParam("pg_hostname").toString();
  int port = sqlConfig.getParam("pg_port").toInt();
  AntiquaCRM::ANetworkIface iface;
  if (!iface.checkRemotePort(host, port)) {
    m_splash->setMessage(tr("Sql Server unreachable!"));
    qWarning("Sql Server unreachable!");
    return false;
  }
  m_splash->setMessage(tr("Remoteserver found!"));
  return true;
}

bool AntiquaAppl::checkDatabase() {
  m_splash->setMessage(tr("Open Database connection."));
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (m_sql->open()) {
    m_splash->setMessage(tr("Database connected!"));
    return true;
  }
  qWarning("No Database connected!");
  return false;
}

bool AntiquaAppl::createCacheFiles() {
  if (m_sql == nullptr)
    return false;

  // ASharedCacheFiles
  m_splash->setMessage(tr("Creating Cachefiles."));
  AntiquaPreloader *m_preload = new AntiquaPreloader(this);
  connect(m_preload, SIGNAL(statusMessage(const QString &)), m_splash,
          SLOT(setMessage(const QString &)));
  foreach(QRunnable *pr, m_preload->getThreads()) {
    // m_splash->setMessage(tr("Start ").arg(pr->name()));
    m_preload->start(pr);
  }
  m_splash->setMessage(tr("Completed..."));
  return true;
}

bool AntiquaAppl::createSocket() {
  bool out = false;
  m_splash->setMessage(tr("Create Socket."));
  m_socket = new AntiquaSocketServer(this);
  out = m_socket->listen(m_socket->name());
  m_splash->setMessage(out ? tr("done") : tr("failed"));
  return out;
}

bool AntiquaAppl::initialPlugins() {
  m_splash->setMessage(tr("Loading plugins!"));
  // m_cfg->getPluginDir();
  m_splash->setMessage(tr("Completed..."));
  return true;
}

void AntiquaAppl::applicationQuit() {
  // Systemtray
  m_systemTray->setVisible(false);
  // Windows
  m_mainWindow->close();
  // SQL
  m_sql->close();
  // Socket
  m_socket->close();
  // Todo: remove tempfiles

  quit();
}

void AntiquaAppl::initDefaultTheme() {
  setStyle(QStyleFactory::create("Fusion"));
  QFont font = qApp->font();
  QString fontdef = m_cfg->value("application/font", QString()).toString();
  if (!fontdef.isEmpty() && font.fromString(fontdef)) {
    qApp->setFont(font);
  }
#ifdef Q_OS_WIN
  /**
   * @short QStyle::Windows::Fusion
   * Hervorgehobener Inaktiver Hintergrund wird bei der Suche in Tabellen und
   * Listen nicht richtig dargestellt! Um ihn für den Klienten besser hervor zu
   * heben und es besser Sichtbar wird. Hier ein Workaround mit Gelber Farbe!
   * Die Standard Textfarbe in QStyle::Windows::Fusion ist "Schwarz".
   */
  QPalette p = qApp->palette();
  QColor lightYellow(255, 255, 127);
  p.setColor(QPalette::Inactive, QPalette::Highlight, lightYellow);
  qApp->setPalette(p);
#endif
}

bool AntiquaAppl::isRunning() {
  QLocalSocket socket(this);
  socket.setServerName(AntiquaSocketServer::name());
  if (socket.open(QLocalSocket::ReadWrite)) {
    QJsonObject obj;
    obj.insert("receiver", QJsonValue("MainWindow"));
    obj.insert("type", QJsonValue("SLOT"));
    obj.insert("value", QJsonValue("showAntiquaWindow"));
    QByteArray data(QJsonDocument(obj).toJson(QJsonDocument::Compact));
    socket.write(data);
    socket.waitForBytesWritten(2000);
    return true;
  }
  return false;
}

int AntiquaAppl::exec() {
  m_splash->show();

  QMutex mutex;
  // Step 1 - Socket erstellen
  mutex.lock();
  createSocket();
  mutex.unlock();

  // Step 2 - Netzwerkschnittstelln finden
  mutex.lock();
  if (!checkInterfaces()) {
    mutex.unlock();
    return 1;
  }
  mutex.unlock();

  // Step 3 - SQL Port Testen
  mutex.lock();
  if (!checkRemotePort()) {
    mutex.unlock();
    return 1;
  }
  mutex.unlock();

  // Step 4 - Datenbanktest durchführen
  mutex.lock();
  if (!checkDatabase()) {
    mutex.unlock();
    return 1;
  }
  mutex.unlock();

  // Step 5 - Temporäre Dateien erstellen.
  mutex.lock();
  createCacheFiles();
  mutex.unlock();

  // Step 6 - Plugins suchen und laden
  mutex.lock();
  initialPlugins();
  mutex.unlock();

  // Step 7 - Systemtray anzeigen
  m_splash->setMessage("Initial Systemtray.");
  m_systemTray->show();

  // Step 8 - Hauptfenster öffnen
  m_splash->setMessage("Start Antiqua CRM");
  m_splash->finish(m_mainWindow);
  m_mainWindow->openWindow();

  return QCoreApplication::exec();
}

AntiquaAppl::~AntiquaAppl() {
  if (m_splash != nullptr)
    m_splash->deleteLater();
}
