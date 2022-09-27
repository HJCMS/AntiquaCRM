// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_STATUSBAR_H
#define ANTIQUACRM_STATUSBAR_H

#include <QMainWindow>
#include <QObject>
#include <QStatusBar>

class AntiquaStatusBar final : public QStatusBar {
  Q_OBJECT

public:
  explicit AntiquaStatusBar(QMainWindow *parent = nullptr);
};

#endif // ANTIQUACRM_STATUSBAR_H
