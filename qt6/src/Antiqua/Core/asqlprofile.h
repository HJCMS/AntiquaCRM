// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_SQLPROFILE_H
#define ANTIQUACRM_SQLPROFILE_H

#include <AGlobal>
#include <QMetaType>
#include <QString>
#include <QStringList>

namespace AntiquaCRM {

/**
 * @class ASqlProfile
 * @brief Configuration class for PostgreSQL Database profiles.
 *
 * it provides a temporary data profile to connect the database
 *
 * @ingroup CoreLibrary
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
  /**
   * @param profile - PgSQL connection profile
   */
  explicit ASqlProfile(const QString &profile = QString("Default"));

  /**
   * @brief profile keys
   *
   * All valid Configuration keys from this profile.
   */
  static const QStringList keys();

  /**
   * @brief set PgSQL databasename
   * @param database - database name
   */
  void setDatabaseName(const QString &database);

  /**
   * @brief get PgSQL databasename
   */
  const QString getDatabaseName();

  /**
   * @brief set PgSQL hostname
   * @param host - hostname (FQDH)
   */
  void setHostname(const QString &host);

  /**
   * @brief get PgSQL hostname
   */
  const QString getHostname();

  /**
   * @brief set PgSQL username
   * @param user - username
   */
  void setUsername(const QString &user);

  /**
   * @brief get PgSQL username
   */
  const QString getUsername();

  /**
   * @brief set PgSQL password
   * @param pass - password
   */
  void setPassword(const QString &pass);

  /**
   * @brief get PgSQL password
   */
  const QString getPassword();

  /**
   * @brief set PgSQL timeout
   * @param timeout - timeout in seconds
   */
  void setTimeout(int timeout = 5);

  /**
   * @brief get PgSQL timeout
   */
  int getTimeout();

  /**
   * @brief set PgSQL Port
   * @param port - port default 5432
   */
  void setPort(int port = 5432);

  /**
   * @brief get PgSQL Port
   */
  int getPort();

  /**
   * @brief set PgSQL SSL/TLS Support
   * @param set - default Enabled
   */
  void setEnableSSL(bool set = true);

  /**
   * @brief get PgSQL SSL/TLS Support
   */
  bool getEnableSSL();

  /**
   * @brief set PgSQL Certificate Issuer (CA)
   * @param name - CA Name
   */
  void setSslIssuer(const QString &name);

  /**
   * @brief get PgSQL Certificate Issuer Name
   */
  const QString getSslIssuer();

  /**
   * @brief set PgSQL Certificate Common Name
   * @param name - common name
   */
  void setSslCommonName(const QString &name);

  /**
   * @brief get PgSQL Certificate Common Name
   */
  const QString getSslCommonName();

  /**
   * @brief set PgSQL SSL Bundle Path
   * @param filePath - absolute file path
   */
  void setSslBundle(const QString &filePath);

  /**
   * @brief get PgSQL SSL Bundle Path
   */
  const QString getSslBundle();

  /**
   * @brief set PgSQL SSL mode
   * @param mode - mode name
   */
  void setSslMode(const QString &mode = QString("require"));

  /**
   * @brief get PgSQL SSL mode
   */
  const QString getSslMode();

  /**
   * @brief set PgSQL Server Public Root Certificate
   * @param filePath - absolute file path
   */
  void setSslRootCert(const QString &filePath);

  /**
   * @brief get PgSQL SSL Server Public Root Certificate
   */
  const QString getSslRootCert();
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SQLPROFILE_H
