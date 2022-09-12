// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "systemtray.h"

SystemTray::SystemTray(const QIcon &icon, QObject *parent)
    : QSystemTrayIcon{icon, parent} {
  setObjectName("antiquacrm_systemtray");
  setIcon(icon);
}
