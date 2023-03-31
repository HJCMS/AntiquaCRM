// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUA_TABS_BAR_H
#define ANTIQUA_TABS_BAR_H

#include <QObject>
#include <QTabBar>
#include <QWheelEvent>
#include <QWidget>

class TabsBar final : public QTabBar {
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
  explicit TabsBar(QWidget *parent = nullptr, bool wheelEvents = false);
  void setTabCloseable(int index, bool closeable = false);
  virtual ~TabsBar();
};

#endif // ANTIQUA_TABS_BAR_H
