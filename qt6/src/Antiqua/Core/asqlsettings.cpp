// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlsettings.h"
#include "aglobal.h"
#include "asqlprofile.h"

#include <QDebug>
#include <QDir>

namespace AntiquaCRM {

ASqlSettings::ASqlSettings(QObject *parent, const QString &profile)
    : ASettings(parent), connection_profile{profile} {
  setObjectName("antiquacrm_sqlsettings");
  if (profile.isEmpty())
    connection_profile = getProfile();
}

const QString ASqlSettings::groupPath() {
  QString group("database");
  group.append("/");
  group.append(connection_profile);
  return group;
}

const QByteArray ASqlSettings::toRealm(const QString &pass) {
  if (pass.isEmpty())
    return QByteArray();

  QByteArray out = pass.toLocal8Bit();
  return out.toBase64();
}

const QString ASqlSettings::fromRealm(const QByteArray &array) {
  QString out;
  QByteArray buf = QByteArray::fromBase64(array, QByteArray::Base64UrlEncoding);
  return out.fromLocal8Bit(buf);
}

const QString ASqlSettings::connectionName() {
  return QString(ANTIQUACRM_CONNECTION_DOMAIN);
}

const AntiquaCRM::ASqlProfile ASqlSettings::connectionProfile() {
  ASqlProfile _cp(connection_profile);
  _cp.setDatabaseName(getParam("pg_database").toString());
  _cp.setHostname(getParam("pg_hostname").toString());
  _cp.setUsername(getParam("pg_username").toString());
  _cp.setPassword(getParam("pg_password").toString());
  _cp.setTimeout(getParam("pg_timeout").toInt());
  _cp.setPort(getParam("pg_port").toInt());
  _cp.setEnableSSL(getParam("pg_ssl").toBool());
  _cp.setSslIssuer(getParam("ssl_CA").toString());
  _cp.setSslCommonName(getParam("ssl_CN").toString());
  _cp.setSslBundle(getParam("ssl_bundle").toString());
  _cp.setSslMode(getParam("ssl_mode").toString());
  _cp.setSslRootCert(getParam("ssl_root_cert").toString());
  return _cp;
}

void ASqlSettings::setProfile(const QString &name) {
  connection_profile = name.trimmed();
  setValue("database_profile", connection_profile);
}

const QString ASqlSettings::getProfile() {
  return value("database_profile", "Default").toString();
}

const QStringList ASqlSettings::profiles() {
  QStringList l;
  beginGroup("database");
  l = childGroups();
  endGroup();
  return l;
}

void ASqlSettings::setParam(const QString &key, const QVariant &value) {
  beginGroup(groupPath());
  if (key.contains("password", Qt::CaseInsensitive)) {
    QByteArray buffer = toRealm(value.toString());
    if (!buffer.isNull())
      setValue(key, buffer);
  } else
    setValue(key, value);
  endGroup();
}

const QVariant ASqlSettings::getParam(const QString &key) {
  QVariant out;
  beginGroup(groupPath());
  if (key.contains("password", Qt::CaseInsensitive))
    out = fromRealm(value(key).toByteArray());
  else
    out = value(key);
  endGroup();
  return out;
}

}; // namespace AntiquaCRM
