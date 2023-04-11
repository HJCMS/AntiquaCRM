// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "application.h"
#include "mainwindow.h"

#include <AntiquaWidgets>
#include <QJsonObject>
#include <QList>
#include <QMutex>
#include <QScreen>
#include <QStyle>
#include <QStyleFactory>

Application::Application(int &argc, char **argv) : QApplication{argc, argv} {
  setApplicationName(ANTIQUACRM_NAME);
  setApplicationVersion(ANTIQUACRM_VERSION);
  setOrganizationDomain(ANTIQUACRM_CONNECTION_DOMAIN);
  // WARNING - Do not init Database Connections in constructors!
  m_cfg = new AntiquaCRM::ASettings(this);
}

bool Application::initTranslations() {
  QString path = m_cfg->getTranslationDir().path();
  QTranslator *transl = new QTranslator(this);
  if (transl->load(QLocale::system(), "antiquacrm", "_", path, ".qm")) {
    installTranslator(transl);
    return true;
  }
  return false;
}

void Application::initGui() {
  // Window
  m_window = new MainWindow;
  m_window->setObjectName("MainWindow");
  m_window->setWindowIcon(applIcon());
  connect(m_window, SIGNAL(sendApplicationQuit()), SLOT(applicationQuit()));
}

bool Application::checkInterfaces() {
  AntiquaCRM::ANetworkIface iface;
  if (iface.connectedIfaceExists())
    return true;

  qWarning("No Network connection found!");
  return false;
}

bool Application::checkRemotePort() {
  qInfo("TODO Network remote ports");
  return true;
}

bool Application::checkDatabase() {
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (m_sql->open()) {
    return true;
  }
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << m_sql->lastError();
#endif
  return false;
}

void Application::applicationQuit() {
  // TODO

  // finaly
  quit();
}

const QIcon Application::applIcon() {
  return QIcon(QString("://icons/antiquacrm.png"));
}

void Application::initTheme() {
  setStyle(QStyleFactory::create("Fusion"));

// Bugfix: Qt6 Linux
#ifdef Q_OS_LINUX
  QPalette _p = palette();
  QColor _phtc = _p.color(QPalette::PlaceholderText).toRgb();
  if (!AntiquaCRM::AColorLuminance(this).checkForeground(_phtc))
    _p.setColor(QPalette::PlaceholderText, Qt::darkGray);
#endif

// Bugfix Qt6 Windows fonts and highlight style
#ifdef Q_OS_WIN
  QFont _font = font();
  QString _fontdef = m_cfg->value("font", _font.toString()).toString();
  if (!_fontdef.isEmpty() && _font.fromString(_fontdef)) {
    setFont(_fontdef);
  }
  QColor lightYellow(255, 255, 127);
  _p.setColor(QPalette::Inactive, QPalette::Highlight, lightYellow);
#endif

  setPalette(_p);

  QStringList customCSS;
  customCSS << "QTabBar::tab:selected {color: palette(highlight);}";
  customCSS << "QPushButton:hover {color:palette(highlight);}";
  customCSS << "QRadioButton:checked {color:palette(highlight);}";
#ifdef Q_OS_LINUX
  // Bugfix: KDE Themes-Layout
  customCSS << "QGroupBox::title{padding-right:10px;}";
#endif

  // qDebug() << Q_FUNC_INFO << QStyleFactory::keys() << font().toString();

  setStyleSheet(customCSS.join("\n"));
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
  // initTranslations();

  mutex.lock();
  initGui();
  if (m_window == nullptr) {
    qFatal("failed to initital widgets");
    return 2;
  }
  mutex.unlock();

  // last step - open application window
  m_window->openWindow();

  // TODO
  return QApplication::exec();
}
