// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlsettings.h"
#include "aglobal.h"
#include "asqlprofile.h"

#include <QDebug>
#include <QDir>

namespace AntiquaCRM {

ASqlSettings::ASqlSettings(QObject *parent)
    : QSettings(QSettings::NativeFormat, QSettings::UserScope, configDomain(),
                ANTIQUACRM_NAME, parent) {
  setObjectName("antiquacrm_sqlsettings");
}

const QString ASqlSettings::groupPath() {
  QString group("database");
  group.append("/");
  group.append(getProfile());
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

const QString ASqlSettings::configDomain() {
  QString str = QString::fromUtf8(HJCMS_CONFIG_DOMAIN);
  str.append(QDir::separator());
  str.append(QString::fromUtf8(ANTIQUACRM_NAME));
  str.append(QDir::separator());
  str.append(QString::fromUtf8(ANTIQUACRM_VERSION));
  return str;
}

const QString ASqlSettings::connectionName() {
  return QString(ANTIQUACRM_CONNECTION_DOMAIN);
}

const ASqlProfile ASqlSettings::connectionProfile() {
  ASqlProfile cp(profile);
  cp.setDatabaseName(getParam("pg_database").toString());
  cp.setHostname(getParam("pg_hostname").toString());
  cp.setUsername(getParam("pg_username").toString());
  cp.setPassword(getParam("pg_password").toString());
  cp.setTimeout(getParam("pg_timeout").toInt());
  cp.setPort(getParam("pg_port").toInt());
  cp.setEnableSSL(getParam("pg_ssl").toBool());
  cp.setSslIssuer(getParam("ssl_CA").toString());
  cp.setSslCommonName(getParam("ssl_CN").toString());
  cp.setSslBundle(getParam("ssl_bundle").toString());
  cp.setSslMode(getParam("ssl_mode").toString());
  cp.setSslRootCert(getParam("ssl_root_cert").toString());
  return cp;
}

void ASqlSettings::setProfile(const QString &name) {
  profile = name.trimmed();
  setValue("database_profile", profile);
}

const QString ASqlSettings::getProfile() {
  return value("database_profile", "Default").toString();
}

const QStringList ASqlSettings::profiles() {
  QStringList l;
  beginGroup("database");
  l = allKeys();
  endGroup();
  return l;
}

void ASqlSettings::setParam(const QString &key, const QVariant &value) {
  beginGroup(groupPath());
  if (key.contains("pass", Qt::CaseInsensitive)) {
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
  if (key.contains("pass", Qt::CaseInsensitive))
    out = fromRealm(value(key).toByteArray());
  else
    out = value(key);
  endGroup();
  return out;
}

}; // namespace AntiquaCRM
