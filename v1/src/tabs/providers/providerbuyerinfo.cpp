// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerbuyerinfo.h"

#include <QLabel>
#include <QLayout>

ProviderBuyerInfo::ProviderBuyerInfo(QWidget *parent) : QWidget{parent} {
  setContentsMargins(0, 0, 0, 0);

  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 2, 0, 2);

  // Left
  QLabel *lb_invoice = new QLabel(this);
  lb_invoice->setText(tr("Invoice Address"));
  lb_invoice->setIndent(5);
  layout->addWidget(lb_invoice, 0, 1, 1, 1, Qt::AlignLeft);

  m_invoice = new TextField(this);
  m_invoice->setObjectName("c_postal_address");
  m_invoice->setEditable(false);
  layout->addWidget(m_invoice, 1, 1, 1, 1);

  // Right
  QLabel *lb_delivery = new QLabel(this);
  lb_delivery->setText(tr("Delivery Address"));
  lb_delivery->setIndent(5);
  layout->addWidget(lb_delivery, 0, 2, 1, 1, Qt::AlignLeft);

  m_delivery = new TextField(this);
  m_delivery->setObjectName("c_shipping_address");
  m_delivery->setEditable(false);
  layout->addWidget(m_delivery, 1, 2, 1, 1);

  setLayout(layout);
}

void ProviderBuyerInfo::setAddressData(const QJsonObject &obj) {
  if (obj.isEmpty())
    return;

  if (obj.contains("c_postal_address"))
    m_invoice->setValue(obj.value("c_postal_address").toString());

  if (obj.contains("c_shipping_address"))
    m_delivery->setValue(obj.value("c_shipping_address").toString());
}
