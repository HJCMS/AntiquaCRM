// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_UI_STATUSBAR_H
#define ANTIQUA_UI_STATUSBAR_H

#include <QObject>
#include <QStatusBar>
#include <QTimer>
#include <QTimerEvent>
#include <QToolBar>
#include <QWidget>

/**
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

  void timerEvent(QTimerEvent *event) override;

Q_SIGNALS:
  void sendTrigger();

public Q_SLOTS:
  void restart();

public:
  explicit StatusTimer(QObject *parent = nullptr);
  virtual ~StatusTimer();
};

/**
 * @brief The StatusToolBar class
 */
class StatusToolBar final : public QToolBar {
  Q_OBJECT

private:
  QAction *ac_status;

private Q_SLOTS:
  void databaseInfoDialog();

public Q_SLOTS:
  void setStatus(bool);

public:
  explicit StatusToolBar(QWidget *parent = nullptr);
};

/**
 * @brief The StatusBar class
 */
class StatusBar final : public QStatusBar {
  Q_OBJECT

private:
  int timeout_seconds = 10;
  StatusToolBar *m_toolBar;
  StatusTimer *m_timer;

private Q_SLOTS:
  void timerTriggered();

public Q_SLOTS:
  void statusInfoMessage(const QString &);
  void statusWarnMessage(const QString &);

public:
  explicit StatusBar(QWidget *parent = nullptr);
  virtual ~StatusBar();
};

#endif // ANTIQUA_UI_STATUSBAR_H
