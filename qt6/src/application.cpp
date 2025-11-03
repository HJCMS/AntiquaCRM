// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "application.h"
#include "mainwindow.h"
#include "splashscreen.h"
#include "switchdatabaseprofile.h"
#include "systemtrayicon.h"
#include "utils/datacache/datacache.h"

#ifdef Q_OS_WIN
#  include <Windows.h>
#else
#  include <chrono>
#  include <thread>
#endif

#ifdef ANTIQUACRM_DBUS_ENABLED
#  include "abusadaptor.h"
#  include <QDBusMessage>
#endif

#include <AntiquaWidgets>
#include <QMessageBox>
#include <QScreen>
#include <QStyle>
#include <QStyleFactory>
#include <QTimer>

#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif

// Normal abort to display the message about a missing network or SQL port in bootsplash.
#ifndef SILENT_QUIT
#ifndef SILENT_QUIT
# define SILENT_QUIT 0
#else
# define SILENT_QUIT EXITS_SUCCESS
#endif
#endif

Application::Application(int& argc, char** argv) : QApplication{argc, argv} {
  setApplicationName(ANTIQUACRM_NAME);
  setDesktopFileName(ANTIQUACRM_NAME);
  setApplicationVersion(ANTIQUACRM_VERSION);
  setOrganizationDomain(ANTIQUACRM_CONNECTION_DOMAIN);
  setQuitOnLastWindowClosed(false);
  // WARNING - Do not init Database Connections in constructors!
  m_cfg = new AntiquaCRM::ASettings(this);
}

