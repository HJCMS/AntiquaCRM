// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customersdatawidget.h"
#include "antiquaicon.h"
#include "genderedit.h"
#include "groupboxedit.h"
#include "phoneedit.h"
#include "postalcodeedit.h"
#include "postalcodelocation.h"
#include "postalcodestate.h"
#include "salutationedit.h"
#include "selecteucountry.h"
#include "textfield.h"
#include "textline.h"
#include "urilineedit.h"

#include <AntiquaCRM>
#ifdef ANTIQUA_DEVELOPMENT
#  include <QDebug>
#endif
#include <QtWidgets>

namespace AntiquaCRM
{

CustomersDataWidget::CustomersDataWidget(QWidget* parent) : QWidget{parent} {
  setWindowTitle(tr("Contact data"));
  setWindowIcon(antiquaIcon("system-users"));

  QVBoxLayout* layout = new QVBoxLayout(this);
  layout->setObjectName("contact_edit_layout");
  layout->setContentsMargins(5, 5, 5, 5);

  // BEGIN #1
  QHBoxLayout* row1 = new QHBoxLayout();
  row1->setObjectName("contact_edit_row1");

  c_gender = new GenderEdit(this);
  c_gender->setObjectName("c_gender");
  c_gender->setBuddyLabel(tr("Gender"));
  c_gender->setRequired(true);
  row1->addWidget(c_gender);

  c_title = new SalutationEdit(this);
  c_title->setObjectName("c_title");
  c_title->setBuddyLabel(tr("Title"));
  row1->addWidget(c_title);

  c_firstname = new TextLine(this);
  c_firstname->setObjectName("c_firstname");
  c_firstname->setBuddyLabel(tr("Firstname"));
  row1->addWidget(c_firstname);

  c_lastname = new TextLine(this);
  c_lastname->setObjectName("c_lastname");
  c_lastname->setBuddyLabel(tr("Lastname"));
  row1->addWidget(c_lastname);

  c_careof = new TextLine(this);
  c_careof->setObjectName("c_careof");
  c_careof->setBuddyLabel(tr("c/o"));
  c_careof->setToolTip(tr("take care of package"));
  row1->addWidget(c_careof);

  row1->addStretch(1);
  layout->addLayout(row1);
  // END #1

  // BEGIN #2
  int gridRow = 0;
  QGridLayout* row2 = new QGridLayout();
  row2->setObjectName("contact_edit_row2");

  /** Postleitzahl */
  c_postalcode = new PostalCodeEdit(this);
  c_postalcode->setObjectName("c_postalcode");
  c_postalcode->setBuddyLabel(tr("Postalcode"));
  c_postalcode->setRequired(true);
  row2->addWidget(c_postalcode, gridRow, 0, 1, 1);

  /** Land */
  QHBoxLayout* countryLayout = new QHBoxLayout();
  c_country = new PostalCodeState(this);
  c_country->setObjectName("c_country");
  c_country->setBuddyLabel(tr("Country"));
  c_country->setToolTip(tr("Country/State or Canton"));
  c_country->setRequired(true);
  countryLayout->addWidget(c_country);

  c_country_bcp47 = new SelectEUCountry(this);
  c_country_bcp47->setObjectName("c_country_bcp47");
  c_country_bcp47->setToolTip("RFC 4647/BCP 47 Language selection");
  countryLayout->addWidget(c_country_bcp47);

  row2->addLayout(countryLayout, gridRow++, 1, 1, 1);

  /** Wohnort */
  c_location = new PostalCodeLocation(this);
  c_location->setObjectName("c_location");
  c_location->setBuddyLabel(tr("Location"));
  c_location->setRequired(true);
  row2->addWidget(c_location, gridRow, 0, 1, 1);

  /** Strasse */
  c_street = new TextLine(this);
  c_street->setObjectName("c_street");
  c_street->setBuddyLabel(tr("Street"));
  row2->addWidget(c_street, gridRow++, 1, 1, 1);

  /** Phone 1 */
  c_phone_0 = new PhoneEdit(this, "c_phone_0");
  c_phone_0->setBuddyLabel(tr("Primary phone number"));
  row2->addWidget(c_phone_0, gridRow, 0, 1, 1);

  /** Phone 2 */
  c_phone_1 = new PhoneEdit(this, "c_phone_1");
  c_phone_1->setBuddyLabel(tr("Secundary phone number"));
  row2->addWidget(c_phone_1, gridRow++, 1, 1, 1);

  /** Mobil 1 */
  c_mobil_0 = new PhoneEdit(this, "c_mobil_0");
  c_mobil_0->setBuddyLabel(tr("Primary mobile number"));
  row2->addWidget(c_mobil_0, gridRow, 0, 1, 1);

  /** Mobil 2 */
  c_mobil_1 = new PhoneEdit(this, "c_mobil_1");
  c_mobil_1->setBuddyLabel(tr("Secundary mobile number"));
  row2->addWidget(c_mobil_1, gridRow++, 1, 1, 1);

  /** Faxnumber */
  c_fax_0 = new PhoneEdit(this, "c_fax_0");
  c_fax_0->setBuddyLabel(tr("fax number"));
  row2->addWidget(c_fax_0, gridRow, 0, 1, 1);

  /** Webseite */
  c_website = new URILineEdit(this);
  c_website->setObjectName("c_website");
  c_website->setBuddyLabel(tr("Website"));
  row2->addWidget(c_website, gridRow++, 1, 1, 1);

  /** E-Mail Primary */
  c_email_0 = new URILineEdit("c_email_0", this);
  c_email_0->setBuddyLabel(tr("Primary eMail Address"));
  row2->addWidget(c_email_0, gridRow, 0, 1, 1);

  /** E-Mail Secundary */
  c_email_1 = new URILineEdit("c_email_1", this);
  c_email_1->setBuddyLabel(tr("Secundary eMail Address"));
  row2->addWidget(c_email_1, gridRow++, 1, 1, 1);
  layout->addLayout(row2);
  // END #2

  // BEGIN #3
  /** CompanyBox */
  c_company = new GroupBoxEdit(this);
  c_company->setObjectName("c_company");
  c_company->setBuddyLabel(tr("Company, Institute or Organisation"));
  layout->addWidget(c_company);
  QBoxLayout* m_boxLayout = c_company->boxLayout();
  c_company_name = new TextLine(c_company);
  c_company_name->setObjectName("c_company_name");
  m_boxLayout->addWidget(c_company_name);
  c_company_employer = new TextLine(c_company);
  c_company_employer->setObjectName("c_company_employer");
  c_company_employer->setBuddyLabel(tr("Employer"));
  m_boxLayout->addWidget(c_company_employer);
  // END 3

  // BEGIN #4
  QGridLayout* row4 = new QGridLayout();
  row4->setObjectName("contact_edit_row5");

  QPushButton* addressGen = new QPushButton(this);
  addressGen->setToolTip(tr("Generate Address with given Dataset."));
  addressGen->setText(tr("Generate Invoice address"));
  addressGen->setIcon(antiquaIcon("view-group"));
  row4->addWidget(addressGen, 0, 0, 1, 1);

  c_postal_address = new TextField(this);
  c_postal_address->setObjectName("c_postal_address");
  c_postal_address->setRequired(true);
  c_postal_address->setBuddyLabel(tr("Invoice Address"));
  row4->addWidget(c_postal_address, 1, 0, 1, 1);

  c_shipping_address = new TextField(this);
  c_shipping_address->setObjectName("c_shipping_address");
  c_shipping_address->setBuddyLabel(tr("Delivery Address"));
  row4->addWidget(c_shipping_address, 1, 1, 1, 1);

  layout->addLayout(row4);
  // END 4
  layout->addStretch(1);
  setLayout(layout);

  // Signals
  connect(c_postalcode, SIGNAL(sendOnLeavePostalEdit(AntiquaCRM::PostalCode)),
          SLOT(setPostalData(AntiquaCRM::PostalCode)));

  connect(addressGen, SIGNAL(clicked()), SLOT(generateAddressBody()));
}

void CustomersDataWidget::setDataField(const QString& name, const QVariant& data) {
  AntiquaCRM::AInputWidget* m_w = findChild<AntiquaCRM::AInputWidget*>(name);
  if (m_w != nullptr) {
    // qDebug() << Q_FUNC_INFO << name << data;
    m_w->setValue(data);
  }
}

void CustomersDataWidget::registerChanges() {
  c_postal_address->setWindowModified(true);
  parentWidget()->setWindowModified(true);
}

void CustomersDataWidget::setPostalData(const PostalCode& code) {
  c_country->setCountry(code);
  c_location->setCompletion(c_postalcode, code);
  c_country_bcp47->setValue(code.country);
}

void CustomersDataWidget::generateAddressBody() {
  QString _body;
  QStringList _buf;
  /* Company */
  if (c_company->getValue().toBool()) {
    if (!c_company_name->getValue().toString().isEmpty()) {
      _body.append(c_company_name->getValue().toString());
      _body.append("\n");
    }
  }
  /* Titelanrede */
  if (!c_title->getValue().toString().isEmpty()) {
    _buf.append(c_title->getValue().toString());
  }
  /* Vorname */
  if (!c_firstname->getValue().toString().isEmpty()) {
    _buf.append(c_firstname->getValue().toString());
  }
  /* Nachname */
  if (!c_lastname->getValue().toString().isEmpty()) {
    _buf.append(c_lastname->getValue().toString());
  }
  _body.append(_buf.join(" "));
  _body.append("\n");

  /* Wohnhaft bei (care of)  */
  if (!c_careof->getValue().toString().isEmpty()) {
    _body.append("c/o " + c_careof->getValue().toString());
    _body.append("\n");
  }

  /* Straße */
  if (!c_street->getValue().toString().isEmpty()) {
    _body.append(c_street->getValue().toString());
    _body.append("\n");
  }

  _buf.clear();
  /* Postleitzahl */
  if (!c_postalcode->getValue().toString().isEmpty()) {
    _buf.append(c_postalcode->getValue().toString());
  }
  /* Wohnort */
  if (!c_location->getValue().toString().isEmpty()) {
    _buf.append(c_location->getValue().toString());
  }
  _body.append(_buf.join(" "));
  _body.append("\n");

  if (!_body.trimmed().isEmpty()) {
    c_postal_address->setValue(_body);
    registerChanges();
  }
}

void CustomersDataWidget::setCustomerId(qint64 cid) {
  if (cid < 1)
    return;

  customerId = cid;
  emit sendCustomerChanged(cid);
}

qint64 CustomersDataWidget::getCustomerId() {
  return customerId;
}

bool CustomersDataWidget::loadCostumerData(qint64 cid) {
  bool _status = false;
  if (cid > 0) {
    customerId = cid;
  }

  if (customerId < 1) {
    qWarning("CustomerId is not set!");
    return _status;
  }

  QString _scid = QString::number(customerId);
  if (_scid.isEmpty())
    return _status;

  AntiquaCRM::ASqlCore* m_sql = new AntiquaCRM::ASqlCore(this);
  if (m_sql == nullptr)
    return false;

  // PLZ Completers
  c_postalcode->initData();

  // Salutations
  c_title->initData();

  AntiquaCRM::ASqlFiles _tpl("query_customer_data");
  if (!_tpl.openTemplate()) {
    m_sql->deleteLater();
    return _status;
  }

  _tpl.setWhereClause("c_id=" + _scid);

  QSqlQuery _q = m_sql->query(_tpl.getQueryContent());
  if (_q.size() > 0) {
    QSqlRecord _r = _q.record();
    _q.next();

    for (int i = 0; i < _r.count(); i++) {
      QString _name = _r.fieldName(i);
      QVariant _value = _q.value(_name);
      setDataField(_name, _value);
    }

    _status = true;
  }

  QString _co = c_country->getValue().toString();
  if (_co.length() > 3)
    setCountry(_co);

  m_sql->deleteLater();
  return _status;
}

void CustomersDataWidget::setCountry(const QString& country) {
  if (c_postalcode->isValid()) {
    c_postalcode->setCountry(country);
    const QString _str = c_postalcode->getValue().toString();
    c_location->setCompletion(c_postalcode, c_postalcode->getPostalCode(_str));
    registerChanges();
  }
}

} // namespace AntiquaCRM
