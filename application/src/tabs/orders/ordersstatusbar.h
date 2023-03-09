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

/**
 * @brief Inventory Orders Status bar
 * @ingroup _orders
 */
class OrdersStatusBar final : public TabStatusBar {
  Q_OBJECT

private:
  OrdersHistoryButton *btn_history;

private Q_SLOTS:
  void setHistoryAction(int) override;

public Q_SLOTS:
  void setCreateButtonEnabled(bool) override{};

public:
  explicit OrdersStatusBar(QWidget *parent = nullptr);
  bool isCreateButtonEnabled() override { return false; };
  static const QString searchYear(int y = QDate::currentDate().year());
};

#endif // ANTIQUACRM_ORDERSSTATUSBAR_H
