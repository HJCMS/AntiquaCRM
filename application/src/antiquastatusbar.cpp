// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquastatusbar.h"
#include "antiquadatabasebar.h"
#include "antiquatimer.h"

#include <AntiquaCRM>

AntiquaStatusBar::AntiquaStatusBar(QMainWindow *parent) : QStatusBar{parent} {
  setObjectName("antiqua_status_bar");
  setContentsMargins(0, 0, 0, 0);
  setStyleSheet("* {margin:0;}");
  m_toolBar = new AntiquaDatabaseBar(this);
  addPermanentWidget(m_toolBar);

  m_timer = new AntiquaTimer(this);
  connect(m_timer, SIGNAL(sendTrigger()), SLOT(timerTriggered()));
  m_timer->restart();
}

void AntiquaStatusBar::timerTriggered() {
  AntiquaCRM::ASqlSettings cfg(this);
  QSqlDatabase db = QSqlDatabase::database(cfg.connectionName());
  if (db.isOpen() && !db.isOpenError()) {
    m_toolBar->setStatus(true);
  } else {
    m_toolBar->setStatus(false);
    statusWarnMessage(tr("Missing database connection!"));
  }
}

void AntiquaStatusBar::statusInfoMessage(const QString &text) {
  showMessage(text, (timeout_seconds * 1000));
}

void AntiquaStatusBar::statusWarnMessage(const QString &text) {
  showMessage(text, (timeout_seconds * 1000));
}

AntiquaStatusBar::~AntiquaStatusBar() {
  if (m_timer != nullptr) {
    qInfo("Shutdown Database Listener ...");
    m_timer->deleteLater();
  }
}
