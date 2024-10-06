// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_SELLERS_PURCHASEACTIONBAR_H
#define ANTIQUACRM_PLUGIN_SELLERS_PURCHASEACTIONBAR_H

#include <AntiquaWidgets>
#include <QPushButton>
#include <QStatusBar>

class PurchaseActionBar final : public QStatusBar {
  Q_OBJECT

private:
  QPushButton* btn_customer;
  QPushButton* btn_provider;
  QPushButton* btn_create;
  QPushButton* btn_check;

private Q_SLOTS:
  void createClicked();

Q_SIGNALS:
  void sendViewCustomer();
  void sendProviderAction();
  void sendCreateOrder();
  void sendCheckArticles();

public Q_SLOTS:
  void enableCreateButton(bool b = false);

public:
  explicit PurchaseActionBar(QWidget* parent = nullptr);
};

#endif // ANTIQUACRM_PLUGIN_SELLERS_PURCHASEACTIONBAR_H