#ifdef ANTIQUACRM_DBUS_ENABLED
bool Application::registerSessionBus() {
  m_dbus = new QDBusConnection(QDBusConnection::sessionBus());
  if (m_dbus->isConnected()) {
    if (m_dbus->registerService(ANTIQUACRM_CONNECTION_DOMAIN)) {
      m_dbus->registerObject(QString("/"), this);
      m_dbus->registerObject(QString("/Window"), m_window);
      if (checkSysTrayIcon())
        m_dbus->registerObject(QString("/Systray"), m_systray);
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

void Application::suspending() const {
#ifdef Q_OS_WIN
  Sleep(500);
#else
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
#endif
}

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
#ifdef ANTIQUA_DEVELOPMENT
  qInfo("Test SQL connection to %s:%d ...", qPrintable(_pr.getHostname()), _pr.getPort());
#endif
  if (iface.checkRemotePort(_pr.getHostname(), _pr.getPort()))
    return true;

  qWarning("Remote port „%s:%d“, is unreachable!", qPrintable(_pr.getHostname()), _pr.getPort());
  return false;
}

bool Application::checkSysTrayIcon() {
  if (!QSystemTrayIcon::isSystemTrayAvailable())
    qWarning("SystemTray is not available!");

  return (m_systray != nullptr);
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

bool Application::initMainWindow() {
  // We need a main widget for process trees into Windows taskbars.
  // This must be initialized before a taskbar entry is created.
  // Otherwise the application can freeze on startup :-(
  QWidget* m_topWidget = nullptr;
  foreach (QWidget* m_w, topLevelWidgets()) {
    if (m_w->isWindow()) {
      m_topWidget = m_w;
      break;
    }
  }
  // Now we can create the taskbar entry
  if (QSystemTrayIcon::isSystemTrayAvailable())
    m_systray = new SystemTrayIcon(applIcon(), this);

  // The MainWindow must initialized behind the taskbar entry,
  // otherwise it can't put the Window to the right process tree.
  m_window = new MainWindow(m_topWidget);
  m_window->setWindowIcon(applIcon());
  connect(m_window, SIGNAL(sendApplicationQuit()), SLOT(applicationQuit()));
  m_window->openWindow();

#ifdef Q_OS_WIN
  // @fixme worker threads in windows taskbar
  suspending();
#endif

  // Checks for System tray and create all required signal bindings.
  if (checkSysTrayIcon()) {
    connect(m_systray, SIGNAL(sendShowWindow()), m_window, SLOT(show()));
    connect(m_systray, SIGNAL(sendHideWindow()), m_window, SLOT(hide()));
    connect(m_systray, SIGNAL(sendToggleView()), m_window, SLOT(setToggleWindow()));
    connect(m_systray, SIGNAL(sendApplQuit()), SLOT(applicationQuit()));
    // ready to view
    m_systray->setVisible(true);
  }

  return (m_window != nullptr);
}

void Application::applicationQuit() {
  if (!m_window->closeWindow()) {
    m_window->showNormal();
    const QString _hint = tr("Please close all editors before exiting!");
    if (checkSysTrayIcon()) {
      m_systray->setMessage(_hint);
    } else {
      QMessageBox::warning(m_window, tr("AntiquaCRM"), _hint);
    }
    return;
  }

#ifdef ANTIQUACRM_DBUS_ENABLED
  m_dbus->unregisterObject(QString("/"), QDBusConnection::UnregisterTree);
  m_dbus->unregisterService(ANTIQUACRM_CONNECTION_DOMAIN);
#endif

  // Force destructers
  if (m_window != nullptr) {
    m_window->hide();
    m_window->deleteLater();
  }

  if (checkSysTrayIcon()) {
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
#ifdef ANTIQUACRM_DBUS_ENABLED
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
  QMutex mutex;
  // Translation at first
  initTranslations();

  // Step 0 - Open splash
  SplashScreen p_splash(this);
  p_splash.show();

  // Step 1 - Stylesheets
  p_splash.setMessage("Initial Themes & styles.");
  mutex.lock();
  initStyleTheme();
  mutex.unlock();

  // Step 2 - Networking
  p_splash.setMessage("Search Networkconnection!");
  mutex.lock();
  if (!checkInterfaces()) {
    p_splash.errorMessage(tr("No Networkconnection found!"));
    mutex.unlock();
    return SILENT_QUIT;
  }
  p_splash.setMessage(tr("Valid Networkconnection found!"));
  mutex.unlock();

  // Step 3 - SQL Server
  p_splash.setMessage(tr("Check Network server port!"));
  mutex.lock();
  if (!checkRemotePort()) {
    p_splash.errorMessage(tr("Network server port isn't reachable!"));
    mutex.unlock();
    suspending();
    return SILENT_QUIT;
  }
  p_splash.setMessage(tr("Network connection to remote port exists."));
  mutex.unlock();

  // Step 4 - SQL Database
  p_splash.setMessage(tr("Open Database connection."));
  mutex.lock();
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
    mutex.unlock();
    suspending();
    return EXIT_FAILURE;
  }
  p_splash.setMessage(tr("Database connection successfully."));
  mutex.unlock();

  // Step 5 - create cache files
  p_splash.setMessage(tr("Update application cache."));
  if (m_sql->open()) {
    mutex.lock();
    p_splash.setMessage(tr("Creating Cachefiles."));
    DataCache* m_cache = new DataCache(m_cfg, m_sql, this);
    connect(m_cache, SIGNAL(statusMessage(QString)), &p_splash, SLOT(setMessage(QString)));

    // m_sql->getDateTimeStamp();
    if (m_cache->createCaches()) {
      p_splash.setMessage(tr("Cachefiles updated ..."));
    }
    m_cache->deleteLater();
    mutex.unlock();
    p_splash.setMessage(tr("Open Application ..."));
    suspending();
  }

  // Step 6 - UIX
  if (!initMainWindow()) {
    p_splash.errorMessage(tr("Open window failed."));
    qFatal("failed to initital antiquacrm window");
    suspending();
    return EXIT_FAILURE;
  }

  // Step 7 - open window
  if (m_window != nullptr) {
#ifdef ANTIQUACRM_DBUS_ENABLED
    if (registerSessionBus()) {
      // qdbus-qt5 de.hjcms.antiquacrm / de.hjcms.antiquacrm.pushMessage shout
      ABusAdaptor* m_adaptor = new ABusAdaptor(this);
      m_adaptor->setObjectName(ANTIQUACRM_CONNECTION_DOMAIN);
      if (checkSysTrayIcon())
        connect(m_adaptor, SIGNAL(sendMessage(QString)), m_systray, SLOT(setMessage(QString)));

      connect(m_adaptor, SIGNAL(sendToggleView()), m_window, SLOT(setToggleWindow()));
      connect(m_adaptor, SIGNAL(sendAboutQuit()), SLOT(applicationQuit()));
    }
#endif
    // Step 8 - finish splash and unlock
    p_splash.finish(m_window);
    return QApplication::exec();
  }
  return EXIT_FAILURE;
}
