// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_UI_SYSTEMTRAYICON_H
#define ANTIQUA_UI_SYSTEMTRAYICON_H

#include <AGlobal>
#include <QAction>
#include <QSystemTrayIcon>

class SystemTrayIcon final : public QSystemTrayIcon {
  Q_OBJECT

private:
  QAction *ac_show;
  QAction *ac_hide;
  QAction *ac_quit;

private Q_SLOTS:
  void actionReason(QSystemTrayIcon::ActivationReason r);

Q_SIGNALS:
  void sendToggleView();
  void sendShowWindow();
  void sendHideWindow();
  void sendApplQuit();

public Q_SLOTS:
  void setConnectionStatus(bool);
  Q_INVOKABLE void setMessage(const QString &body);

public:
  explicit SystemTrayIcon(const QIcon &icon, QObject *parent = nullptr);
};

#endif // ANTIQUA_UI_SYSTEMTRAYICON_H
