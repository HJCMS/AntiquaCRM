// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquaappl.h"
#include "antiquasplashscreen.h"
#include "antiquasystemtray.h"
#include "antiquawindow.h"
#include "cachebuilder.h"
#ifdef ANTIQUACRM_DBUS_ENABLED
#include "abusadaptor.h"
#endif

#include <QDebug>
#ifdef Q_OS_WIN
#include <QColor>
#include <QPalette>
#endif
#include <QLocalSocket>
#include <QMessageBox>
#include <QMetaObject>
#include <QMutex>
#include <QStyleFactory>
#include <QTranslator>

AntiquaAppl::AntiquaAppl(int &argc, char **argv) : QApplication{argc, argv} {
  setApplicationName(ANTIQUACRM_NAME);
  setApplicationVersion(ANTIQUACRM_VERSION);
  setOrganizationDomain(ANTIQUACRM_CONNECTION_DOMAIN);
  /**
   * @warning Do not init Database Connections in constructors!
   */
  m_cfg = new AntiquaCRM::ASettings(this);
  m_cfg->setObjectName("application_settings");
#ifdef ANTIQUACRM_DBUS_ENABLED
  m_dbus = nullptr;
#endif
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

void AntiquaAppl::initGui() {
  m_mainWindow = new AntiquaWindow();
  m_mainWindow->setObjectName("MainWindow");
  m_systemTray = new AntiquaSystemTray(applIcon(), this);
  m_systemTray->setObjectName("SystemTray");

#ifdef ANTIQUACRM_DBUS_ENABLED
  ABusAdaptor *m_adaptor = new ABusAdaptor(this);
  m_adaptor->setObjectName(ANTIQUACRM_CONNECTION_DOMAIN);
  connect(m_adaptor, SIGNAL(sendMessage(const QString &)), m_systemTray,
          SLOT(setMessage(const QString &)));
  connect(m_adaptor, SIGNAL(sendToggleView()), m_mainWindow,
          SLOT(setToggleWindow()));
  connect(m_adaptor, SIGNAL(sendAboutQuit()), SLOT(applicationQuit()));

  m_dbus = new QDBusConnection(QDBusConnection::sessionBus());
  if (m_dbus->registerService(ANTIQUACRM_CONNECTION_DOMAIN)) {
    m_dbus->registerObject(QString("/"), this);
  }
#endif

  // SIGNALS:AntiquaSystemTray
  connect(m_systemTray, SIGNAL(sendShowWindow()), m_mainWindow, SLOT(show()));
  connect(m_systemTray, SIGNAL(sendHideWindow()), m_mainWindow, SLOT(hide()));
  connect(m_systemTray, SIGNAL(sendToggleView()), m_mainWindow,
          SLOT(setToggleWindow()));
  connect(m_systemTray, SIGNAL(sendApplQuit()), SLOT(applicationQuit()));
  // SIGNALS:AntiquaWindow
  connect(m_mainWindow, SIGNAL(sendApplQuit()), SLOT(applicationQuit()));
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
  AntiquaCRM::ASqlSettings sqlConfig(applicationName(), this);
  QString host = sqlConfig.getParam("pg_hostname").toString();
  int port = sqlConfig.getParam("pg_port").toInt();
  AntiquaCRM::ANetworkIface iface;
  if (!iface.checkRemotePort(host, port)) {
    QMessageBox::critical(nullptr, tr("SQL Server Connection"),
                          tr("The SQL Server '%1' is unreachable!").arg(host));
    qFatal("Sql Server unreachable!");
    return false;
  }
  return true;
}

bool AntiquaAppl::checkDatabase() {
  m_sql = new AntiquaCRM::ASqlCore(applicationName(), this);
  if (m_sql->open()) {
    return true;
  }
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << m_sql->lastError();
#endif
  return false;
}

void AntiquaAppl::applicationQuit() {
  // Sind Editoren geöffnet?
  if (!m_mainWindow->checkBeforeClose()) {
    m_mainWindow->show();
    m_systemTray->setMessage(tr("Please close all editors before exiting!"));
    return;
  }
  // Systemtray
  m_systemTray->setVisible(false);
  // Window
  m_mainWindow->close();
  // SQL
  m_sql->close();

#ifdef ANTIQUACRM_DBUS_ENABLED
  m_dbus->unregisterObject(QString("/"), QDBusConnection::UnregisterTree);
  m_dbus->unregisterService(ANTIQUACRM_CONNECTION_DOMAIN);
#endif

  // Force destructers
  if (m_mainWindow != nullptr)
    m_mainWindow->deleteLater();

  if (m_systemTray != nullptr)
    m_systemTray->deleteLater();

  // finaly
  quit();
}

const QIcon AntiquaAppl::applIcon() {
  QIcon icon(QString("://icons/antiqua.png"));
  return icon;
}

void AntiquaAppl::initDefaultTheme() {
  setStyle(QStyleFactory::create("Fusion"));
#ifdef Q_OS_WIN
  // qDebug() << QStyleFactory::keys();
  QFont font = qApp->font();
  qDebug() << font.toString();
  QString fontdef = m_cfg->value("font", font.toString()).toString();
  if (!fontdef.isEmpty() && font.fromString(fontdef)) {
    qApp->setFont(font);
  }
  // @short QStyle::Windows::Fusion
  QPalette p = qApp->palette();
  QColor lightYellow(255, 255, 127);
  p.setColor(QPalette::Inactive, QPalette::Highlight, lightYellow);
  qApp->setPalette(p);
#endif
  QStringList customCSS;
  customCSS << "QTabBar::tab:selected {color: palette(highlight);}";
  customCSS << "QPushButton:hover {color:palette(highlight);}";
  customCSS << "QRadioButton:checked {color:palette(highlight);}";
#ifdef Q_OS_LINUX
  // @bug KDE Layout
  customCSS << "QGroupBox::title{padding-right:10px;}";
#endif

  setStyleSheet(customCSS.join("\n"));
}

bool AntiquaAppl::isRunning() {
  QLocalSocket socket(this);
  socket.setServerName(AntiquaCRM::AUtil::socketName());
  if (socket.open(QLocalSocket::ReadWrite)) {
    qInfo("Application already started.");
    socket.close();
    return true;
  }
  return false;
}

int AntiquaAppl::exec() {
  initTranslations();
  initGui();

  AntiquaSplashScreen p_splashScreen(m_mainWindow);
  p_splashScreen.show();

  QMutex mutex;
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

  // Step 4 - sql connection test
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
  if (m_sql->db().isOpen()) {
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
  } else {
    qWarning("AntiquaCRM: cache update skipped!");
  }

  // Step 7 - finish splash and unlock
  p_splashScreen.setMessage("Start Antiqua CRM");
  p_splashScreen.finish(m_mainWindow);

  // Step 8 - open application window
  m_mainWindow->openWindow();

  return QCoreApplication::exec();
}
