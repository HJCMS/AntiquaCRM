// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statusbar.h"

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QIcon>
#include <QMessageBox>

// BEGIN:StatusCheck
StatusCheck::StatusCheck(QObject* parent) : QThread{parent} {
  setObjectName("NetworkStatusCheck");
}

void StatusCheck::prepareSignal(StatusCheck::Status s) {
  emit signalFinished(s);
  exit(s);
}

void StatusCheck::run() {
  AntiquaCRM::ASqlSettings _config;
  // Check current database connection
  if (!QSqlDatabase::contains(_config.connectionName())) {
    prepareSignal(StatusCheck::Status::NETWORK_CONFIG_ERROR);
    return;
  }
  // Check current database profile
  if (_config.getProfile().isEmpty()) {
    prepareSignal(StatusCheck::Status::NETWORK_DISCONNECTED);
    return;
  }

  AntiquaCRM::ANetworkIface _netrc;
  AntiquaCRM::ASqlProfile _profile = _config.connectionProfile();
  if (!_netrc.isLoopbackInterface(_profile.getHostname()) && !_netrc.connectedIfaceExists()) {
    prepareSignal(StatusCheck::Status::NETWORK_DISCONNECTED);
    return;
  }

  if (_netrc.checkRemotePort(_profile.getHostname(), // remote host
                             _profile.getPort(),     // remote port
                             timeout)) {
    prepareSignal(StatusCheck::Status::NETWORK_CONNECTED);
    return;
  }

  prepareSignal(StatusCheck::Status::NETWORK_DISCONNECTED);
}
// END:StatusCheck

// BEGIN:StatusTimer
StatusTimer::StatusTimer(QObject* parent) : QObject{parent} {
  setObjectName("antiquacrm_timer");
#ifdef ANTIQUA_DEVELOPEMENT
  countBase = 30; // Default is 60 secs
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
  ac_status->setIcon(AntiquaCRM::antiquaIcon("network-disconnect"));
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
  QMessageBox::warning(this, tr("Connection Errors"), error.text());
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

  m_check->wait(qRound(m_check->timeout / 2) * 1000);
  m_check->start();
}

void StatusBar::statusInfoMessage(const QString& text, int timeout) {
  showMessage(text, (timeout * 1000));
}

void StatusBar::statusWarnMessage(const QString& text, int timeout) {
  showMessage(text, (timeout * 1000));
}

void StatusBar::statusFatalMessage(const QString& text) {
  showMessage(text);
}

StatusBar::~StatusBar() {
  if (m_timer != nullptr) {
    qInfo("Shutdown Connections listener ...");
    m_timer->deleteLater();
  }
}
// END:StatusBar
