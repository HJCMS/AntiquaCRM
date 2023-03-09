// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_COSTUMERSDATA_H
#define ANTIQUACRM_COSTUMERSDATA_H

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

/**
 * @brief Customers Editor data widget
 * @ingroup _customers
 */
class CustomersData final : public QWidget {
  Q_OBJECT

public:
  GenderBox *c_gender;              /**< Geschlecht */
  SalutationBox *c_title;           /**< Titelanrede */
  LineEdit *c_firstname;            /**< Vorname */
  LineEdit *c_lastname;             /**< Nachname */
  LineEdit *c_careof;               /**< Wohnhaft bei */
  PostalCodeEdit *c_postalcode;     /**< Postleitzahl */
  LineEdit *c_country;              /**< Land */
  SelectEuCountry *c_country_bcp47; /**< IETF BCP 47 */
  LineEdit *c_location;             /**< Wohnort */
  LineEdit *c_street;               /**< Straße */
  PhoneEdit *c_phone_0;             /**< Telefon Nummer 1 */
  PhoneEdit *c_phone_1;             /**< Telefon Nummer 2 */
  PhoneEdit *c_mobil_0;             /**< Mobilerufnummer 1 */
  PhoneEdit *c_mobil_1;             /**< Mobilerufnummer 2 */
  PhoneEdit *c_fax_0;               /**< Faxnummer */
  EMailEdit *c_email_0;             /**< E-Mail Adresse */
  EMailEdit *c_email_1;             /**< E-Mail Adresse */
  LineEdit *c_website;              /**< Webseite */
  GroupBox *c_company;              /**< Firma/Institut oder Organisation ? */
  LineEdit *c_company_name;         /**< Firmen Name */
  LineEdit *c_company_employer;     /**< Ansprechpartner */
  TextField *c_postal_address;      /**< Adresse */
  TextField *c_shipping_address;    /**< Lieferadresse */

  /**
   * @brief CustomersData
   */
  explicit CustomersData(QWidget *parent = nullptr);

private Q_SLOTS:
  /**
   * @brief Wenn von c_postalcode ein editingFinished absetzt wird!
   */
  void postalCodeComplite(const AntiquaCRM::PostalCode &);

  /**
   * @brief Wenn Knopf generieren geklickt wird.
   * Den Rechnungskopf für den Kunden erstellen.
   */
  void generateAddressBody();
};

#endif // ANTIQUACRM_COSTUMERSDATA_H
