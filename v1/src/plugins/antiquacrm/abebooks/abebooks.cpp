// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooks.h"

/** @brief Konfigurationsgruppe */
#ifndef ABEBOOKS_CONFIG_GROUP
#define ABEBOOKS_CONFIG_GROUP "provider/abebooks"
#endif

/** @brief Wird für Menüeintrag und Gruppenzuweisung benötigt! */
#ifndef ABEBOOKS_CONFIG_PROVIDER
#define ABEBOOKS_CONFIG_PROVIDER "AbeBooks"
#endif

#include <AntiquaCRM>
#include <QDebug>
#include <QTimer>

Abebooks::Abebooks(QObject *parent) : AntiquaCRM::APluginInterface{parent} {
  setObjectName(ABEBOOKS_CONFIG_PROVIDER);
  m_network = new AntiquaCRM::ANetworker(this);
  initConfigurations();
}

void Abebooks::initConfigurations() {
  QUrl url;
  AntiquaCRM::ASettings cfg(this);
  cfg.beginGroup(ABEBOOKS_CONFIG_GROUP);
  url.setScheme("https");
  url.setHost(cfg.value("api_host", "orderupdate.abebooks.com").toString());
  url.setPort(cfg.value("api_port", 10003).toInt());
  apiUser = cfg.value("api_user", "UNKNOWN").toString();
  apiKey = cfg.value("api_key", "007").toString();
  historyCall = cfg.value("api_history_call", -7).toInt();
  cfg.endGroup();
  apiUrl = url;
}

const QUrl Abebooks::apiQuery(const QString &section) {
  Q_UNUSED(section);
  QUrl url(apiUrl);
  actionsCookie = QNetworkCookie("action", "TODO");
  actionsCookie.setDomain(url.host());
  actionsCookie.setSecure(true);
  return url;
}

void Abebooks::prepareJsonResponse(const QJsonDocument &jdoc) {
  Q_UNUSED(jdoc);
}

void Abebooks::queryFinished(QNetworkReply *reply) { Q_UNUSED(reply); }

void Abebooks::queryOrders(int waitSecs) {
  Q_UNUSED(waitSecs);
  emit sendQueryFinished();
}

const QString Abebooks::configProvider() const {
  return QString(ABEBOOKS_CONFIG_PROVIDER).toLower();
}

const QString Abebooks::displayName() const {
  return QString(ABEBOOKS_CONFIG_PROVIDER).trimmed();
}

const AntiquaCRM::AProviderOrders Abebooks::getOrders() const {
  AntiquaCRM::AProviderOrders booking;
  return booking;
}

bool Abebooks::createInterface(QObject *parent) {
  Q_UNUSED(parent);
  return true;
}
