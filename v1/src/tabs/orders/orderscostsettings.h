// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSCOSTSETTINGS_H
#define ANTIQUACRM_ORDERSCOSTSETTINGS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QTabWidget>
#include <QWidget>

class DeliverService;

class OrdersCostSettings : public QTabWidget {
  Q_OBJECT

private:
  QWidget *m_deliveryTab;
  QWidget *m_invoiceTab;
  QWidget *m_providerTab;
  QWidget *m_historyTab;

public:
  DeliverService *o_delivery_service;
  SerialID *o_delivery;
  LineEdit *o_delivery_send_id;
  IntSpinBox *o_delivery_package;
  SelectEuCountry *o_vat_country;
  TaxLevelBox *o_vat_levels;
  BoolBox *o_vat_included;
  BoolBox *o_delivery_add_price;
  LineEdit *o_provider_name;
  LineEdit *o_provider_order_id;
  AntiquaDateInfo *o_since;
  AntiquaDateInfo *o_modified;
  AntiquaDateInfo *o_delivered;
  explicit OrdersCostSettings(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_ORDERSCOSTSETTINGS_H
