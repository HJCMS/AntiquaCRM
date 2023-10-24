// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "importrepairedit.h"

ImportRepairEdit::ImportRepairEdit(QWidget *parent) : QScrollArea{parent} {
  setWidgetResizable(true);

  m_widget = new QWidget(this);
  m_widget->setObjectName("data_diff_widget");
  m_widget->setContentsMargins(2, 2, 2, 2);
  setWidget(m_widget);

  qint8 row = 0;
  QGridLayout *layout = new QGridLayout(m_widget);
  layout->setContentsMargins(0, 0, 0, 0);

  m_mapper = new QSignalMapper(m_widget);

  // Begin::provider_import
  layout->addWidget(info(tr("Unique Key")), row, 0, 1, 1);
  c_provider_import = new AntiquaCRM::TextLine(m_widget);
  c_provider_import->setObjectName("c_provider_import");
  c_provider_import->setReadOnly(true);
  c_provider_import->setToolTip(tr("Read only"));
  layout->addWidget(c_provider_import, row++, 1, 1, 4);
  // End::provider_import

  // Begin::firstname
  layout->addWidget(info(tr("Firstname")), row, 0, 1, 1);
  o_firstname = new AntiquaCRM::TextLine(m_widget);
  o_firstname->setObjectName("o_firstname");
  layout->addWidget(o_firstname, row, 1, 1, 1);
  layout->addWidget(mergeButton("firstname"), row, 2, 1, 1);
  c_firstname = new AntiquaCRM::TextLine(m_widget);
  c_firstname->setObjectName("c_firstname");
  layout->addWidget(c_firstname, row++, 3, 1, 1);
  // End::firstname

  // Begin::lastname
  layout->addWidget(info(tr("Lastname")), row, 0, 1, 1);
  o_lastname = new AntiquaCRM::TextLine(m_widget);
  o_lastname->setObjectName("o_lastname");
  layout->addWidget(o_lastname, row, 1, 1, 1);
  layout->addWidget(mergeButton("lastname"), row, 2, 1, 1);
  c_lastname = new AntiquaCRM::TextLine(m_widget);
  c_lastname->setObjectName("c_lastname");
  layout->addWidget(c_lastname, row++, 3, 1, 1);
  // End::lastname

  // Begin::email_0
  layout->addWidget(info(tr("eMail")), row, 0, 1, 3);
  c_email_0 = new AntiquaCRM::TextLine(m_widget);
  c_email_0->setObjectName("c_email_0");
  layout->addWidget(c_email_0, row++, 3, 1, 1);
  // End::email_0

  // Begin::phone_0
  layout->addWidget(info(tr("Phone")), row, 0, 1, 3);
  c_phone_0 = new AntiquaCRM::PhoneEdit(m_widget);
  c_phone_0->setObjectName("c_phone_0");
  layout->addWidget(c_phone_0, row++, 3, 1, 1);
  // End::phone_0

  // Begin::country
  layout->addWidget(info(tr("Country")), row, 0, 1, 1);
  o_country = new AntiquaCRM::TextLine(m_widget);
  o_country->setObjectName("o_country");
  layout->addWidget(o_country, row, 1, 1, 1);
  layout->addWidget(mergeButton("country"), row, 2, 1, 1);
  c_country = new AntiquaCRM::TextLine(m_widget);
  c_country->setObjectName("c_country");
  layout->addWidget(c_country, row++, 3, 1, 1);
  // End::country

  // Begin::country_bcp47
  c_country_bcp47 = new AntiquaCRM::SelectEUCountry(m_widget);
  c_country_bcp47->setObjectName("c_country_bcp47");
  layout->addWidget(c_country_bcp47, row++, 3, 1, 1);
  // End::country_bcp47

  // Begin::postalcode
  layout->addWidget(info(tr("Postalcode")), row, 0, 1, 1);
  o_postalcode = new AntiquaCRM::TextLine(m_widget);
  o_postalcode->setObjectName("o_postalcode");
  layout->addWidget(o_postalcode, row, 1, 1, 1);
  layout->addWidget(mergeButton("postalcode"), row, 2, 1, 1);
  c_postalcode = new AntiquaCRM::PostalCodeEdit(this);
  c_postalcode->setObjectName("c_postalcode");
  layout->addWidget(c_postalcode, row++, 3, 1, 1);
  // End::postalcode

  // Begin::location
  layout->addWidget(info(tr("Location")), row, 0, 1, 1);
  o_location = new AntiquaCRM::TextLine(m_widget);
  o_location->setObjectName("o_location");
  layout->addWidget(o_location, row, 1, 1, 1);
  layout->addWidget(mergeButton("location"), row, 2, 1, 1);
  c_location = new AntiquaCRM::TextLine(m_widget);
  c_location->setObjectName("c_location");
  layout->addWidget(c_location, row++, 3, 1, 1);
  // End::location

  // Begin::street
  layout->addWidget(info(tr("Street")), row, 0, 1, 1);
  o_street = new AntiquaCRM::TextLine(m_widget);
  o_street->setObjectName("o_street");
  layout->addWidget(o_street, row, 1, 1, 1);
  layout->addWidget(mergeButton("street"), row, 2, 1, 1);
  c_street = new AntiquaCRM::TextLine(m_widget);
  c_street->setObjectName("c_street");
  layout->addWidget(c_street, row++, 3, 1, 1);
  // End::street

  // Begin::Invoice Address
  layout->addWidget(info(tr("Invoice Address")), row, 0, 1, 1);
  o_postal_address = new AntiquaCRM::TextField(m_widget);
  o_postal_address->setObjectName("o_postal_address");
  o_postal_address->setMinimumHeight(100);
  layout->addWidget(o_postal_address, row, 1, 1, 1);
  layout->addWidget(mergeButton("postal_address"), row, 2, 1, 1);
  c_postal_address = new AntiquaCRM::TextField(m_widget);
  c_postal_address->setObjectName("c_postal_address");
  c_postal_address->setMinimumHeight(100);
  layout->addWidget(c_postal_address, row++, 3, 1, 1);
  // End::Invoice Address

  // Begin::Delivery Address
  layout->addWidget(info(tr("Delivery Address")), row, 0, 1, 1);
  o_shipping_address = new AntiquaCRM::TextField(m_widget);
  o_shipping_address->setObjectName("o_shipping_address");
  o_shipping_address->setMinimumHeight(100);
  layout->addWidget(o_shipping_address, row, 1, 1, 1);
  layout->addWidget(mergeButton("shipping_address"), row, 2, 1, 1);
  c_shipping_address = new AntiquaCRM::TextField(m_widget);
  c_shipping_address->setObjectName("c_shipping_address");
  c_shipping_address->setMinimumHeight(100);
  layout->addWidget(c_shipping_address, row++, 3, 1, 1);
  // End::Delivery Address

  layout->setRowStretch(row, 1);
  m_widget->setLayout(layout);

  connect(m_mapper, SIGNAL(mappedString(const QString &)),
          SLOT(mergeData(const QString &)));
}

