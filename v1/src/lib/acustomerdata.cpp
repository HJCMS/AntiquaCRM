// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "acustomerdata.h"

namespace AntiquaCRM {

ACustomer::ACustomer(qint64 customerId) : p_cId(customerId) {}

const QString ACustomer::burnish(const QString &str) const {
  QString out = str.trimmed();
  out.replace(QRegExp("\\s+"), " ");
  return out.trimmed();
}

const QString ACustomer::stripPhone(const QString &phone) const {
  QString out = phone.trimmed();
  out.replace(QRegExp("\\D+"), "");
  return out.trimmed();
}

const QRegExp ACustomer::mailRegExp() {
  QRegExp reg;
  reg.setCaseSensitivity(Qt::CaseInsensitive);
  reg.setPattern("^([\\d\\w\\-\\.]{3,})@([\\d\\w\\-\\.]{3,})\\.([a-z]{2,6})$");
  return reg;
}

const QRegExp ACustomer::phoneRegExp() {
  QRegExp reg;
  reg.setPattern("^([\\d]{6}[\\d]+)$");
  return reg;
}

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

void ACustomer::enableCompany(bool company) { p_company = company; }

bool ACustomer::isCompanyEnabled() { return p_company; }

void ACustomer::setCompanyName(const QString &company) {
  p_company_name = burnish(company);
}

const QString ACustomer::getCompanyName() { return p_company_name; }

void ACustomer::setCompanyEmployer(const QString &employer) {
  p_company_employer = burnish(employer);
}

const QString ACustomer::getCompanyEmployer() { return p_company_employer; }

void ACustomer::setGender(AntiquaCRM::Gender gender) { p_gender = gender; }

AntiquaCRM::Gender ACustomer::getGender() { return p_gender; }

void ACustomer::setSalutation(const QString &salutation) {
  p_salutation = burnish(salutation);
}

const QString ACustomer::getSalutation() { return p_salutation; }

void ACustomer::setFirstname(const QString &firstname) {
  p_firstname = burnish(firstname);
}

const QString ACustomer::getFirstname() { return p_firstname; }

void ACustomer::setLastname(const QString &lastname) { p_lastname = lastname; }

const QString ACustomer::getLastname() { return p_lastname; }

void ACustomer::setPostalcode(const QString &postalcode) {
  p_postalcode = burnish(postalcode);
}

const QString ACustomer::getPostalcode() { return p_postalcode; }

void ACustomer::setCountry(const QString &country) {
  p_country = burnish(country);
}

const QString ACustomer::getCountry() { return p_country; }

void ACustomer::setLocation(const QString &location) {
  p_location = burnish(location);
}

const QString ACustomer::getLocation() { return p_location; }

void ACustomer::setStreet(const QString &street) { p_street = burnish(street); }

const QString ACustomer::getStreet() { return p_street; }

void ACustomer::setEmail(const QString &email, int index) {
  QString b_email = email.trimmed();
  if (!b_email.isEmpty() && !mailRegExp().exactMatch(b_email))
    return;

  if (index > 0)
    p_email_1 = b_email;
  else
    p_email_0 = b_email;
}

const QString ACustomer::getEmail(int index) {
  if (index > 0)
    return p_email_1;

  return p_email_0;
}

void ACustomer::setShippingEMail(const QString &email) {
  QString b_email = email.trimmed();
  if (!b_email.isEmpty() && !mailRegExp().exactMatch(b_email))
    return;

  p_shipping_email = b_email;
}

const QString ACustomer::getShippingEMail() { return p_shipping_email; }

void ACustomer::setWebsite(const QString &website) {
  p_website = burnish(website);
}

const QString ACustomer::getWebsite() { return p_website; }

void ACustomer::setPhone(const QString &phone, int index) {
  QString b_number = stripPhone(phone);
  if (!b_number.isEmpty() && !phoneRegExp().exactMatch(b_number))
    return;

  if (index > 0)
    p_phone_1 = b_number;
  else
    p_phone_0 = b_number;
}

const QString ACustomer::getPhone(int index) {
  if (index > 0)
    return p_phone_1;

  return p_phone_0;
}

void ACustomer::setMobilPhone(const QString &mobil, int index) {
  QString b_number = stripPhone(mobil);
  if (!b_number.isEmpty() && !phoneRegExp().exactMatch(b_number))
    return;

  if (index > 0)
    p_mobil_1 = b_number;
  else
    p_mobil_0 = b_number;
}

const QString ACustomer::getMobilPhone(int index) {
  if (index > 0)
    return p_mobil_1;

  return p_mobil_0;
}

void ACustomer::setFax(const QString &fax, int index) {
  QString b_number = stripPhone(fax);
  if (!b_number.isEmpty() && !phoneRegExp().exactMatch(b_number))
    return;

  if (index > 0)
    p_fax_1 = b_number;
  else
    p_fax_0 = b_number;
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

void ACustomer::setIBAN(const QString &iban) { p_iban = burnish(iban); }

const QString ACustomer::getIBAN() { return p_iban; }

void ACustomer::setSwiftBic(const QString &swiftbic) {
  p_swift_bic = burnish(swiftbic);
}

const QString ACustomer::getSwiftBic() { return p_swift_bic; }

void ACustomer::setTax(const QString &tax) { p_tax_id = burnish(tax); }

const QString ACustomer::getTax() { return p_tax_id; }

void ACustomer::setSince(const QDateTime &since) { p_since = since; }

const QDateTime ACustomer::getSince() { return p_since; }

void ACustomer::setLastChanged(const QDateTime &changed) {
  p_changed = changed;
}

const QDateTime ACustomer::getLastChanged() { return p_changed; }

void ACustomer::setCountryCode(const QString &bcp47) {
  p_country_bcp47 = burnish(bcp47);
}

const QString ACustomer::getCountryCode() { return p_country_bcp47; }

void ACustomer::setUniqImportKey(const QString &key) {
  p_uniq_import_key = key.trimmed();
}

const QString ACustomer::getUniqImportKey() { return p_uniq_import_key; }

}; // namespace AntiquaCRM
