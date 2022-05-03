// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sqlconfig.h"
#include "antiqua_global.h"

#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QSslConfiguration>
#include <QVariant>

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
  qDebug() << Q_FUNC_INFO << "TODO" << list;
}

const QStringList SqlConfig::getOptions() {
  // https://www.postgresql.org/docs/current/libpq-connect.html
  QStringList options;
  options.append("connect_timeout=" + value(section("timeout"), 5).toString());
  options.append("application_name=" + QString(ANTIQUACRM_NAME));
  QFileInfo ca_root_cert(getCaRootCert());
  if (ca_root_cert.exists()) {
    options.append("sslmode=" +
                   value("ssloptions/ssl_mode", "prefer").toString());
    options.append("sslrootcert=" + ca_root_cert.filePath());
  }
  if (value("ssloptions/ssl_peer").toBool()) {
    QString sslcert = value("ssloptions/ssl_peer_cert").toString();
    options.append("sslcert=" + sslcert);
    QString sslkey = value("ssloptions/ssl_peer_key").toString();
    options.append("sslkey=" + sslkey);
  }
  return options;
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
  setValue("ssloptions/ssl_peer_cert", p);
}

const QString SqlConfig::getCertificate() {
  return value("ssloptions/ssl_peer_cert").toString();
}

void SqlConfig::setPrivateKey(const QString &p) {
  setValue("ssloptions/ssl_peer_key", p);
}

const QString SqlConfig::getPrivateKey() {
  return value("ssloptions/ssl_peer_key").toString();
}

void SqlConfig::setCaCN(const QString &cn) {
  setValue("ssloptions/ssl_CN", cn);
}

const QString SqlConfig::getCaCN() {
  return value("ssloptions/ssl_CN").toString();
}

void SqlConfig::setCaRootCert(const QString &ca) {
  setValue("ssloptions/ssl_root_cert", ca);
}

const QString SqlConfig::getCaRootCert() {
  return value("ssloptions/ssl_root_cert").toString();
}

void SqlConfig::setCaCert(const QString &cn) {
  setValue("ssloptions/ssl_CA", cn);
}

const QSslCertificate SqlConfig::getCaCert() {
  QString name = getCaCN();
  QSslCertificate cert;
  if (name.isEmpty()) {
    qWarning("SqlConfig ssl_CA is not set!");
    return cert;
  }

  QSslConfiguration ssl;
  if (!ssl.addCaCertificates(getCaBundle(), QSsl::Pem)) {
    qWarning("SqlConfig ssl_bundle import ca-bundle failed!");
    return cert;
  }

  QList<QSslCertificate> list = ssl.caCertificates();
  for (int i = 0; i < list.size(); i++) {
    QSslCertificate pem = list.at(i);
    if (!pem.isNull() && (pem.issuerDisplayName() == name)) {
      return pem;
    }
  }
  return cert;
}

void SqlConfig::setCaBundle(const QString &p) {
  setValue("ssloptions/ssl_bundle", p);
}

const QString SqlConfig::getCaBundle() {
  return value("ssloptions/ssl_bundle").toString();
}

}; // namespace HJCMS
