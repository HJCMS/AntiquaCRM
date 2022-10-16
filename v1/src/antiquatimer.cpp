// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquatimer.h"
#include "asettings.h"

#include <QDebug>

AntiquaTimer::AntiquaTimer(QObject *parent) : QObject{parent} {
  setObjectName("antiquacrm_timer");
  AntiquaCRM::ASettings cfg(this);
  interval = (cfg.value("timeout",1).toInt() * 1000);
  countBase = cfg.value("counter", 60).toInt();
  countDown = countBase;
}

void AntiquaTimer::timerEvent(QTimerEvent *event) {
  --countDown;
  if (countDown <= 0) {
    qDebug() << Q_FUNC_INFO << countDown;
    countDown = countBase;
    emit sendTrigger();
    return;
  }
  QTime t(0, 0, 0);
  emit sendStatus(t.addSecs(countDown));
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
