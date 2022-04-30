// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sqlconfig.h"
#include "antiqua_global.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QVariant>

namespace HJCMS {

static const QString config_domain() {
  QString str = QString::fromUtf8(HJCMS_CONFIG_DOMAIN);
  str.append(QDir::separator());
  str.append(QString::fromUtf8(ANTIQUACRM_NAME));
  return str;
}

SqlConfig::SqlConfig(QObject *parent)
    : QSettings(QSettings::NativeFormat, QSettings::UserScope, config_domain(),
                ANTIQUACRM_NAME, parent) {
  setObjectName("hjcms_sql_config");
}

const QString SqlConfig::section(const QString &f) {
  QString p("postgresql/");
  p.append(f);
  return p;
}

const QString SqlConfig::getConnectioName() {
  QString str(ANTIQUACRM_CONNECTION_PREFIX);
  str.append(ANTIQUACRM_NAME);
  return str;
}

void SqlConfig::setAddress(const QString &str) {
  if (str.isEmpty())
    return;

  QString s(section("hostname"));
  setValue(s, str);
}

const QString SqlConfig::getAddress() {
  QString s(section("hostname"));
  return value(s, "localhost").toString();
}

void SqlConfig::setPort(int p) {
  QString s(section("port"));
  setValue(s, p);
}

int SqlConfig::getPort() {
  QString s(section("port"));
  return value(s, 5432).toInt();
}

void SqlConfig::setDatabase(const QString &str) {
  if (str.isEmpty())
    return;

  QString s(section("database"));
  setValue(s, str);
}

const QString SqlConfig::getDatabase() {
  QString s(section("database"));
  return value(s, ANTIQUACRM_NAME).toString();
}

void SqlConfig::setUserName(const QString &str) {
  if (str.isEmpty())
    return;
}

const QString SqlConfig::getUserName() {
  QString s(section("username"));
  return value(s, ANTIQUACRM_NAME).toString();
}

void SqlConfig::setPassword(const QString &str) {
  if (str.isEmpty())
    return;

  QByteArray b(str.toLocal8Bit());
  QByteArray p = b.toBase64();
  QString s(section("password"));
  setValue(s, p);
}

const QString SqlConfig::getPassword() {
  QString b;
  QString s(section("password"));
  QByteArray p = value(s, ANTIQUACRM_NAME).toByteArray();
  return b.fromLocal8Bit(
      QByteArray::fromBase64(p, QByteArray::Base64UrlEncoding));
}

void SqlConfig::setOptions(const QStringList &list) {
  QString s(section("options"));
  if (list.isEmpty()) {
    QStringList l(list);
    l << QString("connect_timeout=5");
    setValue(s, l);
    return;
  }
  setValue(s, list);
}

const QStringList SqlConfig::getOptions() {
  QString s(section("options"));
  return value(s).toStringList();
}

void SqlConfig::setSecureEnabled(bool b) {
  QString s(section("ssl"));
  setValue(s, b);
}

bool SqlConfig::isSecureEnabled() {
  QString s(section("ssl"));
  return value(s, false).toBool();
}

void SqlConfig::setCertificate(const QString &p) {
  QString s(section("certpath"));
  QFileInfo f(p);
  if (f.exists()) {
    setValue(s, f.absoluteFilePath());
    return;
  }
  remove(s);
}

const QString SqlConfig::getCertificate() {
  QString s(section("certpath"));
  return value(s, QString()).toString();
}

}; // namespace HJCMS
