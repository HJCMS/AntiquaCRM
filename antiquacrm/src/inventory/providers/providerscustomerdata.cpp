// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerscustomerdata.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "myicontheme.h"

#include <QGridLayout>

ProvidersCustomerData::ProvidersCustomerData(QWidget *parent)
    : QGroupBox{parent} {
  setObjectName("display_purchaser");
  setToolTip(tr("purchaser"));
  setFlat(true);
  setStyleSheet("QGroupBox {border:none;}");

  QGridLayout *customerLayout = new QGridLayout(this);
  QHBoxLayout *hLayout = new QHBoxLayout();
  c_gender = new GenderBox(this);
  c_gender->setObjectName("c_gender");
  c_gender->setInfo(tr("Form"));
  hLayout->addWidget(c_gender);
  c_firstname = new LineEdit(this);
  c_firstname->setObjectName("c_firstname");
  c_firstname->setInfo(tr("Firstname"));
  hLayout->addWidget(c_firstname);
  c_lastname = new LineEdit(this);
  c_lastname->setObjectName("c_lastname");
  c_lastname->setInfo(tr("Lastname"));
  hLayout->addWidget(c_lastname);
  customerLayout->addLayout(hLayout, 0, 0, 1, 2);
  c_postalcode = new PostalCode(this);
  c_postalcode->setObjectName("c_postalcode");
  c_postalcode->setInfo(tr("Postalcode"));
  customerLayout->addWidget(c_postalcode, 1, 0, 1, 1);
  c_location = new LineEdit(this);
  c_location->setObjectName("c_location");
  c_location->setInfo(tr("Location"));
  customerLayout->addWidget(c_location, 1, 1, 1, 1);
  c_street = new LineEdit(this);
  c_street->setObjectName("c_street");
  c_street->setInfo(tr("Street"));
  customerLayout->addWidget(c_street, 2, 0, 1, 1);
  c_country = new LineEdit(this);
  c_country->setObjectName("c_country");
  c_country->setInfo(tr("Country"));
  customerLayout->addWidget(c_country, 2, 1, 1, 1);
  c_phone_0 = new PhoneEdit(this);
  c_phone_0->setObjectName("c_phone_0");
  c_phone_0->setInfo(tr("Phone"));
  customerLayout->addWidget(c_phone_0, 3, 0, 1, 1);
  c_email_0 = new EMailEdit(this);
  c_email_0->setObjectName("c_email_0");
  c_email_0->setInfo(tr("eMail"));
  customerLayout->addWidget(c_email_0, 3, 1, 1, 1);
  setLayout(customerLayout);
}
