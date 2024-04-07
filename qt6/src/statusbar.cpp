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

void StatusCheck::prepareSignal(StatusCheck::ExitCode code) {
  emit signalFinished(code);
  exit(code);
}

void StatusCheck::run() {
  AntiquaCRM::ANetworkIface iface;
  if (!iface.connectedIfaceExists()) {
    prepareSignal(StatusCheck::ExitCode::NETWORK_ERROR);
    return;
  }

  AntiquaCRM::ASqlSettings _cfg;
  // NOTE - do not open database connections in this function!
  if (!QSqlDatabase::contains(_cfg.connectionName())) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Missing connectionName:" << _cfg.connectionName();
#endif
    prepareSignal(StatusCheck::ExitCode::CONFIG_ERROR);
    return;
  } else if (_cfg.getProfile().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Missing connectionProfile:" << _cfg.getProfile();
#endif
    prepareSignal(StatusCheck::ExitCode::CONFIG_ERROR);
  }

  AntiquaCRM::ASqlProfile _profile = _cfg.connectionProfile();
  if (iface.checkRemotePort(_profile.getHostname(), _profile.getPort(), PROCESS_TIMEOUT)) {
    prepareSignal(StatusCheck::ExitCode::CONNECTED);
  } else {
    prepareSignal(StatusCheck::ExitCode::NETWORK_ERROR);
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
  ac_status = addAction(tr("Database Status"));
  ac_status->setIcon(AntiquaCRM::antiquaIcon("database-status"));
  connect(ac_status, SIGNAL(triggered()), SLOT(databaseInfoDialog()));
}

void StatusToolBar::databaseInfoDialog() {
  AntiquaCRM::SqlInfoPopUp infoPopUp(this);
  infoPopUp.exec();
}

void StatusToolBar::setStatus(StatusCheck::ExitCode ec) {
  switch (ec) {
    case (StatusCheck::ExitCode::CONNECTED):
      {
        ac_status->setIcon(AntiquaCRM::antiquaIcon("database-comit"));
        ac_status->setToolTip(tr("Database connected."));
      }
      break;

    case (StatusCheck::ExitCode::NETWORK_ERROR):
      {
        const QString info(tr("Remote connection is not reachable!"));
        ac_status->setIcon(AntiquaCRM::antiquaIcon("database-remove"));
        ac_status->setToolTip(info);
        emit signalErrorMessage(info);
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
    m_toolBar->setStatus(StatusCheck::ExitCode::CONNECTED);

  m_timer->restart();
}

void StatusBar::openErrorPopUp(const QSqlError& error) {
  m_toolBar->setStatus(StatusCheck::ExitCode::DATABASE_ERROR);
  QMessageBox::warning(this, tr("Conenction Errors"), error.text());
}

void StatusBar::startTest() {
  if (!m_sql->status()) {
    statusFatalMessage(tr("No Remote Connection!"));
    m_toolBar->setStatus(StatusCheck::ExitCode::DATABASE_ERROR);
    return;
  }

  StatusCheck* m_check = new StatusCheck(this);
  connect(m_check, SIGNAL(signalFinished(StatusCheck::ExitCode)), m_toolBar,
          SLOT(setStatus(StatusCheck::ExitCode)));
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
