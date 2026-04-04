// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "application.h"
#include "mainwindow.h" // IWYU pragma: keep
#include "splashscreen.h"
#include "switchdatabaseprofile.h"
#include "utils/datacache/datacache.h"
#include "systemtrayicon.h"

#ifdef Q_OS_LINUX
# include "abusadaptor.h"
# include <QDBusMessage>
#endif

#include <AntiquaWidgets>
#include <QMessageBox>
#include <QScreen>
#include <QStyle>
#include <QStyleFactory>
#include <QTimer>

// Normal abort to display the message about a missing network or SQL port in bootsplash.
#ifndef EXIT_SUCCESS
# define SILENT_QUIT 0
#else
# define SILENT_QUIT EXIT_SUCCESS
#endif

#ifndef EXIT_FAILURE
# define EXIT_FAILURE 1
#endif

Application::Application(int& argc, char** argv) : QApplication{argc, argv} {
  setApplicationName(ANTIQUACRM_NAME);
  setDesktopFileName(ANTIQUACRM_NAME);
  setApplicationVersion(ANTIQUACRM_VERSION);
  setOrganizationDomain(ANTIQUACRM_CONNECTION_DOMAIN);
  setQuitOnLastWindowClosed(false);

  // WARNING - Do not init Database Connections in constructors!
  m_cfg = new AntiquaCRM::ASettings(this);
  m_cfg->setValue("name", ANTIQUACRM_NAME);
  m_cfg->setValue("version", ANTIQUACRM_VERSION);
}

#ifdef QT_DBUS_LIB
bool Application::registerSessionBus() {
  m_dbus = new QDBusConnection(QDBusConnection::sessionBus());
  if (m_dbus->isConnected()) {
    if (m_dbus->registerService(ANTIQUACRM_CONNECTION_DOMAIN)) {
      m_dbus->registerObject(QString("/"), this);
      m_dbus->registerObject(QString("/Window"), m_window);
      if (quitOnLastWindowClosed()) {
        m_dbus->registerObject(QString("/Systray"), m_systray);
      }
    }
#  ifdef ANTIQUA_DEVELOPMENT
    else {
      qDebug() << "QDBus::registerService" << m_dbus->lastError().message();
    }
#  endif
    return true;
  }
  return false;
}
#endif

bool Application::checkInterfaces() {
  AntiquaCRM::ANetworkIface iface;
  if (iface.connectedIfaceExists())
    return true;

  qWarning("No Network connection found!");
  return false;
}

bool Application::checkRemotePort() {
  AntiquaCRM::ASqlSettings _csql(this);
  AntiquaCRM::ASqlProfile _pr = _csql.connectionProfile();
  AntiquaCRM::ANetworkIface iface;
  qInfo("PgSQL config test %s:%d ...", qPrintable(_pr.getHostname()), _pr.getPort());
  if (iface.checkRemotePort(_pr.getHostname(), _pr.getPort()))
    return true;

  qWarning("Remote port „%s:%d“, is unreachable!", qPrintable(_pr.getHostname()), _pr.getPort());
  return false;
}

bool Application::openDatabase() {
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (m_sql->status())
    return true;

#ifdef ANTIQUA_DEVELOPMENT
  qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif

  if (m_window != nullptr)
    m_window->hide();

  return false;
}

void Application::initStyleTheme() {
  Q_INIT_RESOURCE(application);

  // AntiquaCRM using Fusion theme
  setStyle(QStyleFactory::create("Fusion"));

  // Required for System Desktop changes
  const QString _platform = platformName().toLower().trimmed();

  if (_platform.startsWith("xcb")) {
    const QString _fallback("hicolor");
    QIcon::setFallbackThemeName(_fallback);
    QIcon::setThemeName(m_cfg->value("icon_theme", _fallback).toString());
  }

  // NOTE Loading stylesheet before change fonts!
  QFileInfo _info(m_cfg->getDataDir(), "antiquacrm.qcss");
  if (_info.isReadable()) {
    QFile _fp(_info.filePath());
    if (_fp.exists() && _fp.open(QFile::ReadOnly)) {
      QString buffer;
      QTextStream in(&_fp);
      while (!in.atEnd()) {
        buffer.append(in.readLine());
      }
      _fp.close();
      setStyleSheet(buffer);
    }
  }

  QPalette _palette = palette();
  // @fixme XDesktop themes
  if (_platform.startsWith("xcb")) {
    const QColor _rgb = _palette.color(QPalette::PlaceholderText).toRgb();
    if (!AntiquaCRM::AColorLuminance(this).checkForeground(_rgb)) {
      _palette.setColor(QPalette::PlaceholderText, Qt::darkGray);
    }
  }
  // @fixme Windows theme
  if (_platform.startsWith("windows")) {
    QFont _font = font();
    QString _fontdef = m_cfg->value("font", _font.toString()).toString();
    if (!_fontdef.isEmpty() && _font.fromString(_fontdef)) {
      setFont(_fontdef);
    }
    QColor _highlight(255, 255, 127);
    _palette.setColor(QPalette::Inactive, QPalette::Highlight, _highlight);
  }
  setPalette(_palette);
}

void Application::initTranslations() {
  const QDir _dir(m_cfg->getTranslationDir());
  if (!_dir.isReadable()) {
    qWarning("No access to %s", qPrintable(_dir.path()));
    return;
  }
  QTranslator* m_qtr = new QTranslator(this);
  if (m_qtr->load(QLocale::system(), "antiquacrm", "_", _dir.path(), ".qm"))
    installTranslator(m_qtr);
}

