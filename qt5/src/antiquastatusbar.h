// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_STATUSBAR_H
#define ANTIQUACRM_STATUSBAR_H

#include <QMainWindow>
#include <QObject>
#include <QStatusBar>
#include <QString>
#include <QWidget>

class AntiquaTimer;
class AntiquaDatabaseBar;

/**
 * @ingroup ui
 * @brief Main Window status bar
 */
class AntiquaStatusBar final : public QStatusBar {
  Q_OBJECT

private:
  AntiquaDatabaseBar *m_toolBar;
  AntiquaTimer *m_timer;

private Q_SLOTS:
  void timerTriggered();

public Q_SLOTS:
  void statusInfoMessage(const QString &, qint8 seconds = 5);
  void statusWarnMessage(const QString &, qint8 seconds = 10);

public:
  explicit AntiquaStatusBar(QMainWindow *parent = nullptr);
  virtual ~AntiquaStatusBar();
};

#endif // ANTIQUACRM_STATUSBAR_H
