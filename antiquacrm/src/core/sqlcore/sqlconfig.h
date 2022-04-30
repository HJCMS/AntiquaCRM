// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef HJCMS_SQLCONFIG_SQLCORE_H
#define HJCMS_SQLCONFIG_SQLCORE_H

#include <QtCore/QObject>
#include <QtCore/QSettings>
#include <QtCore/QString>
#include <QtCore/QStringList>

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
};
};     // namespace HJCMS
#endif // HJCMS_SQLCONFIG_SQLCORE_H
