// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSSTATUSBAR_H
#define ANTIQUACRM_ORDERSSTATUSBAR_H

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QDate>
#include <QObject>
#include <QWidget>

class OrdersHistoryButton;

class OrdersStatusBar final : public TabStatusBar {
  Q_OBJECT

private:
  OrdersHistoryButton *btn_history;
  void setHistoryMenu() override{};

private Q_SLOTS:
  void setHistoryAction(int) override;

Q_SIGNALS:
  void sendDefaultView();

public:
  explicit OrdersStatusBar(QWidget *parent = nullptr);
  static const QString searchYear(int y = QDate::currentDate().year());
};

#endif // ANTIQUACRM_ORDERSSTATUSBAR_H
