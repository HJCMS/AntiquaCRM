// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_UI_STATUSBAR_H
#define ANTIQUA_UI_STATUSBAR_H

#include <AntiquaCRM>
#include <QObject>
#include <QStatusBar>
#include <QThread>
#include <QTimer>
#include <QTimerEvent>
#include <QToolBar>
#include <QWidget>

/**
 * @class StatusCheck
 * @brief Start Connection tests
 *
 */
class StatusCheck final : public QThread {
  Q_OBJECT

public:
  double timeout = 6.00;
  enum Status { NETWORK_CONNECTED = 0, NETWORK_DISCONNECTED = 1, NETWORK_CONFIG_ERROR = 2 };
  explicit StatusCheck(QObject* parent = nullptr);

Q_SIGNALS:
  void signalFinished(StatusCheck::Status);

private:
  void prepareSignal(StatusCheck::Status);
  void run() override;
};

/**
 * @class StatusTimer
 * @brief The StatusTimer class
 */
class StatusTimer final : public QObject {
  Q_OBJECT

private:
  /**
   * @brief Timer Event Id
   */
  int timerId = -1;

  /**
   * @brief Timer Interval in msecs
   */
  int interval = 1000;

  /**
   * @brief Counter base in seconds
   */
  int countBase = 60;

  /**
   * @brief Counter
   */
  int countDown;

  void timerEvent(QTimerEvent* event) override;

Q_SIGNALS:
  void sendTrigger();

public Q_SLOTS:
  void restart();

public:
  explicit StatusTimer(QObject* parent = nullptr);
  virtual ~StatusTimer();
};

/**
 * @class StatusToolBar
 * @brief The StatusToolBar class
 */
class StatusToolBar final : public QToolBar {
  Q_OBJECT

private:
  bool ac_enabled = false;
  QAction* ac_status;

private Q_SLOTS:
  void databaseInfoDialog();

Q_SIGNALS:
  void signalErrorMessage(const QString&);

public Q_SLOTS:
  void setStatus(StatusCheck::Status s);

public:
  explicit StatusToolBar(QWidget* parent = nullptr);
};

/**
 * @class StatusBar
 * @brief The StatusBar class
 */
class StatusBar final : public QStatusBar {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore* m_sql = nullptr;
  StatusToolBar* m_toolBar;
  StatusTimer* m_timer;

private Q_SLOTS:
  void openErrorPopUp(const QSqlError&);
  void startTest();

public Q_SLOTS:
  void statusInfoMessage(const QString&, int timeout = 15);
  void statusWarnMessage(const QString&, int timeout = 30);
  void statusFatalMessage(const QString&);

public:
  explicit StatusBar(QWidget* parent = nullptr);
  virtual ~StatusBar();
};

#endif // ANTIQUA_UI_STATUSBAR_H
