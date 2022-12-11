// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_STATUSBAR_H
#define ANTIQUACRM_STATUSBAR_H

#include <AntiquaCRM>
#include <QMainWindow>
#include <QObject>
#include <QStatusBar>
#include <QToolBar>
#include <QToolButton>
#include <QWidget>

class AntiquaTimer;

class AntiquaDatabaseBar final : public QToolBar {
  Q_OBJECT

private:
  QAction *ac_status;

private Q_SLOTS:
  void databaseInfoDialog();

public Q_SLOTS:
  void setStatus(AntiquaCRM::ASqlCore::Status);

public:
  explicit AntiquaDatabaseBar(QWidget *parent = nullptr);

};

class AntiquaStatusBar final : public QStatusBar {
  Q_OBJECT

private:
  int timeout_seconds = 10;
  AntiquaCRM::ASqlCore *m_sql;
  AntiquaDatabaseBar *m_toolBar;
  AntiquaTimer *m_timer;

private Q_SLOTS:
  void timerTriggered();

public Q_SLOTS:
  void statusInfoMessage(const QString &);
  void statusWarnMessage(const QString &);

public:
  explicit AntiquaStatusBar(QMainWindow *parent = nullptr);
  ~AntiquaStatusBar();
};

#endif // ANTIQUACRM_STATUSBAR_H
