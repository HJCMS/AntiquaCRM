// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDERSCUSTOMERINFO_H
#define ANTIQUACRM_PLUGIN_ORDERSCUSTOMERINFO_H

#include <AntiquaWidgets>
#include <QLabel>
#include <QStackedWidget>

class ANTIQUACRM_LIBRARY OrdersCustomerInfo final : public QWidget {
  Q_OBJECT

private:
  QStackedWidget *m_stackedWidget;
  QLabel *m_pageLabel;

  void setInfoText(int);

private Q_SLOTS:
  void swapPage();

public:
  AntiquaCRM::SerialId *o_customer_id;
  AntiquaCRM::TextField *c_postal_address;
  AntiquaCRM::TextField *c_shipping_address;
  explicit OrdersCustomerInfo(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_PLUGIN_ORDERSCUSTOMERINFO_H
