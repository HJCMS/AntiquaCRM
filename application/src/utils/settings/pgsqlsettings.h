// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PGSQL_SETTINGS_H
#define PGSQL_SETTINGS_H

#include <QGroupBox>
#include <QHash>
#include <QObject>
#include <QRegularExpression>
#include <QUrl>
#include <QVariant>
#include <SettingsDialog>

/**
 * @class PgSQLSettings
 * @brief PostgreSQL Configuration
 * Parameter Key Words:
 * @see https://www.postgresql.org/docs/current/libpq-connect.html
 * @ingroup _settings
 */
class PgSQLSettings final : public SettingsWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  const QRegularExpression pattern = QRegularExpression("^(pg|ssl)_\\w+$");

  /**
   * @brief Profil
   */
  QComboBox *sql_profiles;

  /**
   * @brief PostgreSQL option: host|hostaddr
   */
  LineEdit *pg_hostname;

  /**
   * @brief PostgreSQL option: dbname
   */
  LineEdit *pg_database;

  /**
   * @brief PostgreSQL option: user
   */
  LineEdit *pg_username;

  /**
   * @brief PostgreSQL option: password
   */
  LineEdit *pg_password;

  /**
   * @brief PostgreSQL option: port
   */
  IntSpinBox *pg_port;

  /**
   * @brief PostgreSQL option: connect_timeout
   */
  IntSpinBox *pg_timeout;

  /**
   * @brief Enable SQL SSL/TLS Support
   */
  BoolBox *pg_ssl;

  /**
   * @brief Edit SSL/TLS
   */
  QGroupBox *m_tls;

  /**
   * @brief System SSL/TLS ca-bundle.pem
   */
  LineEdit *ssl_bundle;

  /**
   * @brief Server Certificate CommonName (CN)
   */
  LineEdit *ssl_CN;

  /**
   * @brief CA Subjectname from ca-bundle.pem
   */
  QComboBox *ssl_CA;

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

  /**
   * @brief Configuration
   */
  QHash<QString, QVariant> p_hash;

  void initSignalChanged();
  void initCaBundleData(const QString &bundle);
  void setComboBoxData(const QString &name, const QVariant &value);
  const QVariant getComboBoxData(const QString &name);
  const QString openFileDialog(const QString &dest = QString());

private Q_SLOTS:
  void openCaBundle();
  void openRootCert();
  void openProfile(const QString &name);

public:
  explicit PgSQLSettings(QWidget *parent = nullptr);
  void setPageTitle(const QString &);
  const QString getPageTitle();
  void setPageIcon(const QIcon &);
  const QIcon getPageIcon();
  void loadSectionConfig();
  void saveSectionConfig();

  static const QUrl pgsqlClientAuthDocUrl() {
    QUrl url;
    url.setScheme("https");
    url.setHost("www.postgresql.org");
    url.setPath("/docs/current/client-authentication.html");
    return url;
  }
};

#endif // PGSQL_SETTINGS_H
