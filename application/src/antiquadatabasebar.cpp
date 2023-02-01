// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquadatabasebar.h"
#include "antiquadbinfo.h"

#include <QIcon>

AntiquaDatabaseBar::AntiquaDatabaseBar(QWidget *parent) : QToolBar{parent} {
  setFloatable(false);
  setMovable(false);
  setOrientation(Qt::Horizontal);
  setToolButtonStyle(Qt::ToolButtonIconOnly);
  ac_status = addAction(tr("Database Status"));
  ac_status->setIcon(QIcon("://icons/database.png"));
  connect(ac_status, SIGNAL(triggered()), SLOT(databaseInfoDialog()));
}

void AntiquaDatabaseBar::databaseInfoDialog() {
  AntiquaDbInfo infoPopUp(this);
  infoPopUp.exec();
}

void AntiquaDatabaseBar::setStatus(bool b) {
  if (b) {
    ac_status->setIcon(QIcon("://icons/db_comit.png"));
    ac_status->setToolTip(tr("Database connected."));
  } else {
    ac_status->setIcon(QIcon("://icons/db_status.png"));
    ac_status->setToolTip(tr("Database not connected!"));
  }
}
