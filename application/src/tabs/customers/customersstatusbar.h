// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CUSTOMERSSTATUSBAR_H
#define ANTIQUACRM_CUSTOMERSSTATUSBAR_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

/**
 * @brief Customers Status bar
 * @ingroup AntiquaCustomers
 */
class CustomersStatusBar final : public TabStatusBar {
  Q_OBJECT

private:
  QPushButton *btn_create;
  QPushButton *btn_history;

private Q_SLOTS:
  void setHistoryAction(int) override;

public Q_SLOTS:
  void setCreateButtonEnabled(bool) override;

public:
  explicit CustomersStatusBar(QWidget *parent = nullptr);
  bool isCreateButtonEnabled() override;
};

#endif // ANTIQUACRM_CUSTOMERSSTATUSBAR_H
