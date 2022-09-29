// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#include "antiquaappl.h"
#include "antiquasplashscreen.h"
#include "antiquasystemtray.h"
#include "antiquawindow.h"

#include <QDebug>
#ifdef Q_OS_WIN
#include <QColor>
#include <QPalette>
#endif
#include <QIcon>
#include <QStyleFactory>

static const QIcon applIcon() {
  QIcon icon(QString(":icons/antiqua.png"));
  return icon;
}

AntiquaAppl::AntiquaAppl(int &argc, char **argv) : QApplication{argc, argv} {
  m_cfg = new AntiquaCRM::ASettings(this);
  m_mainWindow = new AntiquaWindow();
  m_systemTray = new AntiquaSystemTray(applIcon(), this);
  m_splash = new AntiquaSplashScreen(m_mainWindow);
  m_splash->show();
  m_splash->setMessage(tr("Open Database connection."));
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

bool AntiquaAppl::isRunning() { return false; }

int AntiquaAppl::exec() {
  QMutex mutex;
  m_splash->setMessage(tr("Search Networkconnection!"));
  mutex.lock();
  AntiquaCRM::ANetworkIface iface;
  if (iface.connectedIfaceExists()) {
    m_splash->setMessage(tr("Valid Networkconnection found!"));
    qInfo("Networkconnection found!");
  } else {
    m_splash->setMessage(tr("No Networkconnection found!"));
    qWarning("No Network connection found!");
    mutex.unlock();
    return 1;
  }
  mutex.unlock();
  m_splash->setMessage(tr("Check Server Networkconnection!"));
  mutex.lock();
  AntiquaCRM::ASqlSettings sqlConfig(this);
  QString host = sqlConfig.getParam("pg_hostname").toString();
  int port = sqlConfig.getParam("pg_port").toInt();
  if (!iface.checkSqlPort(host, port)) {
    m_splash->setMessage(tr("Sql Server unreachable!"));
    qWarning("Sql Server unreachable!");
    mutex.unlock();
    return 1;
  }
  m_splash->setMessage(tr("Sql Server found!"));
  mutex.unlock();

  m_splash->setMessage(tr("Test Database connection!"));
  mutex.lock();
  m_sql = new AntiquaCRM::ASqlCore(this);
  if (!m_sql->open()) {
    qWarning("No Database connected!");
    mutex.unlock();
    return 1;
  }
  m_splash->setMessage(tr("Database connected!"));
  mutex.unlock();

  // TODO Templates and Pluginloader
  // m_cfg->getDataDir();
  // m_cfg->getPluginDir();
  // m_cfg->getTempDir();

  m_splash->finish(m_mainWindow);

  m_splash->setMessage("Open Antiqua CRM");
  m_mainWindow->openWindow();

  m_splash->setMessage("Add Systemtray Icon");
  m_systemTray->show();

  return QCoreApplication::exec();
}

AntiquaAppl::~AntiquaAppl() {
  if (m_splash != nullptr)
    m_splash->deleteLater();

  if (m_sql != nullptr)
    m_sql->close();
}
