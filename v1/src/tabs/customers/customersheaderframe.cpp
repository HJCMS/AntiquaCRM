// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customersheaderframe.h"

CustomersHeaderFrame::CustomersHeaderFrame(QWidget *parent) : QFrame{parent} {
  setObjectName("customers_info_header");
  setContentsMargins(0, 0, 0, 0);
  QHBoxLayout *r1Layout = new QHBoxLayout(this);
  r1Layout->setContentsMargins(2, 0, 2, 0);
  // Kunden Nummer
  c_id = new SerialID(this);
  c_id->setObjectName("c_id");
  c_id->setInfo("Id");
  c_id->setToolTip("Customer Id");
  r1Layout->addWidget(c_id);
  // Anzeigename
  r1Layout->addWidget(new QLabel(" - ", this));
  m_displayName = new QLabel(this);
  m_displayName->setStyleSheet("QLabel {font: bold italic large;margin-right:10px;}");
  r1Layout->addWidget(m_displayName);
  r1Layout->addStretch(1);
  // Datums Infos
  c_since = new AntiquaDateInfo(this);
  c_since->setObjectName("c_since");
  c_since->setDisplayFormat("ddd d MMMM yyyy");
  c_since->setInfo("Since");
  r1Layout->addWidget(c_since);
  c_changed = new AntiquaDateInfo(this);
  c_changed->setObjectName("c_changed");
  c_changed->setDisplayFormat("ddd d MMMM yyyy");
  c_changed->setInfo("Last changed");
  r1Layout->addWidget(c_changed);
  // Bestellungen
  c_transactions = new SerialID(this);
  c_transactions->setObjectName("c_transactions");
  c_transactions->setValue(0);
  c_transactions->setInfo("Orders");
  r1Layout->addWidget(c_transactions);
  // Einkäufe
  c_purchases = new SerialID(this);
  c_purchases->setObjectName("c_purchases");
  c_purchases->setValue(0);
  c_purchases->setInfo("Purchases");
  r1Layout->addWidget(c_purchases);
  setLayout(r1Layout);
}

void CustomersHeaderFrame::setDisplayName(const QString &info) {
  m_displayName->setText(info);
}
