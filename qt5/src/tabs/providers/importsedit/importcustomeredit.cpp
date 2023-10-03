// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "importcustomeredit.h"

#include <QDebug>
#include <QGridLayout>
#include <QJsonValue>

ImportCustomerEdit::ImportCustomerEdit(QWidget *parent) : QWidget{parent} {
  setContentsMargins(2, 2, 2, 2);

  qint8 row = 0;
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  layout->addWidget(info(tr("Firstname")), row, 0, 1, 1);
  c_firstname = new LineEdit(this);
  c_firstname->setObjectName("c_firstname");
  layout->addWidget(c_firstname, row, 1, 1, 1);

  layout->addWidget(info(tr("Lastname")), row, 2, 1, 1);
  c_lastname = new LineEdit(this);
  c_lastname->setObjectName("c_lastname");
  layout->addWidget(c_lastname, row++, 3, 1, 1);

  layout->addWidget(info(tr("eMail")), row, 0, 1, 1);
  c_email_0 = new EMailEdit(this);
  c_email_0->setObjectName("c_email_0");
  layout->addWidget(c_email_0, row, 1, 1, 1);

  layout->addWidget(info(tr("Phone")), row, 2, 1, 1);
  c_phone_0 = new PhoneEdit(this);
  c_phone_0->setObjectName("c_phone_0");
  layout->addWidget(c_phone_0, row++, 3, 1, 1);

  layout->addWidget(info(tr("Country")), row, 0, 1, 1);
  c_country = new LineEdit(this);
  c_country->setObjectName("c_country");
  layout->addWidget(c_country, row, 1, 1, 1);

  c_country_bcp47 = new SelectEuCountry(this);
  c_country_bcp47->setObjectName("c_country_bcp47");
  layout->addWidget(c_country_bcp47, row++, 2, 1, 2);

  layout->addWidget(info(tr("Postalcode")), row, 0, 1, 1);
  c_postalcode = new PostalCodeEdit(this);
  c_postalcode->setObjectName("c_postalcode");
  layout->addWidget(c_postalcode, row, 1, 1, 1);

  layout->addWidget(info(tr("Location")), row, 2, 1, 1);
  c_location = new LineEdit(this);
  c_location->setObjectName("c_location");
  layout->addWidget(c_location, row++, 3, 1, 1);

  layout->addWidget(info(tr("Street")), row, 0, 1, 1);
  c_street = new LineEdit(this);
  c_street->setObjectName("c_street");
  layout->addWidget(c_street, row, 1, 1, 1);

  layout->addWidget(info(tr("Key")), row, 2, 1, 1);
  c_provider_import = new LineEdit(this);
  c_provider_import->setObjectName("c_provider_import");
  c_provider_import->setReadOnly(true);
  c_provider_import->setToolTip(tr("Read only"));
  layout->addWidget(c_provider_import, row++, 3, 1, 1);

  layout->addWidget(info(tr("Invoice Address")), row, 0, 1, 2);
  layout->addWidget(info(tr("Delivery Address")), row++, 2, 1, 2);

  c_postal_address = new TextField(this);
  c_postal_address->setObjectName("c_postal_address");
  layout->addWidget(c_postal_address, row, 0, 1, 2);

  c_shipping_address = new TextField(this);
  c_shipping_address->setObjectName("c_shipping_address");
  layout->addWidget(c_shipping_address, row++, 2, 1, 2);

  layout->setRowStretch(row, 1);
  setLayout(layout);
}

QLabel *ImportCustomerEdit::info(const QString &title) {
  QLabel *lb = new QLabel(title, this);
  lb->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  return lb;
}

void ImportCustomerEdit::setData(const QJsonObject &data) {
  c_postalcode->loadDataset();

  foreach (QString _key, data.keys()) {
    InputEdit *e = findChild<InputEdit *>(_key);
    if (e == nullptr)
      continue;

    e->setValue(data.value(_key).toString());

    if (_key == "c_country")
      c_postalcode->findCountry(e->value().toString());
  }
  c_provider_import->setReadOnly(true);
}

const QJsonObject ImportCustomerEdit::getData() {
  QJsonObject _json;
  QListIterator<InputEdit *> it(findChildren<InputEdit *>(QString()));
  while (it.hasNext()) {
    InputEdit *e = it.next();
    if (e == nullptr)
      continue;

    _json.insert(e->objectName(), e->value().toJsonValue());
  }
  return _json;
}
