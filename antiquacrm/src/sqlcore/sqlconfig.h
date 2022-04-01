// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef HJCMS_SQLCONFIG_H
#define HJCMS_SQLCONFIG_H

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
  class SqlConfig : QSettings
  {
  public:
    explicit SqlConfig();

    const QString getConnectioName();

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

    void setCertificate(const QString &);
    const QString getCertificate();
  };
};     // namespace HJCMS
#endif // SQLCONFIG_H
