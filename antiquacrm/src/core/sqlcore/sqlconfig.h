// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef HJCMS_SQLCONFIG_SQLCORE_H
#define HJCMS_SQLCONFIG_SQLCORE_H

#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtNetwork/QSslCertificate>

namespace HJCMS {
/**
 * @class SqlConfig
 * @short Connection Settings
 * @see https://www.postgresql.org/docs/current/runtime-config-connection.html
 * @ingroup SQL
 */
class SqlConfig : QSettings {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

protected:
  static const QString section(const QString &);

public:
  explicit SqlConfig(QObject *parent = nullptr);

  static const QString getConnectioName();

  void setAddress(const QString &);
  const QString getAddress();

  void setPort(int);
  int getPort();

  void setDatabase(const QString &);
  const QString getDatabase();

  void setUserName(const QString &);
  const QString getUserName();

  void setPassword(const QString &);
  const QString getPassword();

  void setOptions(const QStringList &);
  const QStringList getOptions();

  void setSecureEnabled(bool b = false);
  bool isSecureEnabled();

  void setCertificate(const QString &);
  const QString getCertificate();

  void setPrivateKey(const QString &);
  const QString getPrivateKey();

  void setCaCN(const QString &);
  const QString getCaCN();

  void setCaRootCert(const QString &ca);
  const QString getCaRootCert();

  void setCaCert(const QString &cn);
  const QSslCertificate getCaCert();

  void setCaBundle(const QString &);
  const QString getCaBundle();
};
};     // namespace HJCMS
#endif // HJCMS_SQLCONFIG_SQLCORE_H
