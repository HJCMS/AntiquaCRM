// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customercontact.h"
// #include "version.h"
#include <SqlCore>

#include <QtCore/QDebug>
// #include <QtGui>
#include <QtWidgets>

CustomerContact::CustomerContact(QWidget *parent) : QWidget{parent} {
  setObjectName("CustomerContact");

  m_sql = new HJCMS::SqlCore(this);

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
  QHBoxLayout *row2 = new QHBoxLayout();
  /** Postleitzahl */
  c_postalcode = new PostalCode(this);
  c_postalcode->setObjectName("c_postalcode");
  c_postalcode->setInfo(tr("Postalcode"));
  row2->addWidget(c_postalcode);

  /** Wohnort */
  c_location = new LineEdit(this);
  c_location->setObjectName("c_location");
  c_location->setInfo(tr("Location"));
  row2->addWidget(c_location);

  /** Strasse */
  c_street = new LineEdit(this);
  c_street->setObjectName("c_street");
  c_street->setInfo(tr("Street"));
  row2->addWidget(c_street);

  // row2->addStretch(1);
  layout->addLayout(row2);
  // END #2

  // BEGIN #3
  int gridRow = 0;
  QGridLayout *row3 = new QGridLayout();
  row3->setObjectName("contact_edit_row2");

  /** Telefon Nummer 1 */
  c_phone_0 = new PhoneEdit(this);
  c_phone_0->setObjectName("c_phone_0");
  c_phone_0->setInfo(tr("Primary phone number"));
  c_phone_0->setRequired(true);
  row3->addWidget(c_phone_0, gridRow, 0, 1, 1);

  /** Telefon Nummer 2 */
  c_phone_1 = new PhoneEdit(this);
  c_phone_1->setObjectName("c_phone_1");
  c_phone_1->setInfo(tr("Secundary phone number"));
  row3->addWidget(c_phone_1, gridRow++, 1, 1, 1);

  /** Mobilerufnummer 1 */
  c_mobil_0 = new PhoneEdit(this);
  c_mobil_0->setObjectName("c_mobil_0");
  c_mobil_0->setInfo(tr("Primary mobile number"));
  row3->addWidget(c_mobil_0, gridRow, 0, 1, 1);

  /** Mobilerufnummer 2 */
  c_mobil_1 = new PhoneEdit(this);
  c_mobil_1->setObjectName("c_mobil_1");
  c_mobil_1->setInfo(tr("Secundary mobile number"));
  row3->addWidget(c_mobil_1, gridRow++, 1, 1, 1);

  /** Faxnummer */
  c_fax_0 = new PhoneEdit(this);
  c_fax_0->setObjectName("c_fax_0");
  c_fax_0->setInfo(tr("fax number"));
  row3->addWidget(c_fax_0, gridRow, 0, 1, 1);

  /** Webseite */
  c_website = new LineEdit(this);
  c_website->setObjectName("c_website");
  c_website->setInfo(tr("Website"));
  c_website->setPlaceholderText("https://antiquacrm.hjcms.de");
  row3->addWidget(c_website, gridRow++, 1, 1, 1);

  /** E-Mail Adresse */
  c_email_0 = new EMailEdit(this);
  c_email_0->setObjectName("c_email_0");
  c_email_0->setInfo(tr("Primary E-Mail Address"));
  row3->addWidget(c_email_0, gridRow, 0, 1, 1);

  c_email_1 = new EMailEdit(this);
  c_email_1->setObjectName("c_email_1");
  c_email_1->setInfo(tr("Secundary E-Mail Address"));
  row3->addWidget(c_email_1, gridRow++, 1, 1, 1);
  layout->addLayout(row3);
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

  connect(c_postalcode, SIGNAL(editingFinished()), this,
          SLOT(postalCodeComplite()));
}

void CustomerContact::fetchCountryFromPostal() {
  QString plz = c_postalcode->value().toString();
  if (plz.isEmpty() || (plz.length() < 4))
    return;

  QString table = c_postalcode->selectedTableName();
  QString sql("SELECT DISTINCT * FROM " + table + " WHERE p_plz=" + plz +
              " ORDER BY p_plz;");

  QStringList list;
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      list.append(q.value("p_location").toString());
    }
  }
  c_location->addCompleter(list);
}

void CustomerContact::postalCodeComplite() { fetchCountryFromPostal(); }
