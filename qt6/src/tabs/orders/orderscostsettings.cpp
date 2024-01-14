// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderscostsettings.h"

#include <QLayout>

OrdersCostSettings::OrdersCostSettings(QWidget* parent)
    : QTabWidget{parent}
{
  int _row = 0;
  m_deliveryTab = new QWidget(this);
  QGridLayout* gridLayout = new QGridLayout(m_deliveryTab);
  gridLayout->setColumnStretch(2, 1);

  // BEGIN:DeliveryLayout
  QHBoxLayout* _deliverLayout = new QHBoxLayout;
  o_delivery_service = new AntiquaCRM::SelectDeliverService(m_deliveryTab);
  o_delivery_service->setObjectName("o_delivery_service");
  o_delivery_service->setInputToolTip(tr("Delivery service"));
  _deliverLayout->addWidget(o_delivery_service);
  o_delivery_package = new AntiquaCRM::SelectDeliverPackage(m_deliveryTab);
  o_delivery_package->setObjectName("o_delivery_package");
  o_delivery_package->setInputToolTip(tr("Delivery package"));
  _deliverLayout->addWidget(o_delivery_package);
  gridLayout->addLayout(_deliverLayout, _row++, 0, 1, 3);
  // END:DeliveryLayout

  o_delivery_send_id = new AntiquaCRM::TextLine(m_deliveryTab);
  o_delivery_send_id->setObjectName("o_delivery_send_id");
  o_delivery_send_id->setBuddyLabel(tr("Parcel Shipment Number"));
  gridLayout->addWidget(o_delivery_send_id, _row++, 0, 1, 3);

  o_delivery_note_id = new AntiquaCRM::TextLine(m_deliveryTab);
  o_delivery_note_id->setObjectName("o_delivery");
  o_delivery_note_id->setReadOnly(true);
  o_delivery_note_id->setBuddyLabel(tr("Delivery note number"));
  gridLayout->addWidget(o_delivery_note_id, _row, 0, 1, 1);

  o_delivery_add_price = new AntiquaCRM::BoolBox(m_deliveryTab);
  o_delivery_add_price->setObjectName("o_delivery_add_price");
  o_delivery_add_price->setBuddyLabel(tr("add delivery package price"));
  o_delivery_add_price->setToolTip(tr("add delivery package price to current shipping."));
  o_delivery_add_price->setValue(false);
  o_delivery_add_price->setRequired(false);
  gridLayout->addWidget(o_delivery_add_price, _row++, 1, 1, 2);

  o_vat_levels = new AntiquaCRM::SelectVAT(m_deliveryTab);
  o_vat_levels->setObjectName("o_vat_levels");
  o_vat_levels->setBuddyLabel(tr("Sales tax in invoice"));
  o_vat_levels->setValue(AntiquaCRM::SalesTax::TAX_INCL);
  o_vat_levels->appendStretch(0);
  gridLayout->addWidget(o_vat_levels, _row, 0, 1, 1);

  o_vat_country = new AntiquaCRM::SelectEUCountry(m_deliveryTab);
  o_vat_country->setBuddyLabel(tr("Country"));
  o_vat_country->setObjectName("o_vat_country");
  gridLayout->addWidget(o_vat_country, _row++, 1, 1, 2);

  o_provider_name = new AntiquaCRM::TextLine(m_deliveryTab);
  o_provider_name->setObjectName("o_provider_name");
  o_provider_name->setBuddyLabel(tr("Provider"));
  o_provider_name->setRequired(true);
  gridLayout->addWidget(o_provider_name, _row, 0, 1, 1);

  o_provider_order_id = new AntiquaCRM::TextLine(m_deliveryTab);
  o_provider_order_id->setObjectName("o_provider_order_id");
  o_provider_order_id->setBuddyLabel(tr("Settlement number"));
  o_provider_order_id->setRequired("true");
  gridLayout->addWidget(o_provider_order_id, _row++, 1, 1, 2);

  o_payment_method = new AntiquaCRM::SelectPaymentMethod(m_deliveryTab);
  o_payment_method->setObjectName("o_payment_method");
  o_payment_method->setBuddyLabel(tr("Provider payment method"));
  gridLayout->addWidget(o_payment_method, _row++, 0, 1, 2, Qt::AlignLeft);
  gridLayout->setRowStretch(_row, 1);

  m_deliveryTab->setLayout(gridLayout);
  addTab(m_deliveryTab, parentWidget()->windowIcon(), tr("Delivery-/Invoice Settings"));

  m_historyTab = new QWidget(this);
  QVBoxLayout* vboxLayout = new QVBoxLayout(m_historyTab);
  o_since = new AntiquaCRM::DateTimeInfo(m_historyTab);
  o_since->setObjectName("o_since");
  o_since->setBuddyLabel(tr("Created at"));
  vboxLayout->addWidget(o_since);
  o_modified = new AntiquaCRM::DateTimeInfo(m_historyTab);
  o_modified->setObjectName("o_modified");
  o_modified->setBuddyLabel(tr("Last modified"));
  vboxLayout->addWidget(o_modified);
  o_delivered = new AntiquaCRM::DateTimeInfo(m_historyTab);
  o_delivered->setObjectName("o_delivered");
  o_delivered->setBuddyLabel(tr("Deliverd at"));
  vboxLayout->addWidget(o_delivered);
  vboxLayout->addStretch(1);
  m_historyTab->setLayout(vboxLayout);
  addTab(m_historyTab, parentWidget()->windowIcon(), tr("History info"));

  connect(o_delivery_service, SIGNAL(sendSelectedService(int)), o_delivery_package,
          SLOT(loadPackages(int)));
}
