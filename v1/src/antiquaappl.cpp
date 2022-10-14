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
#include <QMetaObject>
#include <QStyleFactory>
#include <QTranslator>

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
  m_cache->createCaches();
  return true;
}

bool AntiquaAppl::createSocket() {
  bool out = false;
  m_socket = new AntiquaSocketServer(this);
  out = m_socket->listen(m_socket->name());
  return out;
}

bool AntiquaAppl::initTranslations() {
  QString path = m_cfg->getTranslationDir().path();
  QTranslator *transl = new QTranslator(this);
  if (transl->load(QLocale::system(), "antiquacrm", "_", path, ".qm")) {
    installTranslator(transl);
    return true;
  }
  return false;
}

bool AntiquaAppl::initialPlugins(QObject *receiver) {
  p_interfaces.clear();
  AntiquaCRM::APluginLoader *m_pl = new AntiquaCRM::APluginLoader(this);
  QListIterator<AntiquaCRM::APluginInterface *> i(m_pl->pluginInterfaces(this));
  while (i.hasNext()) {
    AntiquaCRM::APluginInterface *m_iface = i.next();
    if (m_iface != nullptr) {
      QString name(m_iface->displayName());
      QString msg = tr("Plugin %1 found and loading ...").arg(qPrintable(name));
      if (receiver != nullptr) {
        QMetaObject::invokeMethod(receiver, "setMessage", Qt::DirectConnection,
                                  Q_ARG(QString, msg));
      } else {
        emit sendStatusMessage(msg);
      }
      p_interfaces.append(m_iface);
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
  AntiquaSplashScreen p_splashScreen(m_mainWindow);
  p_splashScreen.show();

  QMutex mutex;
  // Translation
  mutex.lock();
  p_splashScreen.setMessage(tr("Initial Translation ..."));
  if (initTranslations()) {
    p_splashScreen.setMessage(tr("done."));
  }
  mutex.unlock();

  // Step 1. Socket
  mutex.lock();
  p_splashScreen.setMessage(tr("Create Socket ..."));
  if (createSocket()) {
    p_splashScreen.setMessage(tr("Socket created"));
  } else {
    p_splashScreen.setMessage(tr("Socket failed!"));
  }
  mutex.unlock();

  // Step 2 - Netzwerkschnittstelln finden
  p_splashScreen.setMessage(tr("Search Networkconnection!"));
  mutex.lock();
  if (!checkInterfaces()) {
    p_splashScreen.setMessage(tr("No Networkconnection found!"));
    mutex.unlock();
    return 1;
  }
  p_splashScreen.setMessage(tr("Valid Networkconnection found!"));
  mutex.unlock();

  // Step 3 - SQL Port Testen
  mutex.lock();
  p_splashScreen.setMessage(tr("Check SQL Server connection!"));
  if (!checkRemotePort()) {
    p_splashScreen.setMessage(tr("SQL Server unreachable!"));
    mutex.unlock();
    return 1;
  }
  p_splashScreen.setMessage(tr("SQL Server found!"));
  mutex.unlock();

  // Step 4 - Datenbanktest durchführen
  mutex.lock();
  p_splashScreen.setMessage(tr("Open Database connection."));
  if (!checkDatabase()) {
    qFatal("No Database connected!");
    mutex.unlock();
    return 1;
  }
  p_splashScreen.setMessage(tr("Database connected!"));
  mutex.unlock();

  // Step 5 - Temporäre Dateien erstellen.
  mutex.lock();
  p_splashScreen.setMessage(tr("Creating Cachefiles."));
  if (createCacheFiles()) {
    p_splashScreen.setMessage(tr("Cachefiles completed ..."));
  } else {
    p_splashScreen.setMessage(tr("Create Cachefile failed ..."));
  }
  mutex.unlock();

  // Step 6 - Plugins suchen und laden
  mutex.lock();
  p_splashScreen.setMessage(tr("Loading plugins!"));
  if (initialPlugins(&p_splashScreen)) {
    p_splashScreen.setMessage(tr("Start Providers requests ..."));
    startTriggerProcess();
  }
  p_splashScreen.setMessage(tr("Completed..."));
  mutex.unlock();

  // Step 7 - Systemtray anzeigen
  p_splashScreen.setMessage("Initial Systemtray.");
  m_systemTray->show();

  // Step 8 - Hauptfenster öffnen
  p_splashScreen.setMessage("Start Antiqua CRM");
  // Der Startbildschirm sperrt weitere SQL Anfragen.
  // Muss deshalb an dieser Stelle beendet werden!
  p_splashScreen.finish(m_mainWindow);

  // Step 9 - Timer starten
  m_timer->restart();

  // Step 10 - Fenster öffnen
  m_mainWindow->openWindow();

  return QCoreApplication::exec();
}

AntiquaAppl::~AntiquaAppl() {}
