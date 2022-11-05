// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSSTATUSBAR_H
#define ANTIQUACRM_ORDERSSTATUSBAR_H

#include <QObject>
#include <QWidget>
#include <tabstatusbar.h>

class OrdersStatusBar final : public TabStatusBar {
  Q_OBJECT

private:
  void setHistoryMenu() override;

private Q_SLOTS:
  void setHistoryAction(int) override;

public:
  explicit OrdersStatusBar(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_ORDERSSTATUSBAR_H
