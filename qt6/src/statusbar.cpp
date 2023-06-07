// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statusbar.h"

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QDebug>
#include <QIcon>
#include <QSqlDatabase>

// BEGIN:StatusTimer
StatusTimer::StatusTimer(QObject *parent) : QObject{parent} {
  setObjectName("antiquacrm_timer");
  countDown = countBase;
}

void StatusTimer::timerEvent(QTimerEvent *event) {
  // @note prevent multible global TimerIds
  if (event->timerId() != timerId)
    return;

  --countDown;
  if (countDown > 0)
    return;

  countDown = countBase;
  emit sendTrigger();
}

void StatusTimer::restart() {
  if (timerId != -1)
    killTimer(timerId);

  countDown = countBase;
  timerId = startTimer(interval, Qt::PreciseTimer);
}

StatusTimer::~StatusTimer() {
  if (timerId != -1)
    killTimer(timerId);
}
// END:StatusTimer

// BEGIN:StatusToolBar
StatusToolBar::StatusToolBar(QWidget *parent) : QToolBar{parent} {
  setFloatable(false);
  setMovable(false);
  setOrientation(Qt::Horizontal);
  setToolButtonStyle(Qt::ToolButtonIconOnly);
  ac_status = addAction(tr("Database Status"));
  ac_status->setIcon(AntiquaCRM::AntiquaApplIcon("antiquacrm"));
  connect(ac_status, SIGNAL(triggered()), SLOT(databaseInfoDialog()));
}

void StatusToolBar::databaseInfoDialog() {
  AntiquaCRM::SqlInfoPopUp infoPopUp(this);
  infoPopUp.exec();
}

void StatusToolBar::setStatus(bool b) {
  if (b) {
    ac_status->setIcon(AntiquaCRM::AntiquaApplIcon("database-comit"));
    ac_status->setToolTip(tr("Database connected."));
  } else {
    ac_status->setIcon(AntiquaCRM::AntiquaApplIcon("database-status"));
    ac_status->setToolTip(tr("Database not connected!"));
  }
}
// END:StatusToolBar

// BEGIN:StatusBar
StatusBar::StatusBar(QWidget *parent) : QStatusBar{parent} {
  setObjectName("antiqua_ui_statusbar");
  setContentsMargins(0, 0, 0, 0);
  setStyleSheet("* {margin:0;}");
  m_toolBar = new StatusToolBar(this);
  addPermanentWidget(m_toolBar);

  m_timer = new StatusTimer(this);
  connect(m_timer, SIGNAL(sendTrigger()), SLOT(timerTriggered()));
  m_timer->restart();
}

void StatusBar::timerTriggered() {
  AntiquaCRM::ASqlCore _sql(this);
  if (_sql.status()) {
    m_toolBar->setStatus(true);
  } else {
    m_toolBar->setStatus(false);
    statusWarnMessage(tr("Missing database connection!"));
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << _sql.lastError();
#endif
  }
}

void StatusBar::statusInfoMessage(const QString &text) {
  showMessage(text, (timeout_seconds * 1000));
}

void StatusBar::statusWarnMessage(const QString &text) {
  showMessage(text, (timeout_seconds * 1000));
}

StatusBar::~StatusBar() {
  if (m_timer != nullptr) {
    qInfo("Shutdown Database Listener ...");
    m_timer->deleteLater();
  }
}
// END:StatusBar
