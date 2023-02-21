// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_APPLICATION_H
#define ANTIQUACRM_APPLICATION_H

#include <AntiquaCRM>
#include <QApplication>
#include <QIcon>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QScreen>
#include <QStyle>
#ifdef ANTIQUACRM_DBUS_ENABLED
#include <QtDBus/QDBusConnection>
#endif

class AntiquaWindow;
class AntiquaSystemTray;
class AntiquaTimer;

class AntiquaAppl final : public QApplication {
  Q_OBJECT
#ifdef ANTIQUACRM_DBUS_ENABLED
  Q_CLASSINFO("D-Bus Interface", ANTIQUACRM_CONNECTION_DOMAIN)
#endif

private:
  AntiquaCRM::ASettings *m_cfg;
  AntiquaWindow *m_mainWindow;
  AntiquaSystemTray *m_systemTray;
  AntiquaCRM::ASqlCore *m_sql;
#ifdef ANTIQUACRM_DBUS_ENABLED
  QDBusConnection *m_dbus;
#endif

  QList<AntiquaCRM::APluginInterface *> p_interfaces;

  static const QIcon applIcon();

  void initGui();

  bool checkInterfaces();
  bool checkRemotePort();
  bool checkDatabase();
  bool initTranslations();

Q_SIGNALS:
  void sendStatusMessage(const QString &msg);

public Q_SLOTS:
  void applicationQuit();

public:
  explicit AntiquaAppl(int &argc, char **argv);
  void initDefaultTheme();
  bool isRunning();
  int exec();
};

#endif // ANTIQUACRM_APPLICATION_H
