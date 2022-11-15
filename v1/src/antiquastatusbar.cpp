// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquastatusbar.h"

#include <QDebug>
#include <QIcon>

AntiquaStatusBar::AntiquaStatusBar(QMainWindow *parent) : QStatusBar{parent} {
  setObjectName("antiqua_status_bar");
  m_dbStatus = new QToolButton(this);
  m_dbStatus->setIcon(QIcon("://icons/database.png"));
  addPermanentWidget(m_dbStatus);
}
