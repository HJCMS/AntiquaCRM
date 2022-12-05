// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooks.h"
#include "abebooksdocument.h"

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
#include <QTextCodec>
#include <QTextEncoder>
#include <QTimer>

Abebooks::Abebooks(QObject *parent) : AntiquaCRM::APluginInterface{parent} {
  setObjectName(ABEBOOKS_CONFIG_PROVIDER);
  m_network = nullptr;
}

bool Abebooks::initConfigurations() {
  QUrl url;
  AntiquaCRM::ASettings cfg(this);
  cfg.beginGroup(ABEBOOKS_CONFIG_GROUP);
  url.setScheme("https");
  url.setHost(cfg.value("api_host", "orderupdate.abebooks.com").toString());
  url.setPort(cfg.value("api_port", 10003).toInt());
  apiUser = cfg.value("api_user", QString()).toString();
  apiKey = cfg.value("api_key", QString()).toString();
  cfg.endGroup();
  apiUrl = url;
  return true;
}

AbeBooksDocument Abebooks::initDocument() {
  AbeBooksAccess ac;
  ac.user = apiUser;
  ac.key = apiKey;
  return AbeBooksDocument(ac);
}

const QUrl Abebooks::apiQuery(const QString &section) {
  QString action = configProvider().toLower();
  action.append("_");
  action.append(section.toLower());
  action.append(".xml");

  actionsCookie = QNetworkCookie("action", action.toLocal8Bit());
  actionsCookie.setDomain(apiUrl.host());
  actionsCookie.setSecure(true);

  return apiUrl;
}

const QString Abebooks::dateString(const QDate &date) const {
  return date.toString(Qt::ISODate);
}

void Abebooks::prepareResponse(const QJsonDocument &js) { Q_UNUSED(js); }

void Abebooks::prepareResponse(const QDomDocument &xml) {
  QString fileName = configProvider();
  if (!actionsCookie.name().isNull()) {
    fileName = QString(actionsCookie.value());
  }

  AntiquaCRM::ASharedCacheFiles cacheFile;
  cacheFile.storeTempFile(fileName.toLower(), xml.toString(1));

  emit sendQueryFinished();
}

void Abebooks::queryFinished(QNetworkReply *reply) {
  if (reply->error() != QNetworkReply::NoError) {
    emit sendErrorResponse(AntiquaCRM::WARNING,
                           tr("AbeBooks response with errors!"));
  }
}

void Abebooks::orderUpdateAction(const QJsonObject &options) {
  if (!options.contains("provider")) {
    qWarning("Invalid caller for Abebooks::postOperation!");
    return;
  }
  // Prüfen ob es für ihn ist!
  if (options.value("provider").toString() != configProvider())
    return;

  qDebug() << Q_FUNC_INFO << options;
}

AntiquaCRM::UpdateDialog *Abebooks::actionsDialog(QWidget *parent) {
  Q_UNUSED(parent);
  qDebug() << Q_FUNC_INFO << "TODO";
  return nullptr;
}

const QString Abebooks::configProvider() const {
  return QString(ABEBOOKS_CONFIG_PROVIDER).toLower();
}

const QString Abebooks::displayName() const {
  return QString(ABEBOOKS_CONFIG_PROVIDER).trimmed();
}

bool Abebooks::createInterface(QObject *parent) {
  Q_UNUSED(parent);
  m_network = new AntiquaCRM::ANetworker(AntiquaCRM::XML_QUERY, this);
  connect(m_network, SIGNAL(sendXmlResponse(const QDomDocument &)),
          SLOT(prepareResponse(const QDomDocument &)));
  connect(m_network, SIGNAL(sendContentCodec(QTextCodec *)),
          SLOT(setContentDecoder(QTextCodec *)));
  connect(m_network, SIGNAL(finished(QNetworkReply *)),
          SLOT(queryFinished(QNetworkReply *)));

  return initConfigurations();
}
