// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_CUSTOMERDATA_H
#define INVENTORY_PROVIDERS_CUSTOMERDATA_H

#include <QGroupBox>
#include <QObject>
#include <QWidget>

#include <Utils>

class ProvidersCustomerData final : public QGroupBox {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")
  Q_PROPERTY(int id READ customerId WRITE setCustomerId NOTIFY customerIdChanged)

private:
  int id = 0;
  GenderBox *c_gender;
  LineEdit *c_firstname;
  LineEdit *c_lastname;
  LineEdit *c_location;
  LineEdit *c_street;
  EMailEdit *c_email_0;
  LineEdit *c_country;
  PhoneEdit *c_phone_0;
  PostalCode *c_postalcode;
  SerialID *c_id;
  QLabel *m_info;
  TextField *c_shipping_address;
  TextField *m_customerComment;

private Q_SLOTS:
  void distributeSignals(int customerId);

Q_SIGNALS:
  void customerIdChanged(int);

public Q_SLOTS:
  void setCustomerId(int customerId);

public:
  /**
   * @brief ProvidersCustomerData
   */
  explicit ProvidersCustomerData(QWidget *parent = nullptr);
  int customerId();
  const QVariant getValue(const QString &objName);
};

#endif // INVENTORY_PROVIDERS_CUSTOMERDATA_H