QLabel *ImportRepairEdit::info(const QString &title) {
  QLabel *lb = new QLabel(title, this);
  lb->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  return lb;
}

QToolButton *ImportRepairEdit::mergeButton(const QString &suffix) {
  QToolButton *btn = new QToolButton(this);
  btn->setObjectName(suffix);
  btn->setArrowType(Qt::RightArrow);
  btn->setToolTip(tr("merge data"));
  connect(btn, SIGNAL(clicked()), m_mapper, SLOT(map()));
  m_mapper->setMapping(btn, suffix);
  return btn;
}

void ImportRepairEdit::mergeData(const QString &id) {
  AntiquaCRM::AInputWidget *m_f =
      m_widget->findChild<AntiquaCRM::AInputWidget *>("o_" + id);
  if (m_f == nullptr) // from
    return;

  AntiquaCRM::AInputWidget *m_t =
      m_widget->findChild<AntiquaCRM::AInputWidget *>("c_" + id);
  if (m_t == nullptr) // to
    return;

  m_t->setValue(m_f->getValue());
}

void ImportRepairEdit::setOriginData(const QJsonObject &data) {
  foreach (const QString _key, data.keys()) {
    QString _inp = QString(_key).replace("c_", "o_");
    AntiquaCRM::AInputWidget *e =
        m_widget->findChild<AntiquaCRM::AInputWidget *>(_inp);
    if (e == nullptr)
      continue;

    e->setValue(data.value(_key).toString());
    e->setEnabled(false);
  }
}

void ImportRepairEdit::setImportData(const QJsonObject &data) {
  c_postalcode->initData();

  foreach (const QString _key, data.keys()) {
    AntiquaCRM::AInputWidget *e =
        m_widget->findChild<AntiquaCRM::AInputWidget *>(_key);
    if (e == nullptr)
      continue;

    e->setValue(data.value(_key).toString());

    if (_key == "c_country")
      c_postalcode->setCountry(e->getValue().toString());
  }
  c_provider_import->setReadOnly(true);

  QString _clause("(c_firstname ILIKE '");
  _clause.append(c_firstname->getValue().toString());
  _clause.append("' AND c_lastname ILIKE '");
  _clause.append(c_lastname->getValue().toString());
  _clause.append("')");
  emit sendFindClause(_clause);
}

const QJsonObject ImportRepairEdit::getData() {
  QJsonObject _json;
  QListIterator<AntiquaCRM::AInputWidget *> it(
      m_widget->findChildren<AntiquaCRM::AInputWidget *>(QString()));
  while (it.hasNext()) {
    AntiquaCRM::AInputWidget *e = it.next();
    if (e == nullptr)
      continue;

    _json.insert(e->objectName(), e->getValue().toJsonValue());
  }
  return _json;
}
