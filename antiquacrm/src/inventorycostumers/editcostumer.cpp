// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editcostumer.h"
#include "serialid.h"
#include "costumeradditional.h"
#include "costumerbillinginfo.h"
#include "costumercontact.h"
#include "costumeroverview.h"
#include "genderbox.h"
#include "version.h"

#include <QtCore/QDebug>
// #include <QtGui>
#include <QtWidgets>

EditCostumer::EditCostumer(QWidget *parent) : QWidget{parent} {
  setObjectName("EditCostumer");

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("costumer_edit_layout");

  QHBoxLayout *headerLayout = new QHBoxLayout();
  headerLayout->setObjectName("header_layout");

  c_id = new SerialID(this);
  c_id->setObjectName("c_id");
  c_id->setInfo(tr("Costumers ID:"));
  c_id->setValue(36589);
  headerLayout->addWidget(c_id);

  infoLabel = new QLabel(this);
  infoLabel->setText("Buch- und Kunsthandlung Antiquariat Wilms");
  headerLayout->addWidget(infoLabel);

  headerLayout->addStretch(1);

  mainLayout->addLayout(headerLayout);

  m_dataBox = new QToolBox(this);
  m_dataBox->setObjectName("costumer_edit_box");

  m_overview = new CostumerOverview(m_dataBox);
  m_overview->setObjectName("overview");
  m_dataBox->addItem(m_overview, myIcon("edit_group"), tr("Overview"));

  m_contact = new CostumerContact(m_dataBox);
  m_contact->setObjectName("contact");
  m_dataBox->addItem(m_contact, myIcon("identity"), tr("Edit Contact"));

  m_billing = new CostumerBillingInfo(m_dataBox);
  m_billing->setObjectName("shipping");
  m_dataBox->addItem(m_billing, myIcon("list"), tr("Edit Billing"));

  m_additional = new CostumerAdditional(m_dataBox);
  m_additional->setObjectName("additional");
  m_dataBox->addItem(m_additional, myIcon("filetypes"), tr("Additional"));

  mainLayout->addWidget(m_dataBox);

  setLayout(mainLayout);
}
