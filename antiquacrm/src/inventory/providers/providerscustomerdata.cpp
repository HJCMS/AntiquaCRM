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
  c_id = new SerialID(this);
  c_id->setObjectName("c_id");
  c_id->setInfo(tr("Customer Id"));
  customerLayout->addWidget(c_id, 4, 0, 1, 1);
  m_info = new QLabel(this);
  m_info->setWordWrap(true);
  QString info(tr("at first, click on the customer button and check his "
                  "dataset, it prevents create order failures."));
  m_info->setText(info);
  customerLayout->addWidget(m_info, 5, 0, 1, 2);
  QTabWidget *m_tabWidget = new QTabWidget(this);
  customerLayout->addWidget(m_tabWidget, 6, 0, 1, 2);

  c_shipping_address = new TextField(this);
  c_shipping_address->setObjectName("c_shipping_address");
  m_tabWidget->addTab(c_shipping_address, myIcon("identity"),
                      tr("delivery address"));

  m_customerComment = new TextField(this);
  m_customerComment->setObjectName("m_customerComment");
  m_tabWidget->addTab(m_customerComment, myIcon("info"), tr("Comments"));

  setLayout(customerLayout);

  connect(c_id, SIGNAL(s_serialChanged(int)), this,
          SLOT(distributeSignals(int)));
}

void ProvidersCustomerData::distributeSignals(int customerId) {
  if (customerId > 0) {
    QString info(tr("Customer exists, don't forget to check his Dataset!"));
    info.append(" ");
    info.append(tr("simply click again the customer button."));
    m_info->setText(info);
    customerIdChanged(customerId);
  }
}

void ProvidersCustomerData::setCustomerId(int customerId) {
  c_id->setValue(customerId);
}

int ProvidersCustomerData::customerId() { return c_id->value().toInt(); }

const QVariant ProvidersCustomerData::getValue(const QString &objName) {
  UtilsMain *w = findChild<UtilsMain *>(objName);
  if (w != nullptr) {
    return w->value();
  }
  return QVariant();
}
