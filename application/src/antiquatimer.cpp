// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquatimer.h"
#include "asettings.h"

#include <QDebug>

AntiquaTimer::AntiquaTimer(QObject *parent) : QObject{parent} {
  setObjectName("antiquacrm_timer");
  AntiquaCRM::ASettings cfg(this);
  cfg.beginGroup("timer/dblistener");
  interval = (cfg.value("interval", 1).toInt() * 1000);
  countBase = cfg.value("counter", 60).toInt();
  cfg.endGroup();
  countDown = countBase;
}

void AntiquaTimer::timerEvent(QTimerEvent *event) {
  // @note prevent multible global TimerIds
  if (event->timerId() != timerId)
    return;

  --countDown;
  if (countDown > 0)
    return;

  countDown = countBase;
  emit sendTrigger();
}

void AntiquaTimer::restart() {
  if (timerId != -1)
    killTimer(timerId);

  countDown = countBase;
  timerId = startTimer(interval, Qt::PreciseTimer);
}

AntiquaTimer::~AntiquaTimer() {
  if (timerId != -1)
    killTimer(timerId);
}
