// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_COSTUMERSDATA_H
#define ANTIQUACRM_COSTUMERSDATA_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

/**
 * @brief Customers Editor data widget
 * @ingroup _customers
 */
class ANTIQUACRM_LIBRARY CustomersData final : public QWidget {
  Q_OBJECT

private Q_SLOTS:
  /**
   * @brief Search for European country bindings!
   * This will change the European country selecter.
   * A Option to find the right Shipping-TAX.
   */
  void setPostalData(const AntiquaCRM::PostalCode &);

  /**
   * @brief When Generate button is clicked.
   * Create the invoice header for the customer.
   */
  void generateAddressBody();

public:
  AntiquaCRM::GenderEdit *c_gender;             /**< Gender */
  AntiquaCRM::SalutationEdit *c_title;          /**< Salutation */
  AntiquaCRM::TextLine *c_firstname;            /**< Firstname */
  AntiquaCRM::TextLine *c_lastname;             /**< Lastname */
  AntiquaCRM::TextLine *c_careof;               /**< carry of */
  AntiquaCRM::PostalCodeEdit *c_postalcode;     /**< Postalcode */
  AntiquaCRM::PostalCodeState *c_country;       /**< Country */
  AntiquaCRM::PostalCodeLocation *c_location;   /**< Location */
  AntiquaCRM::SelectEUCountry *c_country_bcp47; /**< IETF BCP 47 */
  AntiquaCRM::TextLine *c_street;               /**< Street */
  AntiquaCRM::PhoneEdit *c_phone_0;             /**< Phone number 1 */
  AntiquaCRM::PhoneEdit *c_phone_1;             /**< Phone number 2 */
  AntiquaCRM::PhoneEdit *c_mobil_0;             /**< Mobil number 1 */
  AntiquaCRM::PhoneEdit *c_mobil_1;             /**< Mobil number 2 */
  AntiquaCRM::PhoneEdit *c_fax_0;               /**< Fax number */
  AntiquaCRM::EMailEdit *c_email_0;             /**< E-Mail Address */
  AntiquaCRM::EMailEdit *c_email_1;             /**< E-Mail Address */
  AntiquaCRM::TextLine *c_website;              /**< Webpage */
  AntiquaCRM::GroupBoxEdit *c_company;       /**< Company, Institut, Orga.. ? */
  AntiquaCRM::TextLine *c_company_name;      /**< Company name */
  AntiquaCRM::TextLine *c_company_employer;  /**< Company employer */
  AntiquaCRM::TextField *c_postal_address;   /**< Invoice Address */
  AntiquaCRM::TextField *c_shipping_address; /**< Shipping Address */

  /**
   * @param parent - parent object
   */
  explicit CustomersData(QWidget *parent = nullptr);

  /**
   * @brief Set country in PostalCode-Selecter and modify c_country if needed.
   * @param country - Fullname or ISO-639-1
   */
  void setCountry(const QString &country);
};

#endif // ANTIQUACRM_COSTUMERSDATA_H
