// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquasystemtray.h"
#include "mwindow.h"
#include "myicontheme.h"

#include <QDebug>
#include <QMenu>

AntiquaSystemTray::AntiquaSystemTray(const QIcon &icon, MWindow *parent)
    : QSystemTrayIcon{icon, parent}, m_window(parent) {
  setObjectName("SystemTray");
  setToolTip(tr("AntiquaCRM Application"));

  QMenu *m_menu = new QMenu(m_window);
  ac_show = m_menu->addAction(myIcon("window_fullscreen"), tr("Show"));
  ac_hide = m_menu->addAction(myIcon("window_nofullscreen"), tr("Hide"));
  m_menu->addSeparator();
  ac_quit = m_menu->addAction(myIcon("exit"), tr("Quit"));
  setContextMenu(m_menu);

  connect(ac_show, SIGNAL(triggered()), m_window, SLOT(show()));
  connect(ac_hide, SIGNAL(triggered()), m_window, SLOT(hide()));
  connect(ac_quit, SIGNAL(triggered()), m_window, SLOT(quitApplication()));
  connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
          SLOT(actionReason(QSystemTrayIcon::ActivationReason)));
}

void AntiquaSystemTray::toggleWindow() {
  if (m_window == nullptr)
    return;

  if (m_window->isVisible())
    m_window->hide();
  else
    m_window->show();
}

void AntiquaSystemTray::actionReason(QSystemTrayIcon::ActivationReason r) {
  switch (r) {
  case (QSystemTrayIcon::Context):
    break;

  default:
    toggleWindow();
  };
}

void AntiquaSystemTray::notify(const QString &msg) {
  QString title("AntiquaCRM ");
  title.append(tr("Message"));
  showMessage(title, msg, QSystemTrayIcon::Information, 10000);
}

void AntiquaSystemTray::warning(const QString &msg) {
  QString title("AntiquaCRM ");
  title.append(tr("Warning"));
  showMessage(title, msg, QSystemTrayIcon::Warning, 10000);
}

void AntiquaSystemTray::fatal(const QString &msg) {
  QString title("AntiquaCRM ");
  title.append(tr("Fatal"));
  showMessage(title, msg, QSystemTrayIcon::Critical, 10000);
}
