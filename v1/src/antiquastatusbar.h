// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_STATUSBAR_H
#define ANTIQUACRM_STATUSBAR_H

#include <QLabel>
#include <QMainWindow>
#include <QObject>
#include <QStatusBar>
#include <QToolButton>

class AntiquaStatusBar final : public QStatusBar {
  Q_OBJECT

private:
  QToolButton *m_dbStatus;

public:
  explicit AntiquaStatusBar(QMainWindow *parent = nullptr);
};

#endif // ANTIQUACRM_STATUSBAR_H
