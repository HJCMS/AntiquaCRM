// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SQLPROFILE_H
#define ANTIQUACRM_SQLPROFILE_H

#include <AGlobal>
#include <QMetaType>
#include <QString>
#include <QStringList>

namespace AntiquaCRM {

/**
 * @brief it provides a temporary data profile to connect the database
 * @section sql
 */
class ANTIQUACRM_LIBRARY ASqlProfile final {

private:
  const QString p_profile;
  QString pg_database;
  QString pg_hostname;
  QString pg_username;
  QString pg_password;
  int pg_timeout = 5;
  int pg_port = 5432;
  bool pg_ssl = true;
  QString ssl_CA;
  QString ssl_CN;
  QString ssl_bundle;
  QString ssl_mode;
  QString ssl_root_cert;

public:
  explicit ASqlProfile(const QString &profile = QString("Default"));

  /**
   * @brief List of all valid Configuration keys inside this profile.
   */
  static const QStringList keys();

  /**
   * @brief set/get SQL Connection Database
   */
  void setDatabaseName(const QString &database);
  const QString getDatabaseName();

  /**
   * @brief set/get SQL Connection Hostname
   */
  void setHostname(const QString &host);
  const QString getHostname();

  /**
   * @brief set/get SQL Connection
   */
  void setUsername(const QString &user);
  const QString getUsername();

  /**
   * @brief set/get SQL Connection Passsword
   */
  void setPassword(const QString &pass);
  const QString getPassword();

  /**
   * @brief set/get SQL Connection timeout
   */
  void setTimeout(int timeout = 5);
  int getTimeout();

  /**
   * @brief set/get SQL Connection Port
   */
  void setPort(int port = 5432);
  int getPort();

  /**
   * @brief set/get SQL SSL/TLS Support
   */
  void setEnableSSL(bool set = true);
  bool getEnableSSL();

  /**
   * @brief set/get Certificate Issuer Name
   */
  void setSslIssuer(const QString &name);
  const QString getSslIssuer();

  /**
   * @brief set/get Certificate Common Name
   */
  void setSslCommonName(const QString &name);
  const QString getSslCommonName();

  /**
   * @brief set/get SSL Bundle Path
   */
  void setSslBundle(const QString &filePath);
  const QString getSslBundle();

  /**
   * @brief set/get SSL Bundle Path
   */
  void setSslMode(const QString &mode = QString("require"));
  const QString getSslMode();

  /**
   * @brief set/get SSL Server Public Root Certificate
   */
  void setSslRootCert(const QString &filePath);
  const QString getSslRootCert();
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SQLPROFILE_H
