// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CUSTOMERDATA_H
#define ANTIQUACRM_CUSTOMERDATA_H

#include <AGlobal>
#include <QDateTime>
#include <QList>
#include <QMetaType>
#include <QRegExp>

namespace AntiquaCRM {

/**
 * @brief Customer data Class
 * @section Customers
 * This class is used to import and export Customer dataset to different class
 * structures, it is mostly used by the Plugin Interface.
 * This is a final class, because it depends on the SQL Table structure.
 */
class ANTIQUACRM_LIBRARAY ACustomer final {
private:
  qint64 p_cId = -1;
  bool p_locked = false;
  AntiquaCRM::CustomerTrustLevel p_trusted = NO_TRUST_LEVEL;
  qint64 p_transactions = 0;
  // QList<AntiquaCRM::APayments &> p_purchases;
  bool p_company = false;
  QString p_company_name = QString();
  QString p_company_employer = QString();
  AntiquaCRM::Gender p_gender = NO_GENDER;
  QString p_salutation = QString();
  QString p_firstname = QString();
  QString p_lastname = QString();
  QString p_postalcode = QString();
  QString p_country = QString();
  QString p_location = QString();
  QString p_street = QString();
  QString p_email_0 = QString();
  QString p_email_1 = QString();
  QString p_shipping_email = QString();
  QString p_website = QString();
  QString p_phone_0 = QString();
  QString p_phone_1 = QString();
  QString p_mobil_0 = QString();
  QString p_mobil_1 = QString();
  QString p_fax_0 = QString();
  QString p_fax_1 = QString();
  QString p_postal_address = QString();
  QString p_shipping_address = QString();
  QString p_comments = QString();
  QString p_iban = QString();
  QString p_swift_bic = QString();
  QString p_tax_id = QString();
  QDateTime p_since = QDateTime();
  QDateTime p_changed = QDateTime();
  QString p_country_bcp47 = QString();
  QString p_uniq_import_key = QString();

public:
  explicit ACustomer(qint64 customerId = 0);

  /**
   * @brief set/get Customer Id
   * @param cId
   */
  void setCustomerId(qint64 cId);
  qint64 getCustomerId();

  /**
   * @brief set/get Customer is locked or not
   * @param locked
   */
  void setLocked(bool locked = false);
  bool isLocked();

  /**
   * @brief set/get Customer trust level
   * @param trust
   */
  void setTrusted(AntiquaCRM::CustomerTrustLevel trust = NO_TRUST_LEVEL);
  AntiquaCRM::CustomerTrustLevel getTrusted();

  void setTransactions(int count = 0);
  int getTransactions();
  /*
    void setPurchases(QList<AntiquaCRM::APayments> &purchases);
    QList<AntiquaCRM::APayments &> getPurchases();
  */
  /**
   * @brief set/query is Company
   * @param company
   */
  void setCompany(bool company = false);
  bool isCompany();

  /**
   * @brief set/get Companyname
   * @param company
   */
  void setCompanyName(const QString &company);
  const QString getCompanyName();

  /**
   * @brief set/get Company Employer
   * @param employer
   */
  void setCompanyEmployer(const QString &employer);
  const QString getCompanyEmployer();

  /**
   * @brief set/get Gender
   * @param gender
   */
  void setGender(AntiquaCRM::Gender gender = NO_GENDER);
  AntiquaCRM::Gender getGender();

  /**
   * @brief set/get salutation
   * @param salutation
   */
  void setSalutation(const QString &salutation);
  const QString getSalutation();

  /**
   * @brief set/get firstname
   * @param firstname
   */
  void setFirstname(const QString &firstname);
  const QString getFirstname();

  /**
   * @brief set/get lastname
   * @param lastname
   */
  void setLastname(const QString &lastname);
  const QString getLastname();

  /**
   * @brief set/get postalcode
   * @param postalcode
   */
  void setPostalcode(const QString &postalcode);
  const QString getPostalcode();

  /**
   * @brief set/get country
   * @param country
   */
  void setCountry(const QString &country);
  const QString getCountry();

  /**
   * @brief set/get location
   * @param location
   */
  void setLocation(const QString &location);
  const QString getLocation();

  /**
   * @brief set/get street
   * @param street
   */
  void setStreet(const QString &street);
  const QString getStreet();

  /**
   * @brief set/get email
   * @param email
   * @param index Default 0 => c_email_0
   */
  void setEmail(const QString &email, int index = 0);
  const QString getEmail(int index = 0);

  /**
   * @brief set/get shipping EMail
   * @param email
   */
  void setShippingEMail(const QString &email);
  const QString getShippingEMail();

  /**
   * @brief set/get website
   * @param website
   */
  void setWebsite(const QString &website);
  const QString getWebsite();

  /**
   * @brief set/get phone
   * @param phone
   * @param index Default 0 => c_phone_0
   */
  void setPhone(const QString &phone, int index = 0);
  const QString getPhone(int index = 0);

  /**
   * @brief set/get mobil phone
   * @param mobil
   * @param index Default 0 => c_mobil_0
   */
  void setMobilPhone(const QString &mobil, int index = 0);
  const QString getMobilPhone(int index = 0);

  /**
   * @brief set/get fax
   * @param fax
   */
  void setFax(const QString &fax, int index = 0);
  const QString getFax(int index = 0);

  /**
   * @brief set/get postal address
   * @param address
   */
  void setPostalAddress(const QString &address);
  const QString getPostalAddress();

  /**
   * @brief set/get shipping address
   * @param address
   */
  void setShippingAddress(const QString &address);
  const QString getShippingAddress();

  /**
   * @brief set/get comments
   * @param comments
   */
  void setComments(const QString &comments);
  const QString getComments();

  /**
   * @brief set/get Banking IBAN
   * @param iban
   */
  void setIBAN(const QString &iban);
  const QString getIBAN();

  /**
   * @brief set/get Banking Swift Bic
   * @param swiftbic
   */
  void setSwiftBic(const QString &swiftbic);
  const QString getSwiftBic();

  /**
   * @brief set/get TAX
   * @param tax
   */
  void setTax(const QString &tax);
  const QString getTax();

  /**
   * @brief set/get Entry Since
   * @param since
   */
  void setSince(const QDateTime &since);
  const QDateTime getSince();

  /**
   * @brief set/get Entry Changed
   * @param changed
   */
  void setLastChanged(const QDateTime &changed);
  const QDateTime getLastChanged();

  /**
   * @brief set/get IETF language tag
   * @param bcp47
   */
  void setCountryCode(const QString &bcp47);
  const QString getCountryCode();

  /**
   * @brief get/set unique import key to prevent duplicate entries
   * @param key
   */
  void setUniqImportKey(const QString &key);
  const QString getUniqImportKey();
};

/**
 * @brief Customer data list
 * @section Customers
 */
typedef QList<ACustomer *> ACustomerData;

}; // namespace AntiquaCRM

Q_DECLARE_METATYPE(AntiquaCRM::ACustomerData)

#endif // ANTIQUACRM_CUSTOMERDATA_H
