// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "application.h"
#include "mainwindow.h"
#include "systemtrayicon.h"

#include <AntiquaWidgets>
#include <QtCore>
#include <QScreen>
#include <QStyle>
#include <QStyleFactory>

Application::Application(int &argc, char **argv) : QApplication{argc, argv} {
  setApplicationName(ANTIQUACRM_NAME);
  setDesktopFileName(ANTIQUACRM_NAME);
  setApplicationVersion(ANTIQUACRM_VERSION);
  setOrganizationDomain(ANTIQUACRM_CONNECTION_DOMAIN);
  // WARNING - Do not init Database Connections in constructors!
  m_cfg = new AntiquaCRM::ASettings(this);
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

void Application::initTheme() {
  // AntiquaCRM using Fusion theme
  setStyle(QStyleFactory::create("Fusion"));

  // Required for System Desktop changes
  const QString _platform = platformName().toLower().trimmed();

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

void Application::initInterface() {
  // Window
  m_window = new MainWindow;
  m_window->setWindowIcon(applIcon());
  connect(m_window, SIGNAL(sendApplicationQuit()), SLOT(applicationQuit()));

  m_systray = new SystemTrayIcon(applIcon(), this);
  connect(m_systray, SIGNAL(sendShowWindow()), m_window, SLOT(show()));
  connect(m_systray, SIGNAL(sendHideWindow()), m_window, SLOT(hide()));
  connect(m_systray, SIGNAL(sendToggleView()), m_window,
          SLOT(setToggleWindow()));
  connect(m_systray, SIGNAL(sendApplQuit()), SLOT(applicationQuit()));
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
  return QIcon(QString("://icons/antiquacrm.png"));
}

bool Application::isRunning() {
  QLocalSocket socket(this);
  socket.setServerName(AntiquaCRM::AUtil::socketName());
  if (socket.open(QLocalSocket::ReadWrite)) {
    qInfo("Application already started.");
    socket.close();
    return true;
  }
  return false;
}

int Application::exec() {
  QMutex mutex;

  mutex.lock();
  initTheme();
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
  initInterface();
  if (m_window == nullptr) {
    qFatal("failed to initital window");
    return 2;
  }
  mutex.unlock();

  // last step - open components
  m_systray->show();
  m_window->openWindow();

  // TODO
  return QApplication::exec();
}
