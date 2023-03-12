// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_TABBAR_H
#define ANTIQUACRM_TABBAR_H

#include <QContextMenuEvent>
#include <QObject>
#include <QTabBar>
#include <QWheelEvent>
#include <QWidget>

/**
 * @brief The AntiquaTabBar class
 * @ingroup widgets
 */
class AntiquaTabBar final : public QTabBar {
  Q_OBJECT

private:
  bool enableWheel;
  int index = -1;

protected:
  void tabInserted(int index) override;
  void wheelEvent(QWheelEvent *event) override;

protected Q_SLOTS:
  void tabIndexChanged(int);
  void checkToClose();

Q_SIGNALS:
  void sendTabChanged(int index);
  void sendCloseTab(int index);

public:
  explicit AntiquaTabBar(QWidget *parent = nullptr, bool wheelEvents = false);
  void setTabCloseable(int index, bool closeable = false);
};

#endif // ANTIQUACRM_TABBAR_H
