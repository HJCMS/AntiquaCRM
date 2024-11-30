// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "systemtrayicon.h"
#include "antiquaicon.h"
#include "application.h"

#include <QMenu>

SystemTrayIcon::SystemTrayIcon(const QIcon& icon, Application* parent)
    : QSystemTrayIcon{icon, parent} {
  setObjectName("antiquacrm_systemtray");
  setToolTip(tr("%1 connected.").arg(ANTIQUACRM_NAME));
  setVisible(false);

  const QIcon _fc = AntiquaCRM::antiquaIcon("view-fullscreen");

  QMenu* m_menu = new QMenu();
  ac_show = m_menu->addAction(_fc, tr("Window show"));
  ac_hide = m_menu->addAction(_fc, tr("Window hide"));
  m_menu->addSeparator();
  ac_quit = m_menu->addAction(AntiquaCRM::antiquaIcon("dialog-close"), tr("Application Quit"));
  setContextMenu(m_menu);

  connect(ac_show, SIGNAL(triggered()), SIGNAL(sendShowWindow()));
  connect(ac_hide, SIGNAL(triggered()), SIGNAL(sendHideWindow()));
  connect(ac_quit, SIGNAL(triggered()), SIGNAL(sendApplQuit()));
  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          SLOT(actionReason(QSystemTrayIcon::ActivationReason)));
}

void SystemTrayIcon::actionReason(QSystemTrayIcon::ActivationReason r) {
  switch (r) {
    case (QSystemTrayIcon::Trigger):
    case (QSystemTrayIcon::MiddleClick):
      emit sendToggleView();
      return;

    default:
      return;
  }
}

void SystemTrayIcon::setConnectionStatus(bool b) {
  if (b) {
    setToolTip(tr("Database connected."));
    setIcon(AntiquaCRM::antiquaIcon("database-update"));
  } else {
    setToolTip(tr("No Database conenction!"));
    setIcon(AntiquaCRM::antiquaIcon("database-status"));
  }
}

void SystemTrayIcon::setMessage(const QString& body) {
  QString title(ANTIQUACRM_WINDOW_TITLE);
  showMessage(title, body + "\n", icon(), (5 * 1000));
}
