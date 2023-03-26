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

   QSqlField _str_field; // default string text field
  _str_field.setMetaType(QMetaType(QMetaType::QString));
  _str_field.setRequiredStatus(QSqlField::Required);
  _str_field.setLength(80);

  // AntiquaCRM::GenderEdit
  AntiquaCRM::GenderEdit *_gender = new AntiquaCRM::GenderEdit(w);
  _gender->setBuddyLabel("Gender");
  _gender->setWhatsThisText("Bitte ein geschlecht auswählen.");
  _gender->appendStretch();
  layout->addWidget(_gender);

  // AntiquaCRM::PriceEdit
  AntiquaCRM::PriceEdit *_price = new AntiquaCRM::PriceEdit(w);
  _price->setObjectName("a_sell_price");
  _price->setBuddyLabel("Verkaufspreis");
  _price->setWhatsThisText("Hier den aktuellen Verkaufspreis einfügen.");
  _price->appendStretch();
  _price->setValue(14.56);
  layout->addWidget(_price);

  // AntiquaCRM::CrowdEdit
  AntiquaCRM::CrowdEdit *_crowd = new AntiquaCRM::CrowdEdit(w);
  _crowd->setObjectName("a_count");
  _crowd->setBuddyLabel("Anzahl");
  _crowd->setWhatsThisText("Hier bitte die Artikel bestands menge angeben.");
  _crowd->appendStretch();
  _crowd->setValue(1);
  layout->addWidget(_crowd);

  // AntiquaCRM::EMailEdit
  AntiquaCRM::EMailEdit *_email0 = new AntiquaCRM::EMailEdit("a_mail_0", w);
  _email0->setBuddyLabel("Primary eMail");
  _email0->setWhatsThisText("Standard E-Mail Adresse.");
  _email0->setValue("hans.mustermann@t-online.de");
  _email0->appendStretch();
  layout->addWidget(_email0);

  // AntiquaCRM::PhoneEdit
  AntiquaCRM::PhoneEdit *_phone0 = new AntiquaCRM::PhoneEdit("a_phone_0", w);
  _phone0->setBuddyLabel("Primary Tel.");
  _phone0->setWhatsThisText("Standard Telefonnummer.");
  _phone0->setValue("+49 01234 12345678");
  _phone0->appendStretch();
  layout->addWidget(_phone0);

  // AntiquaCRM::SalutationEdit
  AntiquaCRM::SalutationEdit *_salutation = new AntiquaCRM::SalutationEdit(w);
  _salutation->setBuddyLabel("Salutation");
  _salutation->setWhatsThisText("Have this customer a title?");
  _salutation->appendStretch();
  layout->addWidget(_salutation);

  // AntiquaCRM::PostalCodeEdit
  AntiquaCRM::PostalCodeEdit *_plz = new AntiquaCRM::PostalCodeEdit(w);
  _plz->setObjectName("a_postal_code");
  _plz->setBuddyLabel("PLZ");
  _plz->setWhatsThisText("Add  postalcode here.");
  _plz->appendStretch();
  layout->addWidget(_plz);
  _plz->initData();

  AntiquaCRM::PostalCodeState *_plz_state = new AntiquaCRM::PostalCodeState(w);
  _plz_state->setObjectName("a_state");
  _str_field.setLength(100);
  _plz_state->setRestrictions(_str_field);
  _plz_state->setBuddyLabel("State");
  _plz_state->setWhatsThisText("Country/State");
  _plz_state->appendStretch();
  layout->addWidget(_plz_state);
  w->connect(_plz,
             SIGNAL(sendOnLeavePostalEdit(const AntiquaCRM::PostalCode &)),
             _plz_state, SLOT(setPostalCodes(const AntiquaCRM::PostalCode &)));
  w->connect(_plz, SIGNAL(sendResetDependencies()), _plz_state, SLOT(reset()));

  AntiquaCRM::PostalCodeLocation *_plz_country =
      new AntiquaCRM::PostalCodeLocation(w);
  _plz_country->setObjectName("a_location");
  _str_field.setLength(80);
  _plz_country->setRestrictions(_str_field);
  _plz_country->setBuddyLabel("Location");
  _plz_country->setWhatsThisText("Location");
  _plz_country->appendStretch();
  layout->addWidget(_plz_country);
  w->connect(
      _plz, SIGNAL(sendOnLeavePostalEdit(const AntiquaCRM::PostalCode &)),
      _plz_country, SLOT(setPostalCodes(const AntiquaCRM::PostalCode &)));
  w->connect(_plz, SIGNAL(sendResetDependencies()), _plz_country, SLOT(reset()));

  // AntiquaCRM::ConditionEdit
  AntiquaCRM::ConditionEdit *_condition = new AntiquaCRM::ConditionEdit(w);
  _condition->setObjectName("a_condition");
  _condition->setBuddyLabel("Condition");
  _condition->setWhatsThisText("Condition of the book.");
  _condition->appendStretch();
  layout->addWidget(_condition);

  // AntiquaCRM::BookBindingEdit
  AntiquaCRM::BookBindingEdit *_binding = new AntiquaCRM::BookBindingEdit(w);
  _binding->setObjectName("a_book_binding");
  _binding->setBuddyLabel("Book binding");
  _binding->setWhatsThisText("Bookcover and Binding description.");
  _binding->appendStretch();
  layout->addWidget(_binding);

  layout->addStretch(1);
  QStatusBar *m_statusbar = new QStatusBar(w);
  layout->addWidget(m_statusbar);

  w->setLayout(layout);
  win->setCentralWidget(w);
  win->show();

  // start plz
  m_statusbar->showMessage("add postal data");
  _plz->setCountry("Deutschland");
  _plz->setValue("06343");

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
