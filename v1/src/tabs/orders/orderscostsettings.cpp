// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderscostsettings.h"
#include "deliverservice.h"

#include <QIcon>
#include <QLayout>

OrdersCostSettings::OrdersCostSettings(QWidget *parent) : QTabWidget{parent} {
  int t = 0;
  m_deliveryTab = new QWidget(this);
  QVBoxLayout *dsLayout = new QVBoxLayout(m_deliveryTab);
  o_delivery_service = new DeliverService(m_deliveryTab);
  o_delivery_service->setObjectName("o_delivery_service");
  o_delivery_service->setInfo(tr("Service"));
  dsLayout->addWidget(o_delivery_service);
  o_delivery_send_id = new LineEdit(m_deliveryTab);
  o_delivery_send_id->setObjectName("o_delivery_send_id");
  o_delivery_send_id->setInfo(tr("Parcel Shipment Number"));
  dsLayout->addWidget(o_delivery_send_id);
  o_delivery = new DeliveryId(m_deliveryTab);
  o_delivery->setObjectName("o_delivery");
  o_delivery->setInfo(tr("Delivery note number"));
  dsLayout->addWidget(o_delivery);
  o_delivery_package = new IntSpinBox(m_deliveryTab);
  o_delivery_package->setObjectName("o_delivery_package");
  o_delivery_package->setInfo("Package Id");
  o_delivery_package->setVisible(false);
  dsLayout->addWidget(o_delivery_package, Qt::AlignLeft);
  dsLayout->addStretch(1);
  m_deliveryTab->setLayout(dsLayout);
  insertTab(t++, m_deliveryTab, QIcon("://icons/user_male.png"),
            tr("Delivery Settings"));

  connect(o_delivery_service, SIGNAL(sendDeliveryPackage(int)),
          o_delivery_package, SLOT(setValue(int)));

  m_invoiceTab = new QWidget(this);
  QGridLayout *billingLayout = new QGridLayout(m_invoiceTab);
  int brow = 0;
  o_vat_country = new SelectEuCountry(m_invoiceTab);
  o_vat_country->setInfo(tr("European Countries"));
  o_vat_country->setObjectName("o_vat_country");
  billingLayout->addWidget(o_vat_country, brow++, 0, 1, 2);
  o_vat_included = new BoolBox(m_invoiceTab);
  o_vat_included->setInfo(tr("vat included"));
  o_vat_included->setObjectName("o_vat_included");
  o_vat_included->setToolTip(
      tr("Normally the vat is included in Book articles. Uncheck it to enable "
         "add VAT in Printing Invoice."));
  o_vat_included->setChecked(true);
  billingLayout->addWidget(o_vat_included, brow, 0, 1, 1, Qt::AlignRight);
  o_vat_levels = new TaxLevelBox(m_invoiceTab);
  o_vat_levels->setObjectName("o_vat_levels");
  billingLayout->addWidget(o_vat_levels, brow++, 1, 1, 1);
  o_delivery_add_price = new BoolBox(m_invoiceTab);
  o_delivery_add_price->setObjectName("o_delivery_add_price");
  o_delivery_add_price->setInfo(tr("add delivery package price"));
  o_delivery_add_price->setToolTip(
      tr("add delivery package price to current shipping."));
  o_delivery_add_price->setChecked(false);
  o_delivery_add_price->setRequired(false);
  billingLayout->addWidget(o_delivery_add_price, brow++, 0, 1, 2);
  billingLayout->setRowStretch(brow, 1);
  m_invoiceTab->setLayout(billingLayout);
  insertTab(t++, m_invoiceTab, QIcon("://icons/edit.png"),
            tr("Invoice Settings"));

  m_providerTab = new QWidget(this);
  QGridLayout *providerLayout = new QGridLayout(m_providerTab);
  providerLayout->setColumnStretch(1, 1);
  o_provider_name = new LineEdit(m_providerTab);
  o_provider_name->setObjectName("o_provider_name");
  o_provider_name->setInfo(tr("Provider"));
  o_provider_name->setRequired(true);
  providerLayout->addWidget(o_provider_name, 0, 0, 1, 1);
  o_provider_order_id = new LineEdit(m_providerTab);
  o_provider_order_id->setObjectName("o_provider_order_id");
  o_provider_order_id->setInfo(tr("Provider settlement number"));
  o_provider_order_id->setRequired("true");
  providerLayout->addWidget(o_provider_order_id, 1, 0, 1, 1);
  providerLayout->setRowStretch(2, 1);
  m_providerTab->setLayout(providerLayout);
  insertTab(t++, m_providerTab, QIcon("://icons/groups.png"),
            tr("Provider Settings"));

  m_historyTab = new QWidget(this);
  QVBoxLayout *historyLayout = new QVBoxLayout(m_historyTab);
  o_since = new AntiquaDateInfo(m_historyTab);
  o_since->setObjectName("o_since");
  o_since->setInfo(tr("Created at"));
  historyLayout->addWidget(o_since);
  o_modified = new AntiquaDateInfo(m_historyTab);
  o_modified->setObjectName("o_modified");
  o_modified->setInfo(tr("Last modified"));
  historyLayout->addWidget(o_modified);
  o_delivered = new AntiquaDateInfo(m_historyTab);
  o_delivered->setObjectName("o_delivered");
  o_delivered->setInfo(tr("Deliverd at"));
  historyLayout->addWidget(o_delivered);
  historyLayout->addStretch(1);
  m_historyTab->setLayout(historyLayout);
  insertTab(t++, m_historyTab, QIcon("://icons/view_info.png"),
            tr("History info"));
}