void Application::applicationQuit() {
  if (!m_window->closeWindow()) {
    m_window->showNormal();
    const QString _hint = tr("Please close all editors before exiting!");
    m_systray->setMessage(_hint);
    return;
  }

#ifdef QT_DBUS_LIB
  m_dbus->unregisterObject(QString("/"), QDBusConnection::UnregisterTree);
  m_dbus->unregisterService(ANTIQUACRM_CONNECTION_DOMAIN);
#endif

  // Force destructers
  if (m_window != nullptr) {
    m_window->hide();
    m_window->deleteLater();
  }

  if (m_systray != nullptr) {
    m_systray->setVisible(false);
    m_systray->deleteLater();
  }

  if (m_sql != nullptr) {
    m_sql->close();
    m_sql->deleteLater();
  }

  // finaly
  quit();
}

const QIcon Application::applIcon() {
  return AntiquaCRM::qrcIcon("antiquacrm");
}

bool Application::isRunning() {
  QLocalSocket socket(this);
  socket.setServerName(AntiquaCRM::AUtil::socketName());
  if (socket.open(QLocalSocket::ReadWrite)) {
#ifdef QT_DBUS_LIB
    QDBusConnection _dbc =
        QDBusConnection::connectToBus(QDBusConnection::SessionBus, ANTIQUACRM_CONNECTION_DOMAIN);
    if (_dbc.isConnected()) {
      const QDBusMessage _call = QDBusMessage::createMethodCall(
          ANTIQUACRM_CONNECTION_DOMAIN, "/", _dbc.name(), qUtf8Printable("toggle"));
      _dbc.call(_call, QDBus::NoBlock);
    }
#endif
    socket.close();
    return true;
  }
  return false;
}

int Application::exec() {
  // Translation at first
  initTranslations();

  // Step 0 - Open splash
  SplashScreen p_splash(this);
  p_splash.show();

  // Step 1 - Stylesheets
  p_splash.setMessage("Initial Themes & styles.");
  initStyleTheme();

         // Step 2 - Networking
  p_splash.setMessage("Search Networkconnection!");
  if (!checkInterfaces()) {
    p_splash.errorMessage(tr("No Networkconnection found!"));
    return SILENT_QUIT;
  }
  p_splash.setMessage(tr("Valid Networkconnection found!"));

  // Step 3 - SQL Server
  p_splash.setMessage(tr("Check Network server port!"));
  if (!checkRemotePort()) {
    p_splash.errorMessage(tr("Network server port isn't reachable!"));
    return SILENT_QUIT;
  }
  p_splash.setMessage(tr("Network connection to remote port exists."));

  // Step 4 - SQL Database
  p_splash.setMessage(tr("Open Database connection."));
  if (!openDatabase()) {
    p_splash.errorMessage(tr("SQL Server connection unsuccessful!"));
    SwitchDatabaseProfile _dbd(m_cfg, &p_splash);
    if (m_sql != nullptr)
      _dbd.setRemoteInfo(m_sql->lastError());

    if (_dbd.exec() == QDialog::Rejected) {
      qInfo("No database profile changes.");
    } else {
      qInfo("Database profile changed, application restart required.");
    }
    return EXIT_FAILURE;
  }
  p_splash.setMessage(tr("Database connection successfully."));

  // Step 5 - create cache files
  p_splash.setMessage(tr("Update application cache."));
  if (m_sql->open()) {
    p_splash.setMessage(tr("Creating Cachefiles."));
    DataCache* m_cache = new DataCache(m_cfg, m_sql, this);
    connect(m_cache, SIGNAL(statusMessage(QString)), &p_splash, SLOT(setMessage(QString)));

    if (m_cache->createCaches()) {
      p_splash.setMessage(tr("Cachefiles updated ..."));
    }
    m_cache->deleteLater();
    p_splash.setMessage(tr("Open Application ..."));
  }

  // Step 6 - UIX
  m_window = new MainWindow(m_cfg);
  m_window->setWindowIcon(applIcon());
  connect(m_window, SIGNAL(sendApplicationQuit()), SLOT(applicationQuit()));

  // @note wait for SocketDescriptor
  m_systray = new SystemTrayIcon(applIcon(), this);
  connect(m_systray, SIGNAL(sendApplQuit()), SLOT(applicationQuit()));
  connect(m_systray, SIGNAL(sendShowWindow()), m_window, SLOT(show()));
  connect(m_systray, SIGNAL(sendHideWindow()), m_window, SLOT(hide()));
  connect(m_systray, SIGNAL(sendToggleView()), m_window, SLOT(setToggleWindow()));

  // Step 7 - open window
#ifdef QT_DBUS_LIB
  if (registerSessionBus()) {
    // qdbus6 de.hjcms.antiquacrm / de.hjcms.antiquacrm.pushMessage test-string
    ABusAdaptor* m_adaptor = new ABusAdaptor(this);
    m_adaptor->setObjectName(ANTIQUACRM_CONNECTION_DOMAIN);
    connect(m_adaptor, SIGNAL(sendMessage(QString)), m_systray, SLOT(setMessage(QString)));
    connect(m_adaptor, SIGNAL(sendToggleView()), m_window, SLOT(setToggleWindow()));
    connect(m_adaptor, SIGNAL(sendAboutQuit()), SLOT(applicationQuit()));
  }
#endif
  // Step 8 - finish splash and unlock
  p_splash.finish(m_window);

  m_systray->setVisible(true);
  m_window->openWindow();

  return QApplication::exec();
}
