// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customercontact.h"
#include "version.h"
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

  c_gender = new GenderBox(this);
  c_gender->setInfo(tr("Gender"));
  row1->addWidget(c_gender);

  QLabel *titleLabel = new QLabel(this);
  titleLabel->setText(tr("Title:"));
  row1->addWidget(titleLabel);

  c_title = new SalutationBox(this);
  row1->addWidget(c_title);

  c_firstname = new LineEdit(this);
  c_firstname->setObjectName("c_firstname");
  c_firstname->setPlaceholderText(tr("Firstname"));
  c_firstname->setInfo(tr("Firstname"));
  c_firstname->restrictDisplay(80, 150);
  row1->addWidget(c_firstname);

  c_lastname = new LineEdit(this);
  c_lastname->setObjectName("c_lastname");
  c_lastname->setPlaceholderText(tr("Lastname"));
  c_lastname->restrictDisplay(80, 150);
  c_lastname->setInfo(tr("Lastname"));
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

  helper_state = new QLabel(this);
  helper_state->setObjectName("c_postalcode");
  helper_state->setToolTip(tr("State / Canton"));
  row2->addWidget(helper_state);

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
  row3->setObjectName("contact_edit_row3");

  /** Phone 1 */
  c_phone_0 = new PhoneEdit(this);
  c_phone_0->setObjectName("c_phone_0");
  c_phone_0->setInfo(tr("Primary phone number"));
  c_phone_0->setRequired(true);
  row3->addWidget(c_phone_0, gridRow, 0, 1, 1);

  /** Phone 2 */
  c_phone_1 = new PhoneEdit(this);
  c_phone_1->setObjectName("c_phone_1");
  c_phone_1->setInfo(tr("Secundary phone number"));
  row3->addWidget(c_phone_1, gridRow++, 1, 1, 1);

  /** Mobil 1 */
  c_mobil_0 = new PhoneEdit(this);
  c_mobil_0->setObjectName("c_mobil_0");
  c_mobil_0->setInfo(tr("Primary mobile number"));
  row3->addWidget(c_mobil_0, gridRow, 0, 1, 1);

  /** Mobil 2 */
  c_mobil_1 = new PhoneEdit(this);
  c_mobil_1->setObjectName("c_mobil_1");
  c_mobil_1->setInfo(tr("Secundary mobile number"));
  row3->addWidget(c_mobil_1, gridRow++, 1, 1, 1);

  /** Faxnumber */
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

  /** E-Mail Primary */
  c_email_0 = new EMailEdit(this);
  c_email_0->setObjectName("c_email_0");
  c_email_0->setInfo(tr("Primary E-Mail Address"));
  row3->addWidget(c_email_0, gridRow, 0, 1, 1);

  /** E-Mail Secundary */
  c_email_1 = new EMailEdit(this);
  c_email_1->setObjectName("c_email_1");
  c_email_1->setInfo(tr("Secundary E-Mail Address"));
  row3->addWidget(c_email_1, gridRow++, 1, 1, 1);
  layout->addLayout(row3);
  // END #3

  // BEGIN #4
  /** Company */
  c_company = new QGroupBox(this);
  c_company->setObjectName("c_company");
  c_company->setTitle(tr("Company, Institute or Organisation"));
  c_company->setCheckable(true);
  c_company->setChecked(false);
  QHBoxLayout *boxLayout = new QHBoxLayout(c_company);
  boxLayout->setContentsMargins(1, 1, 1, 1);
  c_company_name = new LineEdit(this);
  c_company_name->setObjectName("c_company_name");
  c_company_name->setInfo(tr("Company Name"));
  boxLayout->addWidget(c_company_name);
  c_company_employer = new LineEdit(this);
  c_company_employer->setObjectName("c_company_employer");
  c_company_employer->setInfo(tr("Employer"));
  boxLayout->addWidget(c_company_employer);
  c_company->setLayout(boxLayout);
  layout->addWidget(c_company);
  // END 4

  // BEGIN #5
  QGridLayout *row5 = new QGridLayout();
  row5->setObjectName("contact_edit_row3");

  QPushButton *addressGen = new QPushButton(this);
  addressGen->setToolTip(tr("Generate Address with given Dataset."));
  addressGen->setText(tr("Address"));
  addressGen->setIcon(myIcon("group"));
  row5->addWidget(addressGen, 0, 0, 1, 1);

  QLabel *infoShippingAddress = new QLabel(this);
  infoShippingAddress->setText(tr("Shipping Address"));
  row5->addWidget(infoShippingAddress, 0, 1, 1, 1);

  c_postal_address = new TextField(this);
  c_postal_address->setObjectName("c_postal_address");
  row5->addWidget(c_postal_address, 1, 0, 1, 1);

  c_shipping_address = new TextField(this);
  c_shipping_address->setObjectName("c_shipping_address");
  row5->addWidget(c_shipping_address, 1, 1, 1, 1);

  layout->addLayout(row5);
  // END 5

  layout->addStretch(1);

  setLayout(layout);

  connect(addressGen, SIGNAL(clicked()), this, SLOT(generateAddressBody()));
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
      helper_state->setText(q.value("p_state").toString());
    }
  }
  c_location->addCompleter(list);
}

void CustomerContact::postalCodeComplite() { fetchCountryFromPostal(); }

void CustomerContact::generateAddressBody() {
  QStringList name;
  QStringList address;
  /**< Titelanrede */
  if (!c_title->value().toString().isEmpty()) {
    name.append(c_title->value().toString());
  }
  /**< Vorname */
  if (!c_firstname->value().toString().isEmpty()) {
    name.append(c_firstname->value().toString());
  }
  /**< Nachname */
  if (!c_lastname->value().toString().isEmpty()) {
    name.append(c_lastname->value().toString());
  }
  /**< Postleitzahl */
  if (!c_postalcode->value().toString().isEmpty()) {
    address.append(c_postalcode->value().toString());
  }
  /**< Wohnort */
  if (!c_location->value().toString().isEmpty()) {
    address.append(c_location->value().toString());
  }
  QString body = name.join(" ");
  body.append("\n");
  body.append(address.join(" "));
  body.append("\n");

  /**< Straße */
  if (!c_street->value().toString().isEmpty()) {
    body.append(c_street->value().toString());
  }

  if (!body.trimmed().isEmpty())
    c_postal_address->setValue(body);
}
