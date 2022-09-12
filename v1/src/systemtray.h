// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SYSTEMTRAY_H
#define ANTIQUACRM_SYSTEMTRAY_H

#include <QSystemTrayIcon>
#include <QObject>

class SystemTray : public QSystemTrayIcon
{
  Q_OBJECT

public:
  explicit SystemTray(const QIcon &icon, QObject *parent = nullptr);
};

#endif // ANTIQUACRM_SYSTEMTRAY_H
