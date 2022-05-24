// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_CUSTOMERDATA_H
#define INVENTORY_PROVIDERS_CUSTOMERDATA_H

#include <QObject>
#include <QGroupBox>
#include <QWidget>

#include <Utils>

class ProvidersCustomerData : public QGroupBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  GenderBox *c_gender;
  LineEdit *c_firstname;
  LineEdit *c_lastname;
  LineEdit *c_location;
  LineEdit *c_street;
  EMailEdit *c_email_0;
  LineEdit *c_country;
  SerialID *c_id;
  PhoneEdit *c_phone_0;
  PostalCode *c_postalcode;

public:
  explicit ProvidersCustomerData(QWidget *parent = nullptr);
};

#endif // INVENTORY_PROVIDERS_CUSTOMERDATA_H
