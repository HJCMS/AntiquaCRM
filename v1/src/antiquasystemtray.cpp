// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquasystemtray.h"

#include <QMenu>

AntiquaSystemTray::AntiquaSystemTray(const QIcon &icon, QObject *parent)
    : QSystemTrayIcon{icon, parent} {
  setObjectName("antiquacrm_systemtray");
  setToolTip(tr("Database connected."));

  QMenu *m_menu = new QMenu();
  ac_show = m_menu->addAction(getIcon("window_fullscreen"), tr("Window show"));
  ac_hide =
      m_menu->addAction(getIcon("window_nofullscreen"), tr("Window hide"));
  m_menu->addSeparator();
  ac_quit = m_menu->addAction(getIcon("action_quit"), tr("Application Quit"));
  setContextMenu(m_menu);

  connect(ac_show, SIGNAL(triggered()), this, SIGNAL(sendShowWindow()));
  connect(ac_hide, SIGNAL(triggered()), this, SIGNAL(sendHideWindow()));
  connect(ac_quit, SIGNAL(triggered()), this, SIGNAL(sendApplQuit()));
  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
          SLOT(actionReason(QSystemTrayIcon::ActivationReason)));
}

const QIcon AntiquaSystemTray::getIcon(const QString &name) const {
  QIcon icon(QString(":icons/" + name + ".png"));
  return icon;
}

void AntiquaSystemTray::actionReason(QSystemTrayIcon::ActivationReason r) {
  if (r == QSystemTrayIcon::Context)
    return;

  emit sendToggleView();
}

void AntiquaSystemTray::setConnectionStatus(bool b) {
  if (b) {
    setToolTip(tr("Database connected."));
    setIcon(getIcon("database"));
  } else {
    setToolTip(tr("No Database conenction!"));
    setIcon(getIcon("db_status"));
  }
}

void AntiquaSystemTray::setOrdersMessage(const QString &body) {
  QString title = tr("Incoming Orders");
  QIcon icon("://icons/filetypes.png");
  showMessage(title, body, icon, (5 * 1000));
}
