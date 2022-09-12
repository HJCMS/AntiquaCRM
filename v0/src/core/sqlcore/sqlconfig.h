// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef HJCMS_SQLCONFIG_SQLCORE_H
#define HJCMS_SQLCONFIG_SQLCORE_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QSslCertificate>

namespace HJCMS {
/**
 * @class HJCMS::SqlConfig
 * @short Connection Settings
 * <a href="https://www.postgresql.org/docs/current/runtime-config-connection.html">
 *  PostgreSQL Runtime Connectionconfig
 * </a>
 */
class SqlConfig : QSettings {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

protected:
  static const QString section(const QString &);

public:
  explicit SqlConfig(QObject *parent = nullptr);

  /**
   * @brief PgSQL Verbindungsname
   */
  static const QString getConnectioName();

  /**
   * @brief Adresse oder Hostname setzen
   */
  void setAddress(const QString &);
  const QString getAddress();

  /**
   * @brief Datenbank Verbindungsport setzen
   * @note Standard ist 5432
   */
  void setPort(int);
  int getPort();

  /**
   * @brief PgSQL Datenbankname setzen
   */
  void setDatabase(const QString &);
  const QString getDatabase();

  /**
   * @brief Benutzername setzen
   */
  void setUserName(const QString &);
  const QString getUserName();

  /**
   * @brief Passwort setzen
   */
  void setPassword(const QString &);
  const QString getPassword();

  /**
   * @brief Verbindungs Optionen
   */
  const QStringList getOptions();

  /**
   * @brief Aktiviere SSL/TLS
   */
  void setSecureEnabled(bool b = false);
  bool isSecureEnabled();

  /**
   * @brief Benutzer Zertifikat setzen
   */
  void setCertificate(const QString &);
  const QString getCertificate();

  /**
   * @brief Privater Benutzerschlüssel setzen
   */
  void setPrivateKey(const QString &);
  const QString getPrivateKey();

  /**
   * @brief Common Name für Zertifikat setzen
   */
  void setCaCN(const QString &);
  const QString getCaCN();

  /**
   * @brief Pfad zum Server CA
   */
  void setCaRootCert(const QString &ca);
  const QString getCaRootCert();

  /**
   * @brief ROOT-CA Zertifikat auswählen
   */
  void setCaCert(const QString &cn);
  const QSslCertificate getCaCert();

  /**
   * @brief System/Benutzer CA-Paketpfad setzen
   */
  void setCaBundle(const QString &);
  const QString getCaBundle();
};
};     // namespace HJCMS
#endif // HJCMS_SQLCONFIG_SQLCORE_H
