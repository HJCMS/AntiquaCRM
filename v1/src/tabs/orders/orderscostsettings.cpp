// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderscostsettings.h"
#include "deliverservice.h"

#include <QIcon>
#include <QLayout>

OrdersCostSettings::OrdersCostSettings(QWidget *parent) : QTabWidget{parent} {
  int tab_index = 0;
  int p1_row = 0;
  m_costdeliveryTab = new QWidget(this);
  QGridLayout *firstLayout = new QGridLayout(m_costdeliveryTab);
  o_delivery_service = new DeliverService(m_costdeliveryTab);
  o_delivery_service->setObjectName("o_delivery_service");
  o_delivery_service->setInfo(tr("Service"));
  firstLayout->addWidget(o_delivery_service, p1_row++, 0, 1, 3);

  o_delivery_send_id = new LineEdit(m_costdeliveryTab);
  o_delivery_send_id->setObjectName("o_delivery_send_id");
  o_delivery_send_id->setInfo(tr("Parcel Shipment Number"));
  firstLayout->addWidget(o_delivery_send_id, p1_row++, 0, 1, 3);

  o_delivery_note_id = new DeliveryId(m_costdeliveryTab);
  o_delivery_note_id->setObjectName("o_delivery");
  o_delivery_note_id->setInfo(tr("Delivery note number"));
  firstLayout->addWidget(o_delivery_note_id, p1_row, 0, 1, 1);

  o_delivery_add_price = new BoolBox(m_costdeliveryTab);
  o_delivery_add_price->setObjectName("o_delivery_add_price");
  o_delivery_add_price->setInfo(tr("add delivery package price"));
  o_delivery_add_price->setToolTip(
      tr("add delivery package price to current shipping."));
  o_delivery_add_price->setChecked(false);
  o_delivery_add_price->setRequired(false);
  firstLayout->addWidget(o_delivery_add_price, p1_row, 1, 1, 1);

  o_delivery_package = new IntSpinBox(m_costdeliveryTab);
  o_delivery_package->setObjectName("o_delivery_package");
  o_delivery_package->setInfo("Package Id");
  o_delivery_package->setVisible(false); // HIDDEN
  firstLayout->addWidget(o_delivery_package, p1_row++, 2, 1, 1);

  o_vat_levels = new TaxLevelBox(m_costdeliveryTab);
  o_vat_levels->setObjectName("o_vat_levels");
  o_vat_levels->setInfo(tr("VAT"));
  o_vat_levels->setValue(2);
  firstLayout->addWidget(o_vat_levels, p1_row, 0, 1, 1, Qt::AlignRight);

  o_vat_included = new BoolBox(m_costdeliveryTab);
  o_vat_included->setInfo(tr("vat included"));
  o_vat_included->setObjectName("o_vat_included");
  o_vat_included->setToolTip(
      tr("Normally the vat is included in Book articles. Uncheck it to enable "
         "add VAT in Printing Invoice."));
  o_vat_included->setChecked(true);
  firstLayout->addWidget(o_vat_included, p1_row++, 1, 1, 1);

  firstLayout->setRowStretch(p1_row++, 1);

  m_costdeliveryTab->setLayout(firstLayout);
  insertTab(tab_index++, m_costdeliveryTab, QIcon("://icons/user_male.png"),
            tr("Delivery-/Invoice Settings"));

  m_providerTab = new QWidget(this);
  QVBoxLayout *providerLayout = new QVBoxLayout(m_providerTab);
  o_provider_name = new LineEdit(m_providerTab);
  o_provider_name->setObjectName("o_provider_name");
  o_provider_name->setInfo(tr("Platform"));
  o_provider_name->setRequired(true);
  providerLayout->addWidget(o_provider_name);

  o_provider_order_id = new LineEdit(m_providerTab);
  o_provider_order_id->setObjectName("o_provider_order_id");
  o_provider_order_id->setInfo(tr("Provider settlement number"));
  o_provider_order_id->setRequired("true");
  providerLayout->addWidget(o_provider_order_id);

  o_payment_method = new ShowPaymentMethod(m_providerTab);
  o_payment_method->setObjectName("o_payment_method");
  o_payment_method->setInfo(tr("Desired payment method"));
  providerLayout->addWidget(o_payment_method);

  o_vat_country = new SelectEuCountry(m_providerTab);
  o_vat_country->setInfo(tr("European Country Info"));
  o_vat_country->setObjectName("o_vat_country");
  providerLayout->addWidget(o_vat_country);

  providerLayout->addStretch(1);
  m_providerTab->setLayout(providerLayout);
  insertTab(tab_index++, m_providerTab, QIcon("://icons/groups.png"),
            tr("Platform"));

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
  insertTab(tab_index++, m_historyTab, QIcon("://icons/view_info.png"),
            tr("History info"));

  // SIGNALS
  connect(o_delivery_service, SIGNAL(sendDeliveryPackage(int)),
          o_delivery_package, SLOT(setValue(int)));
}
