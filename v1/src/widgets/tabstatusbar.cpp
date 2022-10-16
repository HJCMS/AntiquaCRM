// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabstatusbar.h"

#include <QDebug>

TabStatusBar::TabStatusBar(QWidget *parent) : QStatusBar{parent} {
  setSizeGripEnabled(false);
  setContentsMargins(0, 0, 4, 5);

  m_frame = new QFrame(this);
  m_frame->setContentsMargins(0, 0, 0, 0);
  insertPermanentWidget(0, m_frame, 0);

  btn_refresh = new QPushButton(this);
  btn_refresh->setIcon(getIcon("action_reload"));
  btn_refresh->setText(tr("Update"));
  btn_refresh->setToolTip(tr("Update current View"));
  insertPermanentWidget(1, btn_refresh, 0);

  connect(btn_refresh, SIGNAL(clicked()), this, SIGNAL(sendReloadView()));
}

const QIcon TabStatusBar::getIcon(const QString &name) const {
  return QIcon(":icons/" + name + ".png");
}
