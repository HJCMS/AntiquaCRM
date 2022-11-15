// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquaappl.h"
#include "antiquasocketserver.h"
#include "antiquasplashscreen.h"
#include "antiquasystemtray.h"
#include "antiquatimer.h"
#include "antiquawindow.h"
#include "cachebuilder.h"

#include <QDebug>
#ifdef Q_OS_WIN
#include <QColor>
#include <QPalette>
#endif
#include <QIcon>
#include <QLocalSocket>
#include <QMetaObject>
#include <QMutex>
#include <QStyleFactory>
#include <QTranslator>

static const QIcon applIcon() {
  QIcon icon(QString(":icons/antiqua.png"));
  return icon;
}

AntiquaAppl::AntiquaAppl(int &argc, char **argv) : QApplication{argc, argv} {
  setApplicationName(ANTIQUACRM_NAME);
  setApplicationVersion(ANTIQUACRM_VERSION);
  setOrganizationDomain(ANTIQUACRM_CONNECTION_DOMAIN);

  m_cfg = new AntiquaCRM::ASettings(this);
  m_cfg->setObjectName("application_settings");

  m_orderSystem = new OrderSystem(this);
}

void AntiquaAppl::initGui() {
  m_mainWindow = new AntiquaWindow();
  m_mainWindow->setObjectName("MainWindow");
  m_systemTray = new AntiquaSystemTray(applIcon(), this);
  m_systemTray->setObjectName("SystemTray");
  m_timer = new AntiquaTimer(this);

  // SIGNALS:AntiquaSystemTray
  connect(m_systemTray, SIGNAL(sendShowWindow()), m_mainWindow, SLOT(show()));
  connect(m_systemTray, SIGNAL(sendHideWindow()), m_mainWindow, SLOT(hide()));
  connect(m_systemTray, SIGNAL(sendToggleView()), m_mainWindow,
          SLOT(setToggleWindow()));
  connect(m_systemTray, SIGNAL(sendApplQuit()), SLOT(applicationQuit()));
  // SIGNALS:AntiquaWindow
  connect(m_mainWindow, SIGNAL(sendApplQuit()), SLOT(applicationQuit()));
  // SIGNALS:AntiquaTimer
  connect(m_timer, SIGNAL(sendTrigger()), SLOT(startTriggerProcess()));
  // SIGNALS:OrderSystem
  connect(m_orderSystem, SIGNAL(sendNewOrdersArrived(const QString &)),
          m_systemTray, SLOT(setOrdersMessage(const QString &)));
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
      connect(m_iface, SIGNAL(sendQueryFinished()), this,
              SLOT(setPluginQueryFinished()));
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
  int secs = 1;
  bool connection = checkRemotePort();
  m_systemTray->setConnectionStatus(connection);
  if (connection && p_interfaces.size() > 0) {
    QListIterator<AntiquaCRM::APluginInterface *> i(p_interfaces);
    while (i.hasNext()) {
      AntiquaCRM::APluginInterface *m_iface = i.next();
      if (m_iface != nullptr) {
        m_iface->queryNewOrders(secs++);
      }
    }
  }
}

/**
 * @brief Pluginabfrage auf neue Bestellungen
 * Nehme von QList<AProviderOrder> die Anzahl neuer Bestellungen.
 * Sind neue Bestellungen vorhanden speichere Sie in die Datenbank.
 * @note Wir müssen hier wegen den SQL-Queries einen Mutex verwenden.
 */
void AntiquaAppl::setPluginQueryFinished() {
  QMutex mutex;
  AntiquaCRM::APluginInterface *m_iface =
      qobject_cast<AntiquaCRM::APluginInterface *>(sender());
  if (m_iface != nullptr) {
    mutex.lock();
    AntiquaCRM::AProviderOrders orders = m_iface->getOrders();
    if (orders.size() > 0) {
      m_orderSystem->updateOrders(m_iface->displayName(), orders);
    }
    mutex.unlock();
  }
}

