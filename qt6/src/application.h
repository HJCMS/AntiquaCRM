// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_UI_APPLICATION_H
#define ANTIQUA_UI_APPLICATION_H

#include <AntiquaCRM>
#include <QApplication>
#include <QIcon>
#include <QObject>

class MainWindow;
class SystemTrayIcon;

class Application final : public QApplication {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *m_cfg;          /**< @brief Configurations */
  AntiquaCRM::ASqlCore *m_sql = nullptr; /**< @brief PostgreSQL Database */
  MainWindow *m_window = nullptr;        /**< @brief UI Window */
  SystemTrayIcon *m_systray = nullptr;   /**< @brief UI SystemTray */

  /**
   * @brief Search Qt based translation files „*.qm“ and load it.
   */
  bool initTranslations();

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
  bool openDatabase();

  /**
   * @brief Load and initial components.
   */
  void initGui();

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
  explicit Application(int &argc, char **argv);

  /**
   * @brief default application icon
   */
  static const QIcon applIcon();

  /**
   * @brief Initial Stylesheet
   * Loading Default Window Stylesheet theme and make some changes.
   */
  void initTheme();

  /**
   * @brief Check if Application is already up and running
   */
  bool isRunning();

  /**
   * @brief Default execution method to start AntiquaCRM.
   */
  int exec();
};

#endif // ANTIQUA_UI_APPLICATION_H
