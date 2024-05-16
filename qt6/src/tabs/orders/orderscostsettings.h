// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDERSCOSTSETTINGS_H
#define ANTIQUACRM_PLUGIN_ORDERSCOSTSETTINGS_H

#include <AntiquaWidgets>
#include <QTabWidget>

class ANTIQUACRM_LIBRARY OrdersCostSettings final : public QTabWidget {
  Q_OBJECT

private:
  QWidget *m_deliveryTab;
  QWidget *m_historyTab;

public:
  AntiquaCRM::SelectDeliverService *o_delivery_service;
  AntiquaCRM::SelectDeliverPackage *o_delivery_package;
  AntiquaCRM::TextLine *o_delivery_note_id;
  AntiquaCRM::TextLine *o_delivery_send_id;
  AntiquaCRM::SelectEUCountry *o_vat_country;
  AntiquaCRM::SelectVAT *o_vat_levels;
  AntiquaCRM::BoolBox *o_delivery_add_price;
  AntiquaCRM::SelectPaymentMethod *o_payment_method;
  AntiquaCRM::TextLine *o_provider_name;
  AntiquaCRM::TextLine *o_provider_order_id;
  AntiquaCRM::DateTimeInfo *o_since;
  AntiquaCRM::DateTimeInfo *o_modified;
  AntiquaCRM::DateTimeInfo *o_delivered;
  explicit OrdersCostSettings(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_PLUGIN_ORDERSCOSTSETTINGS_H
