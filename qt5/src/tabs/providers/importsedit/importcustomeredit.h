// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PROVIDER_IMPORTS_CUSTOMER_EDIT_H
#define PROVIDER_IMPORTS_CUSTOMER_EDIT_H

#include <AntiquaWidgets>
#include <QJsonObject>
#include <QLabel>
#include <QObject>
#include <QWidget>

class ImportCustomerEdit final : public QWidget {
  Q_OBJECT

private:
  LineEdit *c_firstname;
  LineEdit *c_lastname;
  EMailEdit *c_email_0;
  PhoneEdit *c_phone_0;
  LineEdit *c_country;
  SelectEuCountry *c_country_bcp47;
  PostalCodeEdit *c_postalcode;
  LineEdit *c_location;
  LineEdit *c_street;
  LineEdit *c_provider_import;
  TextField *c_postal_address;
  TextField *c_shipping_address;
  QLabel *info(const QString &title);

public:
  explicit ImportCustomerEdit(QWidget *parent = nullptr);
  void setData(const QJsonObject &data);
  const QJsonObject getData();
};

#endif // PROVIDER_IMPORTS_CUSTOMER_EDIT_H
