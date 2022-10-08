// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "acustomerdata.h"

namespace AntiquaCRM {

ACustomer::ACustomer(qint64 customerId) : p_cId(customerId) {}

void ACustomer::setCustomerId(qint64 cId) { p_cId = cId; }

qint64 ACustomer::getCustomerId() { return p_cId; }

void ACustomer::setLocked(bool locked) { p_locked = locked; }

bool ACustomer::isLocked() { return p_locked; }

void ACustomer::setTrusted(AntiquaCRM::CustomerTrustLevel trust) {
  p_trusted = trust;
}

AntiquaCRM::CustomerTrustLevel ACustomer::getTrusted() { return p_trusted; }

void ACustomer::setTransactions(int count) { p_transactions = count; }

int ACustomer::getTransactions() { return p_transactions; }

/*
void ACustomer::setPurchases(QList<AntiquaCRM::Payments> &purchases) {
  p_purchases = purchases;
}
QList<AntiquaCRM::Payments &> ACustomer::getPurchases() {
  return p_purchases;
}
*/

void ACustomer::setCompany(bool company) { p_company = company; }

bool ACustomer::isCompany() { return p_company; }

void ACustomer::setCompanyName(const QString &company) {
  p_company_name = company;
}

const QString ACustomer::getCompanyName() { return p_company_name; }

void ACustomer::setCompanyEmployer(const QString &employer) {
  p_company_employer = employer;
}

const QString ACustomer::getCompanyEmployer() { return p_company_employer; }

void ACustomer::setGender(AntiquaCRM::Gender gender) { p_gender = gender; }

AntiquaCRM::Gender ACustomer::getGender() { return p_gender; }

void ACustomer::setSalutation(const QString &salutation) {
  p_salutation = salutation;
}

const QString ACustomer::getSalutation() { return p_salutation; }

void ACustomer::setFirstname(const QString &firstname) {
  p_firstname = firstname;
}

const QString ACustomer::getFirstname() { return p_firstname; }

void ACustomer::setLastname(const QString &lastname) { p_lastname = lastname; }

const QString ACustomer::getLastname() { return p_lastname; }

void ACustomer::setPostalcode(const QString &postalcode) {
  p_postalcode = postalcode;
}

const QString ACustomer::getPostalcode() { return p_postalcode; }

void ACustomer::setCountry(const QString &country) { p_country = country; }

const QString ACustomer::getCountry() { return p_country; }

void ACustomer::setLocation(const QString &location) { p_location = location; }

const QString ACustomer::getLocation() { return p_location; }

void ACustomer::setStreet(const QString &street) { p_street = street; }

const QString ACustomer::getStreet() { return p_street; }

void ACustomer::setEmail(const QString &email, int index) {
  if (index > 0)
    p_email_1 = email;
  else
    p_email_0 = email;
}

const QString ACustomer::getEmail(int index) {
  if (index > 0)
    return p_email_1;

  return p_email_0;
}

void ACustomer::setShippingEMail(const QString &email) {
  p_shipping_email = email;
}

const QString ACustomer::getShippingEMail() { return p_shipping_email; }

void ACustomer::setWebsite(const QString &website) { p_website = website; }

const QString ACustomer::getWebsite() { return p_website; }

void ACustomer::setPhone(const QString &phone, int index) {
  if (index > 0)
    p_phone_1 = phone;
  else
    p_phone_0 = phone;
}

const QString ACustomer::getPhone(int index) {
  if (index > 0)
    return p_phone_1;

  return p_phone_0;
}

void ACustomer::setMobilPhone(const QString &mobil, int index) {
  if (index > 0)
    p_mobil_1 = mobil;
  else
    p_mobil_0 = mobil;
}

const QString ACustomer::getMobilPhone(int index) {
  if (index > 0)
    return p_mobil_1;

  return p_mobil_0;
}

void ACustomer::setFax(const QString &fax, int index) {
  if (index > 0)
    p_fax_1 = fax;
  else
    p_fax_0 = fax;
}

const QString ACustomer::getFax(int index) {
  if (index > 0)
    return p_fax_1;

  return p_fax_0;
}

void ACustomer::setPostalAddress(const QString &address) {
  p_postal_address = address;
}

const QString ACustomer::getPostalAddress() { return p_postal_address; }

void ACustomer::setShippingAddress(const QString &address) {
  p_shipping_address = address;
}

const QString ACustomer::getShippingAddress() { return p_shipping_address; }

void ACustomer::setComments(const QString &comments) { p_comments = comments; }

const QString ACustomer::getComments() { return p_comments; }

void ACustomer::setIBAN(const QString &iban) { p_iban = iban; }

const QString ACustomer::getIBAN() { return p_iban; }

void ACustomer::setSwiftBic(const QString &swiftbic) { p_swift_bic = swiftbic; }

const QString ACustomer::getSwiftBic() { return p_swift_bic; }

void ACustomer::setTax(const QString &tax) { p_tax_id = tax; }

const QString ACustomer::getTax() { return p_tax_id; }

void ACustomer::setSince(const QDateTime &since) { p_since = since; }

const QDateTime ACustomer::getSince() { return p_since; }

void ACustomer::setLastChanged(const QDateTime &changed) {
  p_changed = changed;
}

const QDateTime ACustomer::getLastChanged() { return p_changed; }

void ACustomer::setCountryCode(const QString &bcp47) {
  p_country_bcp47 = bcp47;
}

const QString ACustomer::getCountryCode() { return p_country_bcp47; }

void ACustomer::setUniqImportKey(const QString &key) {
  p_uniq_import_key = key;
}

const QString ACustomer::getUniqImportKey() { return p_uniq_import_key; }

}; // namespace AntiquaCRM
