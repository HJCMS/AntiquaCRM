// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customercontact.h"
#include "genderbox.h"
#include "phoneedit.h"
#include "salutationbox.h"
#include "emailedit.h"
#include "strlineedit.h"
// #include "version.h"

#include <QtCore/QDebug>
// #include <QtGui>
#include <QtWidgets>

CustomerContact::CustomerContact(QWidget *parent) : QWidget{parent} {
  setObjectName("CustomerContact");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("contact_edit_layout");

  // BEGIN #1
  QHBoxLayout *row1 = new QHBoxLayout();
  row1->setObjectName("contact_edit_row1");

  QLabel *genderLabel = new QLabel(this);
  genderLabel->setText(tr("Gender:"));
  row1->addWidget(genderLabel);

  c_gender = new GenderBox(this);
  row1->addWidget(c_gender);

  QLabel *titleLabel = new QLabel(this);
  titleLabel->setText(tr("Title:"));
  row1->addWidget(titleLabel);

  c_title = new SalutationBox(this);
  row1->addWidget(c_title);

  QLabel *firstNameLabel = new QLabel(this);
  firstNameLabel->setText(tr("Firstname:"));
  row1->addWidget(firstNameLabel);

  c_firstname = new QLineEdit(this);
  c_firstname->setObjectName("c_firstname");
  c_firstname->setPlaceholderText(tr("Firstname"));
  c_firstname->setMinimumWidth(150);
  c_firstname->setMaxLength(80);
  row1->addWidget(c_firstname);

  QLabel *lastNameLabel = new QLabel(this);
  lastNameLabel->setText(tr("Lastname:"));
  row1->addWidget(lastNameLabel);

  c_lastname = new QLineEdit(this);
  c_lastname->setObjectName("c_lastname");
  c_lastname->setPlaceholderText(tr("Lastname"));
  c_lastname->setMinimumWidth(150);
  c_lastname->setMaxLength(80);
  row1->addWidget(c_lastname);

  row1->addStretch(1);
  layout->addLayout(row1);
  // END #1

  // BEGIN #2
  QGridLayout *row2 = new QGridLayout();
  row2->setObjectName("contact_edit_row2");

  /** Telefon Nummer 1 */
  c_phone_0 = new PhoneEdit(this);
  c_phone_0->setObjectName("c_phone_0");
  c_phone_0->setInfoText(tr("Primary phone number"));
  c_phone_0->setRequired(true);
  row2->addWidget(c_phone_0, 0, 0, 1, 1);

  /** Telefon Nummer 2 */
  c_phone_1 = new PhoneEdit(this);
  c_phone_1->setObjectName("c_phone_1");
  c_phone_1->setInfoText(tr("Secundary phone number"));
  row2->addWidget(c_phone_1, 0, 1, 1, 1);
  layout->addLayout(row2);

  /** Mobilerufnummer 1 */
  c_mobil_0 = new PhoneEdit(this);
  c_mobil_0->setObjectName("c_mobil_0");
  c_mobil_0->setInfoText(tr("Primary mobile number"));
  row2->addWidget(c_mobil_0, 1, 0, 1, 1);

  /** Mobilerufnummer 2 */
  c_mobil_1 = new PhoneEdit(this);
  c_mobil_1->setObjectName("c_mobil_1");
  c_mobil_1->setInfoText(tr("Secundary mobile number"));
  row2->addWidget(c_mobil_1, 1, 1, 1, 1);

  /** Faxnummer */
  c_fax_0 = new PhoneEdit(this);
  c_fax_0->setObjectName("c_fax_0");
  c_fax_0->setInfoText(tr("fax number"));
  row2->addWidget(c_fax_0, 2, 0, 1, 1);

  /** Webseite */
  QHBoxLayout *webLayout = new QHBoxLayout();
  QLabel *infoWeb = new QLabel(this);
  infoWeb->setText(tr("Website:"));
  webLayout->addWidget(infoWeb);

  c_website = new StrLineEdit(this);
  c_website->setObjectName("c_website");
  c_website->setMaxAllowedLength(80);
  webLayout->addWidget(c_website);
  row2->addLayout(webLayout, 2, 1, 1, 1);

  /** E-Mail Adresse */
  c_email_0 = new EMailEdit(this);
  c_email_0->setObjectName("c_email_0");
  c_email_0->setInfoText(tr("Primary E-Mail Address"));
  row2->addWidget(c_email_0, 3, 0, 1, 1);

  c_email_1 = new EMailEdit(this);
  c_email_1->setObjectName("c_email_1");
  c_email_1->setInfoText(tr("Secundary E-Mail Address"));
  row2->addWidget(c_email_1, 3, 1, 1, 1);
  layout->addLayout(row2);
  // END #3

  // BEGIN #4
  c_company = new QGroupBox(this);
  c_company->setObjectName("c_company");
  c_company->setTitle(tr("Company, Institute or Organisation"));
  c_company->setCheckable(true);
  c_company->setChecked(true);
  layout->addWidget(c_company);
  // END 4

  layout->addStretch(1);

  setLayout(layout);
}
