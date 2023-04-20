// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "systemtrayicon.h"
#include "abstractinput.h"

#include <QMenu>

SystemTrayIcon::SystemTrayIcon(const QIcon &icon, QObject *parent)
    : QSystemTrayIcon{icon, parent} {
  setObjectName("antiquacrm_systemtray");
  setToolTip(tr("Database connected."));

  QMenu *m_menu = new QMenu();
  ac_show = m_menu->addAction(AntiquaCRM::AntiquaApplIcon("window-fullscreen"),
                              tr("Window show"));
  ac_hide = m_menu->addAction(
      AntiquaCRM::AntiquaApplIcon("window-nofullscreen"), tr("Window hide"));
  m_menu->addSeparator();
  ac_quit = m_menu->addAction(AntiquaCRM::AntiquaApplIcon("action-quit"),
                              tr("Application Quit"));
  setContextMenu(m_menu);

  connect(ac_show, SIGNAL(triggered()), this, SIGNAL(sendShowWindow()));
  connect(ac_hide, SIGNAL(triggered()), this, SIGNAL(sendHideWindow()));
  connect(ac_quit, SIGNAL(triggered()), this, SIGNAL(sendApplQuit()));
  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
          SLOT(actionReason(QSystemTrayIcon::ActivationReason)));
}

void SystemTrayIcon::actionReason(QSystemTrayIcon::ActivationReason r) {
  if (r == QSystemTrayIcon::Context)
    return;

  emit sendToggleView();
}

void SystemTrayIcon::setConnectionStatus(bool b) {
  if (b) {
    setToolTip(tr("Database connected."));
    setIcon(AntiquaCRM::AntiquaApplIcon("database-update"));
  } else {
    setToolTip(tr("No Database conenction!"));
    setIcon(AntiquaCRM::AntiquaApplIcon("database-status"));
  }
}

void SystemTrayIcon::setMessage(const QString &body) {
  QString title(ANTIQUACRM_WINDOW_TITLE);
  showMessage(title, body + "\n", icon(), (5 * 1000));
}
