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
  mutex.lock();
  m_sql = new AntiquaCRM::ASqlCore(this);
  qInfo("Open Database connection");
  if (!m_sql->open()) {
    qFatal("no database connection");
    mutex.unlock();
    return 134;
  }
  m_splash->setMessage(tr("Database connected"));
  mutex.unlock();

  // TODO Templates and Pluginloader
  // m_cfg->getDataTarget();
  // m_cfg->getPluginTarget();
  qDebug() << m_cfg->getLocalTempDir();

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
