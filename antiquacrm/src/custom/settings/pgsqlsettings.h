// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PGSQL_SETTINGS_H
#define PGSQL_SETTINGS_H

#include "settingswidget.h"

#include <QComboBox>
#include <QGroupBox>
#include <QHash>
#include <QObject>
#include <QVariant>

/**
 * @class PgSQLSettings
 * @brief PostgreSQL Configuration
 * Parameter Key Words:
 *  @link https://www.postgresql.org/docs/current/libpq-connect.html
 *
 */
class PgSQLSettings final : public SettingsWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @defgroup postgresql
   * @{
   * @brief sql_hostname
   * PostgreSQL option: host|hostaddr
   */
  LineEdit *sql_hostname;

  /**
   * @brief sql_databasename
   * PostgreSQL option: dbname
   */
  LineEdit *sql_databasename;

  /**
   * @brief sql_username
   * PostgreSQL option: user
   */
  LineEdit *sql_username;

  /**
   * @brief sql_password
   * PostgreSQL option: password
   */
  LineEdit *sql_password;

  /**
   * @brief sql_port
   * PostgreSQL option: port
   */
  IntSpinBox *sql_port;

  /**
   * @brief sql_timeout
   * PostgreSQL option: connect_timeout
   */
  IntSpinBox *sql_timeout;

  /**
   * @brief m_ssl
   * Enable SQL SSL/TLS Support
   */
  BoolBox *sql_ssl;
  /** @} */

  /**
   * @defgroup SSL/TLS Connection
   * @{
   */

  /**
   * @brief Edit SSL/TLS
   */
  QGroupBox *m_tls;

  /**
   * @brief System SSL/TLS ca-bundle.pem
   */
  LineEdit *ssl_ca_bundle;

  /**
   * @brief Server Certificate CommonName (CN)
   */
  LineEdit *ssl_CN;

  /**
   * @brief CA Subjectname from ca-bundle.pem
   */
  QComboBox *ssl_ca_CN;

  /**
   * @brief Location of the Server Issuer CA
   * PostgreSQL option: @b sslrootcert
   * Default ~/.postgresql/root.crt
   */
  LineEdit *ssl_root_cert;

  /**
   * @brief ssl_mode
   * PostgreSQL option:
   *  @li prefer (default)
   *  @li verify-ca (AntiquaCRM Default)
   */
  QComboBox *ssl_mode;

  /** @} */

  /**
   * @defgroup SSL/Peer Connection
   * @{
   * @brief Enable SSL/TLS Support
   */
  QGroupBox *ssl_peer;

  /**
   * @brief ssl_peer_certfile
   * PostgreSQL option: @b sslcert
   * Default ~/.postgresql/postgresql.crt
   */
  LineEdit *ssl_peer_cert;

  /**
   * @brief ssl_peer_keyfile
   * PostgreSQL option: @b sslkey
   * Default ~/.postgresql/postgresql.key
   */
  LineEdit *ssl_peer_key;

  /**
   * @brief ssl_peer_sslpassword
   * PostgreSQL option: @b sslpassword
   */
  LineEdit *ssl_peer_pass;

  /** @} */

  /**
   * @brief Configuration
   */
  QHash<QString, QVariant> p_hash;

  void initCaBundleData(const QString &bundle);

  const QString openFileDialog(const QString &dest = QString());

private Q_SLOTS:
  void openCaBundle();
  void openRootCert();

public:
  explicit PgSQLSettings(QWidget *parent = nullptr);
  void setPageTitle(const QString &);
  const QString getPageTitle();
  void setPageIcon(const QIcon &);
  const QIcon getPageIcon();
  void loadSectionConfig();
  void saveSectionConfig();
};

#endif // PGSQL_SETTINGS_H
