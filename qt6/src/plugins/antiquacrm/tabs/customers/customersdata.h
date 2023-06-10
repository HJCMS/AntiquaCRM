// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

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
class ANTIQUACRM_LIBRARY CustomersData final : public QWidget {
  Q_OBJECT

public:
  AntiquaCRM::GenderEdit *c_gender;             /**< Geschlecht */
  AntiquaCRM::SalutationEdit *c_title;          /**< Titelanrede */
  AntiquaCRM::TextLine *c_firstname;            /**< Vorname */
  AntiquaCRM::TextLine *c_lastname;             /**< Nachname */
  AntiquaCRM::TextLine *c_careof;               /**< Wohnhaft bei */
  AntiquaCRM::PostalCodeEdit *c_postalcode;     /**< Postleitzahl */
  AntiquaCRM::PostalCodeState *c_country;       /**< Land */
  AntiquaCRM::PostalCodeLocation *c_location;   /**< Wohnort */
  AntiquaCRM::SelectEUCountry *c_country_bcp47; /**< IETF BCP 47 */
  AntiquaCRM::TextLine *c_street;               /**< Straße */
  AntiquaCRM::PhoneEdit *c_phone_0;             /**< Telefon Nummer 1 */
  AntiquaCRM::PhoneEdit *c_phone_1;             /**< Telefon Nummer 2 */
  AntiquaCRM::PhoneEdit *c_mobil_0;             /**< Mobilerufnummer 1 */
  AntiquaCRM::PhoneEdit *c_mobil_1;             /**< Mobilerufnummer 2 */
  AntiquaCRM::PhoneEdit *c_fax_0;               /**< Faxnummer */
  AntiquaCRM::EMailEdit *c_email_0;             /**< E-Mail Adresse */
  AntiquaCRM::EMailEdit *c_email_1;             /**< E-Mail Adresse */
  AntiquaCRM::TextLine *c_website;              /**< Webseite */
  AntiquaCRM::GroupBoxEdit *c_company;  /**< Firma, Institut, Organisation ? */
  AntiquaCRM::TextLine *c_company_name; /**< Firmen Name */
  AntiquaCRM::TextLine *c_company_employer;  /**< Ansprechpartner */
  AntiquaCRM::TextField *c_postal_address;   /**< Adresse */
  AntiquaCRM::TextField *c_shipping_address; /**< Lieferadresse */

  /**
   * @brief CustomersData
   */
  explicit CustomersData(QWidget *parent = nullptr);

  /**
   * @brief Find country in Selecter and modify c_country if needed.
   * @param country
   */
  void setCountry(const QString &country);

private Q_SLOTS:
  /**
   * @brief Suche nach EU Staaten angehörigkeit!
   */
  void fetchEuropeanCountry(const AntiquaCRM::PostalCode &);

  /**
   * @brief Wenn Knopf generieren geklickt wird.
   * Den Rechnungskopf für den Kunden erstellen.
   */
  void generateAddressBody();
};

#endif // ANTIQUACRM_COSTUMERSDATA_H
