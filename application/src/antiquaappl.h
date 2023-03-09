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

/**
 * @defgroup ui User Application
 * @brief AntiquaCRM Interface
 *
 * @class AntiquaAppl
 * @brief Main Application Class
 */
class AntiquaAppl final : public QApplication {
  Q_OBJECT

private:
  /**
   * @brief global configuration
   */
  AntiquaCRM::ASettings *m_cfg;

  /**
   * @brief main window
   */
  AntiquaWindow *m_mainWindow;

  /**
   * @brief system tray icon
   */
  AntiquaSystemTray *m_systemTray;

  /**
   * @brief PostgreSQL database connection
   */
  AntiquaCRM::ASqlCore *m_sql;

#ifdef ANTIQUACRM_DBUS_ENABLED
  /**
   * @brief D-Bus Connection
   */
  QDBusConnection *m_dbus;
#endif

  /**
   * @brief Search Qt based translation files „*.qm“ and load it.
   */
  bool initTranslations();

  /**
   * @brief Load and initial - D-Bus, Window and System Tray.
   */
  void initGui();

  /**
   * @brief Network Interfaces and connection check.
   */
  bool checkInterfaces();

  /**
   * @brief Database remote port and status check
   */
  bool checkRemotePort();

  /**
   * @brief Connect to Database
   */
  bool checkDatabase();

Q_SIGNALS:
  /**
   * @brief Internal Message Signal for subclasses
   */
  void sendStatusMessage(const QString &);

public Q_SLOTS:
  /**
   * @brief Request to shutdown the application
   */
  void applicationQuit();

public:
  explicit AntiquaAppl(int &argc, char **argv);

  /**
   * @brief default application icon
   */
  static const QIcon applIcon();

  /**
   * @brief Initial Stylesheet
   * Loading Default Window Stylesheet theme and make some changes.
   */
  void initDefaultTheme();

  /**
   * @brief Check if Application is already up and running
   */
  bool isRunning();

  /**
   * @brief Default execution method to start AntiquaCRM.
   */
  int exec();
};

#endif // ANTIQUACRM_APPLICATION_H
