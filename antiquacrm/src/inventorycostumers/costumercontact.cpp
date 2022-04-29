// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "costumercontact.h"
#include "version.h"
#include <SqlCore>

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>

CostumerContact::CostumerContact(QWidget *parent) : QWidget{parent} {
  setObjectName("CostumerContact");

  m_sql = new HJCMS::SqlCore(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("contact_edit_layout");

  // BEGIN #1
  QHBoxLayout *row1 = new QHBoxLayout();
  row1->setObjectName("contact_edit_row1");

  c_gender = new GenderBox(this);
  c_gender->setObjectName("c_gender");
  c_gender->setInfo(tr("Gender"));
  row1->addWidget(c_gender);

  QLabel *titleLabel = new QLabel(this);
  titleLabel->setText(tr("Title:"));
  row1->addWidget(titleLabel);

  c_title = new SalutationBox(this);
  c_title->setObjectName("c_title");
  row1->addWidget(c_title);

  c_firstname = new LineEdit(this);
  c_firstname->setObjectName("c_firstname");
  c_firstname->setPlaceholderText(tr("Firstname"));
  c_firstname->setInfo(tr("Firstname"));
  c_firstname->restrictDisplay(80, 150);
  c_firstname->setRequired(true);
  row1->addWidget(c_firstname);

  c_lastname = new LineEdit(this);
  c_lastname->setObjectName("c_lastname");
  c_lastname->setPlaceholderText(tr("Lastname"));
  c_lastname->restrictDisplay(80, 150);
  c_lastname->setInfo(tr("Lastname"));
  c_lastname->setRequired(true);
  row1->addWidget(c_lastname);

  row1->addStretch(1);
  layout->addLayout(row1);
  // END #1

  // BEGIN #2
  int gridRow = 0;
  QGridLayout *row2 = new QGridLayout();
  row2->setObjectName("contact_edit_row2");

  /** Postleitzahl */
  c_postalcode = new PostalCode(this);
  c_postalcode->setObjectName("c_postalcode");
  c_postalcode->setInfo(tr("Postalcode"));
  c_postalcode->setRequired(true);
  row2->addWidget(c_postalcode, gridRow, 0, 1, 1);

  /** Land */
  c_country = new LineEdit(this);
  c_country->setObjectName("c_country");
  c_country->setInfo(tr("Country"));
  c_country->setToolTip(tr("Country/State or Canton"));
  c_country->setRequired(true);
  row2->addWidget(c_country, gridRow++, 1, 1, 1);

  /** Wohnort */
  c_location = new LineEdit(this);
  c_location->setObjectName("c_location");
  c_location->setInfo(tr("Location"));
  c_location->setRequired(true);
  row2->addWidget(c_location, gridRow, 0, 1, 1);

  /** Strasse */
  c_street = new LineEdit(this);
  c_street->setObjectName("c_street");
  c_street->setInfo(tr("Street"));
  c_street->setRequired(true);
  row2->addWidget(c_street, gridRow++, 1, 1, 1);

  /** Phone 1 */
  c_phone_0 = new PhoneEdit(this);
  c_phone_0->setObjectName("c_phone_0");
  c_phone_0->setInfo(tr("Primary phone number"));
  c_phone_0->setRequired(true);
  row2->addWidget(c_phone_0, gridRow, 0, 1, 1);

  /** Phone 2 */
  c_phone_1 = new PhoneEdit(this);
  c_phone_1->setObjectName("c_phone_1");
  c_phone_1->setInfo(tr("Secundary phone number"));
  row2->addWidget(c_phone_1, gridRow++, 1, 1, 1);

  /** Mobil 1 */
  c_mobil_0 = new PhoneEdit(this);
  c_mobil_0->setObjectName("c_mobil_0");
  c_mobil_0->setInfo(tr("Primary mobile number"));
  row2->addWidget(c_mobil_0, gridRow, 0, 1, 1);

  /** Mobil 2 */
  c_mobil_1 = new PhoneEdit(this);
  c_mobil_1->setObjectName("c_mobil_1");
  c_mobil_1->setInfo(tr("Secundary mobile number"));
  row2->addWidget(c_mobil_1, gridRow++, 1, 1, 1);

  /** Faxnumber */
  c_fax_0 = new PhoneEdit(this);
  c_fax_0->setObjectName("c_fax_0");
  c_fax_0->setInfo(tr("fax number"));
  row2->addWidget(c_fax_0, gridRow, 0, 1, 1);

  /** Webseite */
  c_website = new LineEdit(this);
  c_website->setObjectName("c_website");
  c_website->setInfo(tr("Website"));
  c_website->setPlaceholderText("https://antiquacrm.hjcms.de");
  row2->addWidget(c_website, gridRow++, 1, 1, 1);

  /** E-Mail Primary */
  c_email_0 = new EMailEdit(this);
  c_email_0->setObjectName("c_email_0");
  c_email_0->setInfo(tr("Primary E-Mail Address"));
  row2->addWidget(c_email_0, gridRow, 0, 1, 1);

  /** E-Mail Secundary */
  c_email_1 = new EMailEdit(this);
  c_email_1->setObjectName("c_email_1");
  c_email_1->setInfo(tr("Secundary E-Mail Address"));
  row2->addWidget(c_email_1, gridRow++, 1, 1, 1);
  layout->addLayout(row2);
  // END #2

  // BEGIN #3
  /** Company */
  c_company = new GroupBox(this);
  c_company->setObjectName("c_company");
  c_company->setInfo(tr("Company, Institute or Organisation"));
  layout->addWidget(c_company);
  QHBoxLayout *groupBoxLayout = new QHBoxLayout(c_company->box);
  groupBoxLayout->setContentsMargins(1, 1, 1, 1);
  c_company_name = new StrLineEdit(this);
  c_company_name->setObjectName("c_company_name");
  // c_company_name->setInfo(tr("Company Name"));
  groupBoxLayout->addWidget(c_company_name);
  c_company_employer = new LineEdit(this);
  c_company_employer->setObjectName("c_company_employer");
  c_company_employer->setInfo(tr("Employer"));
  groupBoxLayout->addWidget(c_company_employer);
  c_company->box->setLayout(groupBoxLayout);
  // END 3

  // BEGIN #4
  QGridLayout *row4 = new QGridLayout();
  row4->setObjectName("contact_edit_row5");

  QPushButton *addressGen = new QPushButton(this);
  addressGen->setToolTip(tr("Generate Address with given Dataset."));
  addressGen->setText(tr("Address"));
  addressGen->setIcon(myIcon("group"));
  row4->addWidget(addressGen, 0, 0, 1, 1);

  QLabel *infoShippingAddress = new QLabel(this);
  infoShippingAddress->setText(tr("Shipping Address"));
  row4->addWidget(infoShippingAddress, 0, 1, 1, 1);

  c_postal_address = new TextField(this);
  c_postal_address->setObjectName("c_postal_address");
  c_postal_address->setRequired(true);
  c_postal_address->setInfo(tr("Postal Address"));
  row4->addWidget(c_postal_address, 1, 0, 1, 1);

  c_shipping_address = new TextField(this);
  c_shipping_address->setObjectName("c_shipping_address");
  c_shipping_address->setInfo(tr("Shipping Address"));
  row4->addWidget(c_shipping_address, 1, 1, 1, 1);

  layout->addLayout(row4);
  // END 4
  layout->addStretch(1);
  setLayout(layout);

  connect(addressGen, SIGNAL(clicked()), this, SLOT(generateAddressBody()));
  connect(c_postalcode, SIGNAL(editingFinished()), this,
          SLOT(postalCodeComplite()));
}

void CostumerContact::fetchCountryFromPostal() {
  QString plz = c_postalcode->value().toString();
  if (plz.isEmpty() || (plz.length() < 4))
    return;

  QString table = c_postalcode->selectedTableName();
  if (table.isEmpty())
    return;

  QString sql("SELECT DISTINCT ");
  sql.append("a.p_state AS st, a.p_location AS lo, b.p_country AS co");
  sql.append(" FROM " + table + " AS a LEFT JOIN ui_postalcodes AS b");
  sql.append(" ON b.p_table='" + table + "'");
  sql.append(" WHERE a.p_plz=" + plz + ";");

  QStringList list;
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      list.append(q.value("lo").toString());
      QString country("/");
      country.prepend(q.value("co").toString());
      country.append(q.value("st").toString());
      c_country->setValue(country);
    }
    if (!list.isEmpty())
      c_location->addCompleter(list);

  } else {
    qDebug() << m_sql->lastError();
  }
}

void CostumerContact::postalCodeComplite() { fetchCountryFromPostal(); }

void CostumerContact::generateAddressBody() {
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
  QString body;
  /**< Company */
  if (c_company->value().toBool()) {
    if (!c_company_name->value().toString().isEmpty()) {
      body.append(c_company_name->value().toString());
      body.append("\n");
    }
  }
  body.append(name.join(" "));
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
