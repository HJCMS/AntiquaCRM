// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QtCore>
#include <QtGui>
#include <QtWidgets>

int main(int argc, char *argv[]) {
  QApplication appl(argc, argv);
  appl.setApplicationName(ANTIQUACRM_NAME);
  appl.setApplicationVersion(ANTIQUACRM_VERSION);
  appl.setOrganizationDomain(ANTIQUACRM_CONNECTION_DOMAIN);
  appl.setStyle(QStyleFactory::create("Fusion"));
  QStringList customCSS;
  customCSS << "QTabBar::tab:selected {color: palette(highlight);}";
  customCSS << "QPushButton:hover {color:palette(highlight);}";
  customCSS << "QRadioButton:checked {color:palette(highlight);}";
  customCSS << "QGroupBox::title{padding-right:10px;}";
  appl.setStyleSheet(customCSS.join("\n"));

  QMainWindow *win = new QMainWindow;
  win->setWindowTitle("Tests [*]");
  win->setWindowIcon(QIcon("://icons/antiquacrm.png"));
  win->setMinimumSize(600, 400);

  QWidget *w = new QWidget(win);
  QVBoxLayout *layout = new QVBoxLayout(w);
  layout->setContentsMargins(2, 0, 2, 0);

  // QSqlField _fp; // sql test field
  //_fp.setMetaType(QMetaType(QMetaType::Double));
  //_fp.setRequiredStatus(QSqlField::Required);
  //_fp.setLength(80);
  //_fp.setName("a_sell_price");
  //_fp.setDefaultValue(7.00);

  // AntiquaCRM::GenderEdit
  AntiquaCRM::GenderEdit *_gender = new AntiquaCRM::GenderEdit(w);
  _gender->setBuddyLabel("Gender");
  _gender->addWhatsThisText("Bitte ein geschlecht auswählen.");
  _gender->appendStretch();
  layout->addWidget(_gender);

  // AntiquaCRM::PriceEdit
  AntiquaCRM::PriceEdit *_price = new AntiquaCRM::PriceEdit(w);
  _price->setObjectName("a_sell_price");
  _price->setBuddyLabel("Verkaufspreis");
  _price->addWhatsThisText("Hier den aktuellen Verkaufspreis einfügen.");
  _price->appendStretch();
  _price->setValue(14.56);
  layout->addWidget(_price);

  // AntiquaCRM::CrowdEdit
  AntiquaCRM::CrowdEdit *_crowd = new AntiquaCRM::CrowdEdit(w);
  _crowd->setObjectName("a_count");
  _crowd->setBuddyLabel("Anzahl");
  _crowd->addWhatsThisText("Hier bitte die Artikel bestands menge angeben.");
  _crowd->appendStretch();
  _crowd->setValue(1);
  layout->addWidget(_crowd);

  // AntiquaCRM::EMailEdit
  AntiquaCRM::EMailEdit *_email0 = new AntiquaCRM::EMailEdit("a_mail_0", w);
  _email0->setBuddyLabel("Primary eMail");
  _email0->addWhatsThisText("Standard E-Mail Adresse.");
  _email0->setValue("hans.mustermann@t-online.de");
  _email0->appendStretch();
  layout->addWidget(_email0);

  // AntiquaCRM::PhoneEdit
  AntiquaCRM::PhoneEdit *_phone0 = new AntiquaCRM::PhoneEdit("a_phone_0", w);
  _phone0->setBuddyLabel("Primary Tel.");
  _phone0->addWhatsThisText("Standard Telefonnummer.");
  _phone0->setValue("+49 01234 12345678");
  _phone0->appendStretch();
  layout->addWidget(_phone0);

  // AntiquaCRM::SalutationEdit
  AntiquaCRM::SalutationEdit *_salutation = new AntiquaCRM::SalutationEdit(w);
  _salutation->setBuddyLabel("Salutation");
  _salutation->addWhatsThisText("Have this customer a title?");
  _salutation->appendStretch();
  layout->addWidget(_salutation);

  // AntiquaCRM::PostalCodeEdit
  AntiquaCRM::PostalCodeEdit *_plz = new AntiquaCRM::PostalCodeEdit(w);
  _plz->setObjectName("a_postal_code");
  _plz->setBuddyLabel("PLZ");
  _plz->addWhatsThisText("Add  postalcode here.");
  _plz->appendStretch();
  layout->addWidget(_plz);

  AntiquaCRM::PostalCodeState *_plz_state = new AntiquaCRM::PostalCodeState(w);
  _plz_state->setObjectName("a_state");
  _plz_state->setBuddyLabel("State");
  layout->addWidget(_plz_state);
  w->connect(_plz,
             SIGNAL(sendOnLeavePostalEdit(const AntiquaCRM::PostalCode &)),
             _plz_state, SLOT(setPostalCodes(const AntiquaCRM::PostalCode &)));
  w->connect(_plz, SIGNAL(sendResetDependencies()), _plz_state, SLOT(reset()));

  AntiquaCRM::PostalCodeCountry *_plz_country =
      new AntiquaCRM::PostalCodeCountry(w);
  _plz_country->setObjectName("a_location");
  _plz_country->setBuddyLabel("Country");
  layout->addWidget(_plz_country);
  w->connect(
      _plz, SIGNAL(sendOnLeavePostalEdit(const AntiquaCRM::PostalCode &)),
      _plz_country, SLOT(setPostalCodes(const AntiquaCRM::PostalCode &)));
  w->connect(_plz, SIGNAL(sendResetDependencies()), _plz_country, SLOT(reset()));

  // start plz
  _plz->initData();
  _plz->setCountry("Deutschland");
  _plz->setValue("06343");

  layout->addStretch(1);
  QStatusBar *m_statusbar = new QStatusBar(w);
  layout->addWidget(m_statusbar);

  w->setLayout(layout);
  win->setCentralWidget(w);
  win->show();

  m_statusbar->showMessage("initialed");

  QRegularExpression pattern("^[a-z]{1}_[a-z_]{2,}");
  QList<AntiquaCRM::AbstractInput *> list =
      w->findChildren<AntiquaCRM::AbstractInput *>(pattern);
  for (int i = 0; i < list.size(); i++) {
    AntiquaCRM::AbstractInput *obj = list.at(i);
    if (obj != nullptr) {
      qDebug() << obj->objectName() << obj->getValue();
    }
  }
  return appl.exec();
}
