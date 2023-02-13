// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SYSTEMTRAY_H
#define ANTIQUACRM_SYSTEMTRAY_H

#include <QAction>
#include <QObject>
#include <QSystemTrayIcon>

class AntiquaSystemTray : public QSystemTrayIcon {
  Q_OBJECT

private:
  QAction *ac_show;
  QAction *ac_hide;
  QAction *ac_quit;

  const QIcon getIcon(const QString &name) const;

private Q_SLOTS:
  void actionReason(QSystemTrayIcon::ActivationReason r);

Q_SIGNALS:
  void sendToggleView();
  void sendShowWindow();
  void sendHideWindow();
  void sendApplQuit();

public Q_SLOTS:
  void setConnectionStatus(bool);
  void setMessage(const QString &body);

public:
  explicit AntiquaSystemTray(const QIcon &icon, QObject *parent = nullptr);
};

#endif // ANTIQUACRM_SYSTEMTRAY_H
