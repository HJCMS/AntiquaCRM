// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statusbar.h"

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QDebug>
#include <QIcon>
#include <QMessageBox>
#include <QSqlDatabase>

inline const double PROCESS_TIMEOUT = 6;

// BEGIN:StatusCheck
StatusCheck::StatusCheck(QObject* parent) : QThread{parent} {
}

void StatusCheck::prepareSignal(StatusCheck::Status s) {
  emit signalFinished(s);
  exit(s);
}

void StatusCheck::run() {
  AntiquaCRM::ANetworkIface iface;
  if (!iface.connectedIfaceExists()) {
    prepareSignal(StatusCheck::Status::NETWORK_DISCONNECTED);
    return;
  }

  AntiquaCRM::ASqlSettings _cfg;
  // NOTE - do not open database connections in this function!
  if (!QSqlDatabase::contains(_cfg.connectionName())) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Missing connectionName:" << _cfg.connectionName();
#endif
    prepareSignal(StatusCheck::Status::NETWORK_CONFIG_ERROR);
    return;
  } else if (_cfg.getProfile().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Missing connectionProfile:" << _cfg.getProfile();
#endif
    prepareSignal(StatusCheck::Status::NETWORK_DISCONNECTED);
  }

  AntiquaCRM::ASqlProfile _profile = _cfg.connectionProfile();
  if (iface.checkRemotePort(_profile.getHostname(), _profile.getPort(), PROCESS_TIMEOUT)) {
    prepareSignal(StatusCheck::Status::NETWORK_CONNECTED);
  } else {
    prepareSignal(StatusCheck::Status::NETWORK_DISCONNECTED);
  }
}
// END:StatusCheck

// BEGIN:StatusTimer
StatusTimer::StatusTimer(QObject* parent) : QObject{parent} {
  setObjectName("antiquacrm_timer");
#ifdef ANTIQUA_DEVELOPEMENT
  // Standard ist 60 Sekunden
  countBase = 30;
#endif
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
  ac_status = addAction(tr("Network status"));
  ac_status->setIcon(AntiquaCRM::antiquaIcon("network-connect"));
  connect(ac_status, SIGNAL(triggered()), SLOT(databaseInfoDialog()));
}

void StatusToolBar::databaseInfoDialog() {
  if (!ac_enabled) {
    emit signalErrorMessage(tr("Connection is not available!"));
    return;
  }
  AntiquaCRM::SqlInfoPopUp infoPopUp(this);
  infoPopUp.exec();
}

void StatusToolBar::setStatus(StatusCheck::Status s) {
  switch (s) {
    case (StatusCheck::Status::NETWORK_CONNECTED):
      {
        ac_status->setIcon(AntiquaCRM::antiquaIcon("network-connect"));
        ac_status->setToolTip(tr("Network connected."));
        ac_enabled = true;
      }
      break;

    case (StatusCheck::Status::NETWORK_DISCONNECTED):
      {
        const QString info(tr("Remote connection is not reachable!"));
        ac_status->setIcon(AntiquaCRM::antiquaIcon("network-disconnect"));
        ac_status->setToolTip(info);
        ac_enabled = false;
        emit signalErrorMessage(info);
      }
      break;

    default:
      {
        ac_status->setIcon(AntiquaCRM::antiquaIcon("network-disconnect"));
        ac_status->setToolTip(tr("No network connection!"));
        ac_enabled = false;
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

  m_sql = new AntiquaCRM::ASqlCore(this);

  m_toolBar = new StatusToolBar(this);
  addPermanentWidget(m_toolBar);

  m_timer = new StatusTimer(this);

  // SIGNALS
  connect(m_toolBar, SIGNAL(signalErrorMessage(const QString&)), this,
          SLOT(statusFatalMessage(const QString&)));

  connect(m_sql, SIGNAL(sendStatementError(const QSqlError&)),
          SLOT(openErrorPopUp(const QSqlError&)));

  connect(m_timer, SIGNAL(sendTrigger()), SLOT(startTest()));

  if (m_sql->status())
    m_toolBar->setStatus(StatusCheck::Status::NETWORK_CONNECTED);

  m_timer->restart();
}

void StatusBar::openErrorPopUp(const QSqlError& error) {
  m_toolBar->setStatus(StatusCheck::Status::NETWORK_DISCONNECTED);
  QMessageBox::warning(this, tr("Conenction Errors"), error.text());
}

void StatusBar::startTest() {
  if (!m_sql->status()) {
    statusFatalMessage(tr("No Remote Connection!"));
    m_toolBar->setStatus(StatusCheck::Status::NETWORK_DISCONNECTED);
    return;
  }

  StatusCheck* m_check = new StatusCheck(this);
  connect(m_check, SIGNAL(signalFinished(StatusCheck::Status)), m_toolBar,
          SLOT(setStatus(StatusCheck::Status)));
  connect(m_check, SIGNAL(finished()), m_check, SLOT(deleteLater()));

  m_check->wait(qRound(PROCESS_TIMEOUT / 2) * 1000);
  m_check->start();
}

void StatusBar::statusInfoMessage(const QString& text) {
  showMessage(text, (PROCESS_TIMEOUT * 1000));
}

void StatusBar::statusWarnMessage(const QString& text) {
  showMessage(text, (PROCESS_TIMEOUT * 1000));
}

void StatusBar::statusFatalMessage(const QString& text) {
  showMessage(text);
}

StatusBar::~StatusBar() {
  if (m_timer != nullptr) {
    qInfo("Shutdown Database Listener ...");
    m_timer->deleteLater();
  }
}
// END:StatusBar
