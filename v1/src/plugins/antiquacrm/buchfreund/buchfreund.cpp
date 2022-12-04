// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "buchfreund.h"

/**
 * @def DATE_FORMAT
 * @ingroup Providers SQL Statements
 */
#ifndef BUCHFREUND_DATE_FORMAT
#define BUCHFREUND_DATE_FORMAT "yyyy-MM-dd hh:mm:ss"
#endif

/** @brief Konfigurationsgruppe */
#ifndef BUCHFREUND_CONFIG_GROUP
#define BUCHFREUND_CONFIG_GROUP "provider/buchfreund"
#endif

/** @brief Wird für Menüeintrag und Gruppenzuweisung benötigt! */
#ifndef BUCHFREUND_CONFIG_PROVIDER
#define BUCHFREUND_CONFIG_PROVIDER "Buchfreund"
#endif

// Headers
#include <AntiquaCRM>
#include <QDateTime>
#include <QDebug>
#include <QDialog>
#include <QJsonArray>
#include <QTimeZone>
#include <QUrlQuery>

Buchfreund::Buchfreund(QObject *parent)
    : AntiquaCRM::APluginInterface{parent} {
  setObjectName(BUCHFREUND_CONFIG_PROVIDER);
  m_network = nullptr;
}

bool Buchfreund::initConfigurations() {
  QUrl url;
  QString apiHost("www.buchfreund.de");
  AntiquaCRM::ASettings cfg(this);
  cfg.beginGroup(BUCHFREUND_CONFIG_GROUP);
  url.setScheme("https");
  url.setHost(cfg.value("api_host", apiHost).toString());
  apiKey = cfg.value("api_key", QString()).toString();
  cfg.endGroup();
  apiUrl = url;
  return true;
}

const QUrl Buchfreund::apiQuery(const QString &section) {
  QString p("/verkaeufer/api/");
  p.append(apiKey);
  p.append("/");
  p.append(section);

  QUrl url(apiUrl);
  url.setPath(p);

  return url;
}

const QString Buchfreund::dateString(const QDate &date) const {
  return date.toString(BUCHFREUND_DATE_FORMAT);
}

void Buchfreund::prepareResponse(const QJsonDocument &js) {
  QString fileName = configProvider();
  if (!actionsCookie.name().isNull()) {
    fileName = QString(actionsCookie.value());
  }

  if (actionsCookie.value().contains("_bestellung")) {
    AntiquaCRM::ASharedCacheFiles cacheFile;
    QString jsonData = js.toJson(QJsonDocument::Indented);
    cacheFile.storeTempFile(fileName.toLower(), jsonData);
    emit sendQueryFinished();
  }
}

void Buchfreund::queryFinished(QNetworkReply *reply) {
  if (reply->error() != QNetworkReply::NoError) {
    emit sendErrorResponse(AntiquaCRM::WARNING,
                           tr("Buchfreund response with errors!"));
  }
}

void Buchfreund::orderUpdateAction(const QJsonObject &options) {
  Q_UNUSED(options);
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << Qt::endl << options;
#endif
}

bool Buchfreund::authenticationRequired() { return false; }

const QString Buchfreund::configProvider() const {
  return QString(BUCHFREUND_CONFIG_PROVIDER).toLower();
}

const QString Buchfreund::displayName() const {
  return QString(BUCHFREUND_CONFIG_PROVIDER).trimmed();
}

bool Buchfreund::createInterface(QObject *parent) {
  Q_UNUSED(parent);

  m_network = new AntiquaCRM::ANetworker(AntiquaCRM::JSON_QUERY, this);
  connect(m_network, SIGNAL(sendJsonResponse(const QJsonDocument &)),
          SLOT(prepareResponse(const QJsonDocument &)));
  connect(m_network, SIGNAL(sendContentCodec(QTextCodec *)),
          SLOT(setContentDecoder(QTextCodec *)));
  connect(m_network, SIGNAL(finished(QNetworkReply *)),
          SLOT(queryFinished(QNetworkReply *)));

  return initConfigurations();
}
