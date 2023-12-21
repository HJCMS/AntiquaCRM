// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "application.h"
#include "mainwindow.h"
#include "systemtrayicon.h"
#ifdef ANTIQUACRM_DBUS_ENABLED
#include "antiquabusadaptor.h"
#include <QDBusMessage>
#endif

#include <AntiquaWidgets>
#include <QScreen>
#include <QStyle>
#include <QStyleFactory>
#include <QtCore>

Application::Application(int &argc, char **argv) : QApplication{argc, argv} {
  setApplicationName(ANTIQUACRM_NAME);
  setDesktopFileName(ANTIQUACRM_NAME);
  setApplicationVersion(ANTIQUACRM_VERSION);
  setOrganizationDomain(ANTIQUACRM_CONNECTION_DOMAIN);
  // WARNING - Do not init Database Connections in constructors!
  m_cfg = new AntiquaCRM::ASettings(this);
}

#ifdef ANTIQUACRM_DBUS_ENABLED
bool Application::registerSessionBus() {
  m_dbus = new QDBusConnection(QDBusConnection::sessionBus());
  if (m_dbus->registerService(ANTIQUACRM_CONNECTION_DOMAIN)) {
    m_dbus->registerObject(QString("/"), this);
    m_dbus->registerObject(QString("/Window"), m_window);
    m_dbus->registerObject(QString("/Systray"), m_systray);
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
  if (iface.checkRemotePort(_pr.getHostname(), _pr.getPort()))
    return true;

  qWarning("PostgreSQL Connection profile „%s“, is unreachable!",
           qPrintable(_csql.getProfile()));
  return false;
}

bool Application::openDatabase() {
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (m_sql->open()) {
    return true;
  }
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << m_sql->lastError();
#else
  qFatal("No Database connection!");
#endif
  return false;
}

void Application::initStyleTheme() {
  Q_INIT_RESOURCE(application);

  // AntiquaCRM using Fusion theme
  setStyle(QStyleFactory::create("Fusion"));

  // Required for System Desktop changes
  const QString _platform = platformName().toLower().trimmed();

  const QString _fallback("hicolor");
  QIcon::setFallbackThemeName(_fallback);
  QIcon::setThemeName(m_cfg->value("icon_theme", _fallback).toString());

  QPalette _palette = palette();
  // @fixme KDE theme
  if (_platform.startsWith("xcb")) {
    QColor _rgb = _palette.color(QPalette::PlaceholderText).toRgb();
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

  // Loading stylesheet
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
}

bool Application::initTranslations() {
  qInfo("Translation is not activated!");
  return true;

  QString _p = m_cfg->getTranslationDir().path();
  QTranslator *m_qtr = new QTranslator(this);
  if (m_qtr->load(QLocale::system(), "antiquacrm", "_", _p, ".qm")) {
    installTranslator(m_qtr);
    return true;
  }
  return false;
}

void Application::initGUI() {
  // Window
  m_window = new MainWindow;
  m_window->setWindowIcon(applIcon());
  connect(m_window, SIGNAL(sendApplicationQuit()), SLOT(applicationQuit()));
  // SysTray
  m_systray = new SystemTrayIcon(applIcon(), this);
  connect(m_systray, SIGNAL(sendShowWindow()), m_window, SLOT(show()));
  connect(m_systray, SIGNAL(sendHideWindow()), m_window, SLOT(hide()));
  connect(m_systray, SIGNAL(sendToggleView()), m_window,
          SLOT(setToggleWindow()));
  connect(m_systray, SIGNAL(sendApplQuit()), SLOT(applicationQuit()));

#ifdef ANTIQUACRM_DBUS_ENABLED
  // qdbus-qt5 de.hjcms.antiquacrm / de.hjcms.antiquacrm.pushMessage testing
  if (registerSessionBus()) {
    AntiquaBusAdaptor *m_adaptor = new AntiquaBusAdaptor(this);
    m_adaptor->setObjectName(ANTIQUACRM_CONNECTION_DOMAIN);
    connect(m_adaptor, SIGNAL(sendMessage(const QString &)), m_systray,
            SLOT(setMessage(const QString &)));
    connect(m_adaptor, SIGNAL(sendToggleView()), m_window,
            SLOT(setToggleWindow()));
    connect(m_adaptor, SIGNAL(sendAboutQuit()), SLOT(applicationQuit()));
  } else {
    qWarning("No Dbus registration!");
  }
#endif
}

void Application::applicationQuit() {
  // close
  if (!m_window->closeWindow()) {
    m_window->showNormal();
    m_systray->setMessage(tr("Please close all editors before exiting!"));
    return;
  }
  m_systray->setVisible(false);
  m_sql->close();

#ifdef ANTIQUACRM_DBUS_ENABLED
  m_dbus->unregisterObject(QString("/"), QDBusConnection::UnregisterTree);
  m_dbus->unregisterService(ANTIQUACRM_CONNECTION_DOMAIN);
#endif

  // Force destructers
  if (m_window != nullptr)
    m_window->deleteLater();

  if (m_systray != nullptr)
    m_systray->deleteLater();

  if (m_sql != nullptr)
    m_sql->deleteLater();

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
    QDBusConnection bus = QDBusConnection::connectToBus(
        QDBusConnection::SessionBus, ANTIQUACRM_CONNECTION_DOMAIN);
    if (bus.isConnected()) {
      bus.call(QDBusMessage::createMethodCall(
                   ANTIQUACRM_CONNECTION_DOMAIN, // Service
                   "/", bus.name(), "toggle"),
               QDBus::NoBlock);
    }
#endif
    socket.close();
    return true;
  }
  return false;
}

int Application::exec() {
  QMutex mutex;

  mutex.lock();
  initStyleTheme();
  mutex.unlock();

  mutex.lock();
  initTranslations();
  mutex.unlock();

  mutex.lock();
  checkInterfaces();
  mutex.unlock();

  mutex.lock();
  checkRemotePort();
  mutex.unlock();

  mutex.lock();
  openDatabase();
  mutex.unlock();

  mutex.lock();
  initGUI();
  if (m_window == nullptr) {
    qFatal("failed to initital window");
    return 2;
  }
  mutex.unlock();

  // last step - open components
  m_systray->show();
  m_window->openWindow();

  return QApplication::exec();
}
