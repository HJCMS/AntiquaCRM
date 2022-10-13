// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquaappl.h"
#include "antiquasocketserver.h"
#include "antiquasplashscreen.h"
#include "antiquasystemtray.h"
#include "antiquatimer.h"
#include "antiquawindow.h"
#include "cachefiles.h"

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
  m_timer = new AntiquaTimer(this);

  connect(m_systemTray, SIGNAL(sendShowWindow()), m_mainWindow, SLOT(show()));
  connect(m_systemTray, SIGNAL(sendHideWindow()), m_mainWindow, SLOT(hide()));
  connect(m_systemTray, SIGNAL(sendToggleView()), m_mainWindow, SLOT(toggle()));
  connect(m_systemTray, SIGNAL(sendApplQuit()), this, SLOT(applicationQuit()));
  connect(m_mainWindow, SIGNAL(sendApplQuit()), this, SLOT(applicationQuit()));
  connect(m_timer, SIGNAL(sendTrigger()), this, SLOT(startTriggerProcess()));
}

bool AntiquaAppl::checkInterfaces() {
  AntiquaCRM::ANetworkIface iface;
  if (iface.connectedIfaceExists()) {
    return true;
  }
  qWarning("No Network connection found!");
  return false;
}

bool AntiquaAppl::checkRemotePort() {
  AntiquaCRM::ASqlSettings sqlConfig(this);
  QString host = sqlConfig.getParam("pg_hostname").toString();
  int port = sqlConfig.getParam("pg_port").toInt();
  AntiquaCRM::ANetworkIface iface;
  if (!iface.checkRemotePort(host, port)) {
    qWarning("Sql Server unreachable!");
    return false;
  }
  return true;
}

bool AntiquaAppl::checkDatabase() {
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (m_sql->open()) {
    return true;
  }
  return false;
}

bool AntiquaAppl::createCacheFiles() {
  if (m_sql == nullptr)
    return false;

  AntiquaCacheFiles *m_cache = new AntiquaCacheFiles(this);
  connect(m_cache, SIGNAL(statusMessage(const QString &)), m_splash,
          SLOT(setMessage(const QString &)));
  m_cache->createCaches();
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
  p_interfaces.clear();
  AntiquaCRM::APluginLoader *m_pl = new AntiquaCRM::APluginLoader(this);
  QListIterator<AntiquaCRM::APluginInterface *> i(m_pl->pluginInterfaces(this));
  while (i.hasNext()) {
    AntiquaCRM::APluginInterface *m_iface = i.next();
    if (m_iface != nullptr) {
      QString name(m_iface->displayName());
      QString msg = tr("Plugin %1 found, loading ...").arg(qPrintable(name));
      m_splash->setMessage(msg);
      p_interfaces.append(m_iface);
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << "AntiquaAppl:" << msg;
#endif
    }
  }
  return (p_interfaces.size() > 0);
}

void AntiquaAppl::startTriggerProcess() {
  bool connection = checkRemotePort();
  m_systemTray->setConnectionStatus(connection);
  if (connection && p_interfaces.size() > 0) {
    QListIterator<AntiquaCRM::APluginInterface *> i(p_interfaces);
    while (i.hasNext()) {
      AntiquaCRM::APluginInterface *m_iface = i.next();
      if (m_iface != nullptr) {
        connect(m_iface, SIGNAL(sendQueryFinished()), this,
                SLOT(setPluginQueryFinished()));
        m_iface->queryOrders();
      }
    }
  }
}

void AntiquaAppl::setPluginQueryFinished() {
  AntiquaCRM::APluginInterface *m_iface =
      qobject_cast<AntiquaCRM::APluginInterface *>(sender());
  if (m_iface != nullptr) {
    // AntiquaCRM::AProviderOrders
    qDebug() << m_iface->displayName() << m_iface->getResponse();
  }
}

void AntiquaAppl::applicationQuit() {
  // Systemtray
  m_systemTray->setVisible(false);
  // Window
  m_mainWindow->close();
  // SQL
  m_sql->close();
  // Socket
  m_socket->close();
  // finaly
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
  m_splash->setMessage(tr("Creating Cachefiles."));
  if (createSocket()) {
    m_splash->setMessage(tr("Cachefiles completed ..."));
  } else {
    m_splash->setMessage(tr("Create Cachefile failed ..."));
  }
  mutex.unlock();

  // Step 2 - Netzwerkschnittstelln finden
  m_splash->setMessage(tr("Search Networkconnection!"));
  mutex.lock();
  if (!checkInterfaces()) {
    m_splash->setMessage(tr("No Networkconnection found!"));
    mutex.unlock();
    return 1;
  }
  m_splash->setMessage(tr("Valid Networkconnection found!"));
  mutex.unlock();

  // Step 3 - SQL Port Testen
  mutex.lock();
  m_splash->setMessage(tr("Check SQL Server connection!"));
  if (!checkRemotePort()) {
    m_splash->setMessage(tr("SQL Server unreachable!"));
    mutex.unlock();
    return 1;
  }
  m_splash->setMessage(tr("SQL Server found!"));
  mutex.unlock();

  // Step 4 - Datenbanktest durchführen
  mutex.lock();
  m_splash->setMessage(tr("Open Database connection."));
  if (!checkDatabase()) {
    qFatal("No Database connected!");
    mutex.unlock();
    return 1;
  }
  m_splash->setMessage(tr("Database connected!"));
  mutex.unlock();

  // Step 5 - Temporäre Dateien erstellen.
  mutex.lock();
  createCacheFiles();
  mutex.unlock();

  // Step 6 - Plugins suchen und laden
  mutex.lock();
  m_splash->setMessage(tr("Loading plugins!"));
  if (initialPlugins()) {
    m_splash->setMessage(tr("Start Providers requests ..."));
    startTriggerProcess();
  }
  m_splash->setMessage(tr("Completed..."));
  mutex.unlock();

  // Step 7 - Systemtray anzeigen
  m_splash->setMessage("Initial Systemtray.");
  m_systemTray->show();

  // Step 8 - Hauptfenster öffnen
  m_splash->setMessage("Start Antiqua CRM");
  m_splash->finish(m_mainWindow);
  m_mainWindow->openWindow();
  m_timer->restart();

  return QCoreApplication::exec();
}

AntiquaAppl::~AntiquaAppl() {
  if (m_splash != nullptr)
    m_splash->deleteLater();
}
