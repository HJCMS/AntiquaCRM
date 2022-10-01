// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "asqlprofile.h"

namespace AntiquaCRM {

ASqlProfile::ASqlProfile(const QString &profile) : p_profile(profile) {}

const QStringList ASqlProfile::keys() {
  QStringList k;
  k << "pg_database";
  k << "pg_hostname";
  k << "pg_username";
  k << "pg_password";
  k << "pg_timeout";
  k << "pg_port";
  k << "pg_ssl";
  k << "ssl_CA";
  k << "ssl_CN";
  k << "ssl_bundle";
  k << "ssl_mode";
  k << "ssl_root_cert";
  return k;
}

void ASqlProfile::setDatabaseName(const QString &database) {
  pg_database = database;
}

const QString ASqlProfile::getDatabaseName() { return pg_database; }

void ASqlProfile::setHostname(const QString &host) { pg_hostname = host; }

const QString ASqlProfile::getHostname() { return pg_hostname; }

void ASqlProfile::setUsername(const QString &user) { pg_username = user; }

const QString ASqlProfile::getUsername() { return pg_username; }

void ASqlProfile::setPassword(const QString &pass) { pg_password = pass; }

const QString ASqlProfile::getPassword() { return pg_password; }

void ASqlProfile::setTimeout(int timeout) { pg_timeout = timeout; }

int ASqlProfile::getTimeout() { return pg_timeout; }

void ASqlProfile::setPort(int port) { pg_port = port; }

int ASqlProfile::getPort() { return pg_port; }

void ASqlProfile::setEnableSSL(bool set) { pg_ssl = set; }

bool ASqlProfile::getEnableSSL() { return pg_ssl; }

void ASqlProfile::setSslIssuer(const QString &name) { ssl_CA = name; }

const QString ASqlProfile::getSslIssuer() { return ssl_CA; }

void ASqlProfile::setSslCommonName(const QString &name) { ssl_CN = name; }

const QString ASqlProfile::getSslCommonName() { return ssl_CN; }

void ASqlProfile::setSslBundle(const QString &filePath) {
  ssl_bundle = filePath;
}

const QString ASqlProfile::getSslBundle() { return ssl_bundle; }

void ASqlProfile::setSslMode(const QString &mode) { ssl_mode = mode; }

const QString ASqlProfile::getSslMode() { return ssl_mode; }

void ASqlProfile::setSslRootCert(const QString &filePath) {
  ssl_root_cert = filePath;
}

const QString ASqlProfile::getSslRootCert() { return ssl_root_cert; }

}; // namespace AntiquaCRM
