// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statusbar.h"

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QDebug>
#include <QIcon>
#include <QSqlDatabase>

// BEGIN:StatusTimer
StatusTimer::StatusTimer(QObject* parent) : QObject{parent} {
  setObjectName("antiquacrm_timer");
  countDown = countBase;
}

void StatusTimer::timerEvent(QTimerEvent* event) {
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
StatusToolBar::StatusToolBar(QWidget* parent) : QToolBar{parent} {
  setFloatable(false);
  setMovable(false);
  setOrientation(Qt::Horizontal);
  setToolButtonStyle(Qt::ToolButtonIconOnly);
  ac_status = addAction(tr("Database Status"));
  ac_status->setIcon(AntiquaCRM::antiquaIcon("antiquacrm"));
  connect(ac_status, SIGNAL(triggered()), SLOT(databaseInfoDialog()));
}

void StatusToolBar::databaseInfoDialog() {
  AntiquaCRM::SqlInfoPopUp infoPopUp(this);
  infoPopUp.exec();
}

void StatusToolBar::setStatus(StatusToolBar::Status st) {
  switch (st) {
    case (Status::CONNECTED):
      {
        ac_status->setIcon(AntiquaCRM::antiquaIcon("database-comit"));
        ac_status->setToolTip(tr("Database connected."));
      }
      break;

    case (Status::NETWORK_ERROR):
      {
        ac_status->setIcon(AntiquaCRM::antiquaIcon("database-status"));
        ac_status->setToolTip(tr("Remote connection is not reachable!"));
      }
      break;

    default:
      {
        ac_status->setIcon(AntiquaCRM::antiquaIcon("database-status"));
        ac_status->setToolTip(tr("Database not connected!"));
      }
      break;
  };
}
// END:StatusToolBar

// BEGIN:StatusBar
StatusBar::StatusBar(QWidget* parent) : QStatusBar{parent} {
  setObjectName("antiqua_ui_statusbar");
  setContentsMargins(0, 0, 0, 0);
  setStyleSheet("* {margin:0;}");
  m_toolBar = new StatusToolBar(this);
  addPermanentWidget(m_toolBar);

  m_timer = new StatusTimer(this);
  connect(m_timer, SIGNAL(sendTrigger()), SLOT(startTest()));
  m_timer->restart();
}

void StatusBar::startTest() {
  AntiquaCRM::ANetworkIface iface;
  if (!iface.connectedIfaceExists()) {
    m_toolBar->setStatus(StatusToolBar::Status::NETWORK_ERROR);
    return;
  }

  AntiquaCRM::ASqlCore _sql(this);
  if (iface.checkRemotePort(_sql.db().hostName(), _sql.db().port())) {
    if (_sql.status()) {
      m_toolBar->setStatus(StatusToolBar::Status::CONNECTED);
    } else {
      m_toolBar->setStatus(StatusToolBar::Status::DATABASE_ERROR);
      statusWarnMessage(tr("Missing database connection!"));
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << _sql.lastError();
#endif
    }
  } else {
    m_toolBar->setStatus(StatusToolBar::Status::CONNECTION_ERROR);
    statusWarnMessage(tr("Remote port is unreachable!"));
  }
}

void StatusBar::statusInfoMessage(const QString& text) {
  showMessage(text, (timeout_seconds * 1000));
}

void StatusBar::statusWarnMessage(const QString& text) {
  showMessage(text, (timeout_seconds * 1000));
}

StatusBar::~StatusBar() {
  if (m_timer != nullptr) {
    qInfo("Shutdown Database Listener ...");
    m_timer->deleteLater();
  }
}
// END:StatusBar
