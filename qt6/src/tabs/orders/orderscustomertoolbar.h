// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDERSCUSTOMERTOOLBAR_H
#define ANTIQUACRM_PLUGIN_ORDERSCUSTOMERTOOLBAR_H

#include <AntiquaWidgets>
#include <QFrame>
#include <QToolButton>

class OrdersCustomerToolBar : public QFrame {
  Q_OBJECT

private:
  QHBoxLayout* m_layout;
  AntiquaCRM::SerialId* m_edit = nullptr;
  QAction* ac_open;
  QAction* ac_edit;
  qint64 customerId();

private Q_SLOTS:
  void openCustomer();
  void changeCustomer();

public:
  explicit OrdersCustomerToolBar(QWidget *parent = nullptr);
  void addEditWidget(AntiquaCRM::SerialId*);
};

#endif // ANTIQUACRM_PLUGIN_ORDERSCUSTOMERTOOLBAR_H
