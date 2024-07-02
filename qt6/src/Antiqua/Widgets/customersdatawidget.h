// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_CUSTOMERSDATAWIDGET_H
#define ANTIQUACRM_WIDGETS_CUSTOMERSDATAWIDGET_H

#include <AntiquaInput>
#include <QObject>
#include <QWidget>

namespace AntiquaCRM
{

class GenderEdit;
class GroupBoxEdit;
class PhoneEdit;
class PostalCodeEdit;
class PostalCodeLocation;
class PostalCodeState;
class SalutationEdit;
class SelectEUCountry;
class TextField;
class TextLine;
class URILineEdit;

/**
 * @brief Customers Editor data widget
 * @ingroup EditWidgets
 */
class ANTIQUACRM_LIBRARY CustomersDataWidget final : public QWidget {
  Q_OBJECT
  Q_PROPERTY(qint64 customerId READ getCustomerId WRITE setCustomerId NOTIFY sendCustomerChanged)

private:
  /**
   * @brief optional customer id
   *
   * Only used when setCustomerById is used!
   * This is a non standard value and normally exists in parent classes!
   */
  qint64 customerId = -1;

  /**
   * @brief Set data with object name
   * @param name - ObjectName
   * @param data - Data
   */
  inline void setDataField(const QString& name, const QVariant& data);

private Q_SLOTS:
  /**
   * @brief Summary changed
   */
  void registerChanges();

  /**
   * @brief Search for European country bindings!
   * This will change the European country selecter.
   * A Option to find the right Shipping-TAX.
   */
  void setPostalData(const AntiquaCRM::PostalCode&);

  /**
   * @brief When Generate button is clicked.
   * Create the invoice header for the customer.
   */
  void generateAddressBody();

Q_SIGNALS:
  void sendCustomerChanged(qint64);

public Q_SLOTS:
  void setCustomerId(qint64);

public:
  GenderEdit* c_gender;             /**< Gender */
  SalutationEdit* c_title;          /**< Salutation */
  TextLine* c_firstname;            /**< Firstname */
  TextLine* c_lastname;             /**< Lastname */
  TextLine* c_careof;               /**< carry of */
  PostalCodeEdit* c_postalcode;     /**< Postalcode */
  PostalCodeState* c_country;       /**< Country */
  PostalCodeLocation* c_location;   /**< Location */
  SelectEUCountry* c_country_bcp47; /**< IETF BCP 47 */
  TextLine* c_street;               /**< Street */
  PhoneEdit* c_phone_0;             /**< Phone number 1 */
  PhoneEdit* c_phone_1;             /**< Phone number 2 */
  PhoneEdit* c_mobil_0;             /**< Mobil number 1 */
  PhoneEdit* c_mobil_1;             /**< Mobil number 2 */
  PhoneEdit* c_fax_0;               /**< Fax number */
  URILineEdit* c_email_0;           /**< E-Mail Address */
  URILineEdit* c_email_1;           /**< E-Mail Address */
  URILineEdit* c_website;           /**< Webpage */
  GroupBoxEdit* c_company;          /**< Company, Institut, Orga.. ? */
  TextLine* c_company_name;         /**< Company name */
  TextLine* c_company_employer;     /**< Company employer */
  TextField* c_postal_address;      /**< Invoice Address */
  TextField* c_shipping_address;    /**< Shipping Address */

  /**
   * @param parent - parent object
   */
  explicit CustomersDataWidget(QWidget* parent = nullptr);

  qint64 getCustomerId();

  /**
   * @brief load customer data with id
   * @param cid - customer id
   *
   * This will create a sql connection from current environment.
   * If \i cid is null, the \b customerId property is used.
   * When \b customerId is lower then 1 nothing will done.
   *
   * This function returning \b false, when \b customerId property is not set
   * or no result exists!
   */
  bool loadCostumerData(qint64 cid = 0);

  /**
   * @brief Set country in PostalCode-Selecter and modify c_country if needed.
   * @param country - Fullname or ISO-639-1
   */
  void setCountry(const QString& country);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_CUSTOMERSDATAWIDGET_H