void AntiquaAppl::applicationQuit() {
  // Systemtray
  m_systemTray->setVisible(false);
  // Window
  m_mainWindow->close();
  // SQL
  m_sql->close();
  // finaly
  quit();
}

void AntiquaAppl::initDefaultTheme() {
#ifdef Q_OS_WIN
  // qDebug() << QStyleFactory::keys();
  setStyle(QStyleFactory::create("Fusion"));
  QFont font = qApp->font();
  qDebug() << font.toString();
  QString fontdef = m_cfg->value("font", font.toString()).toString();
  if (!fontdef.isEmpty() && font.fromString(fontdef)) {
    qApp->setFont(font);
  }
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
  socket.setServerName(AntiquaSocketServer::socketPath());
  if (socket.open(QLocalSocket::ReadWrite)) {
    qInfo("Application already started.");
    socket.close();
    return true;
  }
  return false;
}

int AntiquaAppl::exec() {
  initGui();

  AntiquaSplashScreen p_splashScreen(m_mainWindow);
  p_splashScreen.show();

  QMutex mutex;
  // Step 1 - load Translation
  mutex.lock();
  p_splashScreen.setMessage(tr("Initial Translation ..."));
  if (initTranslations()) {
    p_splashScreen.setMessage(tr("done."));
  }
  mutex.unlock();

  // Step 2 - show systemtray
  p_splashScreen.setMessage("Initial Systemtray.");
  m_systemTray->show();

  // Step 3 - network connection test
  p_splashScreen.setMessage(tr("Search Networkconnection!"));
  mutex.lock();
  if (!checkInterfaces()) {
    p_splashScreen.setMessage(tr("No Networkconnection found!"));
    mutex.unlock();
    return 1;
  }
  p_splashScreen.setMessage(tr("Valid Networkconnection found!"));
  mutex.unlock();

  // Step 4 - sql conenction test
  mutex.lock();
  p_splashScreen.setMessage(tr("Check SQL Server connection!"));
  if (!checkRemotePort()) {
    p_splashScreen.setMessage(tr("SQL Server unreachable!"));
    mutex.unlock();
    return 1;
  }
  p_splashScreen.setMessage(tr("SQL Server found!"));
  mutex.unlock();

  // Step 5 - test database
  mutex.lock();
  p_splashScreen.setMessage(tr("Open Database connection."));
  if (!checkDatabase()) {
    qFatal("No Database connected!");
    mutex.unlock();
    return 1;
  }
  p_splashScreen.setMessage(tr("Database connected!"));
  mutex.unlock();

  // Step 6 - create cache files
  mutex.lock();
  p_splashScreen.setMessage(tr("Creating Cachefiles."));
  CacheBuilder *m_cache = new CacheBuilder(this);
  connect(m_cache, SIGNAL(statusMessage(const QString &)), &p_splashScreen,
          SLOT(setMessage(const QString &)));
  if (m_cache->createCaches()) {
    p_splashScreen.setMessage(tr("Cachefiles completed ..."));
  } else {
    p_splashScreen.setMessage(tr("Create Cachefile failed ..."));
  }
  m_cache->deleteLater();
  mutex.unlock();

  // Step 7 - loading plugins
  mutex.lock();
  p_splashScreen.setMessage(tr("Loading plugins!"));
  if (initialPlugins(&p_splashScreen)) {
    p_splashScreen.setMessage(tr("Start Providers requests ..."));
    startTriggerProcess();
  }
  p_splashScreen.setMessage(tr("Completed..."));
  mutex.unlock();

  // Step 8 - finish splash and unlock
  p_splashScreen.setMessage("Start Antiqua CRM");
  p_splashScreen.finish(m_mainWindow);

  // Step 9 - start network listener
  m_timer->restart();

  // Step 10 - open application window
  m_mainWindow->openWindow();

  return QCoreApplication::exec();
}

AntiquaAppl::~AntiquaAppl() {}
