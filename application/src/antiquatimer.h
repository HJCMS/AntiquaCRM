// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TIMER_H
#define ANTIQUACRM_TIMER_H

#include <QObject>
#include <QTime>
#include <QTimerEvent>

/**
 * @brief Main Timer class for operations
 */
class AntiquaTimer final : public QObject {
  Q_OBJECT

private:
  /**
   * @brief Timer Event Id
   */
  int timerId = -1;

  /**
   * @brief Timer Interval in msecs
   */
  int interval;

  /**
   * @brief Counter base in secs
   */
  int countBase;

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
  explicit AntiquaTimer(QObject *parent = nullptr);
  virtual ~AntiquaTimer();
};

#endif // ANTIQUACRM_TIMER_H
