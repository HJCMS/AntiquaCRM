// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquastatusbar.h"
#include "antiquatimer.h"

#include <QDebug>
#include <QIcon>
#include <QLayout>

AntiquaDatabaseBar::AntiquaDatabaseBar(QWidget *parent) : QToolBar{parent} {
  setFloatable(false);
  setMovable(false);
  setOrientation(Qt::Horizontal);
  setToolButtonStyle(Qt::ToolButtonIconOnly);
  ac_status = addAction(tr("Database Status"));
  ac_status->setIcon(QIcon("://icons/database.png"));
  // TODO HOVER ->setCursor(Qt::PointingHandCursor);
}

void AntiquaDatabaseBar::setStatus(AntiquaCRM::ASqlCore::Status status) {
  switch (status) {
  case AntiquaCRM::ASqlCore::Status::CLOSED:
    ac_status->setIcon(QIcon("://icons/db_status.png"));
    ac_status->setToolTip(tr("Database not connected!"));
    break;

  case AntiquaCRM::ASqlCore::Status::OPEN:
    ac_status->setIcon(QIcon("://icons/db_comit.png"));
    ac_status->setToolTip(tr("Database connected."));
    break;

  default:
    ac_status->setIcon(QIcon("://icons/db_status.png"));
    ac_status->setToolTip(tr("Invalid database status!"));
    break;
  }
}

AntiquaStatusBar::AntiquaStatusBar(QMainWindow *parent) : QStatusBar{parent} {
  setObjectName("antiqua_status_bar");
  setContentsMargins(0, 0, 0, 0);
  setStyleSheet("* {margin:0;}");
  m_toolBar = new AntiquaDatabaseBar(this);
  addPermanentWidget(m_toolBar);
  // Im Konstruktor keine Datenbankverbindung aufmachen!
  m_sql = nullptr;

  m_timer = new AntiquaTimer(this);
  connect(m_timer, SIGNAL(sendTrigger()), SLOT(timerTriggered()));
  m_timer->restart();
}

void AntiquaStatusBar::timerTriggered() {
  if (m_sql == nullptr)
    m_sql = new AntiquaCRM::ASqlCore(this);

  m_toolBar->setStatus(m_sql->status());
}

void AntiquaStatusBar::statusInfoMessage(const QString &text) {
  showMessage(text, (timeout_seconds * 1000));
}

void AntiquaStatusBar::statusWarnMessage(const QString &text) {
  showMessage(text, (timeout_seconds * 1000));
}

AntiquaStatusBar::~AntiquaStatusBar() {
  if (m_timer != nullptr) {
    qInfo("Shutdown Timer ...");
    m_timer->deleteLater();
  }
}
