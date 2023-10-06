// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PROVIDER_IMPORTS_CUSTOMER_EDIT_H
#define PROVIDER_IMPORTS_CUSTOMER_EDIT_H

#include <AntiquaWidgets>
#include <QJsonObject>
#include <QLabel>
#include <QObject>
#include <QSignalMapper>
#include <QToolButton>
#include <QWidget>

class ImportCustomerEdit final : public QWidget {
  Q_OBJECT

private:
  LineEdit *c_provider_import;      // display unique key
  LineEdit *o_firstname;            // origin firstname
  LineEdit *c_firstname;            // edit firstname
  LineEdit *o_lastname;             // origin lastname
  LineEdit *c_lastname;             // edit lastname
  EMailEdit *c_email_0;             // edit email_0
  PhoneEdit *c_phone_0;             // edit phone_0
  LineEdit *o_country;              // origin country
  LineEdit *c_country;              // edit country
  SelectEuCountry *c_country_bcp47; // edit country_bcp47
  LineEdit *o_postalcode;           // origin postalcode
  PostalCodeEdit *c_postalcode;     // edit postalcode
  LineEdit *o_location;             // origin location
  LineEdit *c_location;             // edit location
  LineEdit *o_street;               // origin street
  LineEdit *c_street;               // edit street
  TextField *o_postal_address;      // origin postal_address
  TextField *c_postal_address;      // edit postal_address
  TextField *o_shipping_address;    // origin shipping_address
  TextField *c_shipping_address;    // edit shipping_address
  QSignalMapper *m_mapper;
  QLabel *info(const QString &title);
  QToolButton *mergeButton(const QString &suffix);

private Q_SLOTS:
  void mergeData(const QString &id);

Q_SIGNALS:
  void sendFindClause(const QString &clause);

public:
  explicit ImportCustomerEdit(QWidget *parent = nullptr);
  void setOriginData(const QJsonObject &data);
  void setImportData(const QJsonObject &data);
  const QJsonObject getData();
};

#endif // PROVIDER_IMPORTS_CUSTOMER_EDIT_H
