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

static const QString to_iso8859_1(const QString &str) {
  QTextCodec *codec = QTextCodec::codecForLocale();
  QTextEncoder encoder(codec);
  return QString(encoder.fromUnicode(str));
}

Abebooks::Abebooks(QObject *parent) : AntiquaCRM::APluginInterface{parent} {
  setObjectName(ABEBOOKS_CONFIG_PROVIDER);
  m_network = new AntiquaCRM::ANetworker(AntiquaCRM::XML_QUERY, this);
  initConfigurations();

  connect(m_network, SIGNAL(sendXmlResponse(const QDomDocument &)),
          SLOT(prepareResponse(const QDomDocument &)));
  connect(m_network, SIGNAL(sendContentCodec(QTextCodec *)),
          SLOT(setContentDecoder(QTextCodec *)));
  connect(m_network, SIGNAL(finished(QNetworkReply *)),
          SLOT(queryFinished(QNetworkReply *)));
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

AbeBooksDocument Abebooks::initDocument() {
  AbeBooksAccess ac;
  ac.user = apiUser;
  ac.key = apiKey;
  return AbeBooksDocument(ac);
}

const QUrl Abebooks::apiQuery(const QString &section) {
  QString action(section.toLower());
  action.prepend("_");
  action.prepend(configProvider());

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
  fileName.append(".xml");
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

void Abebooks::queryNewOrders(int waitSecs) {
  Q_UNUSED(waitSecs);
  QString operation("getAllNewOrders");
  AbeBooksDocument doc = initDocument();
  doc.createAction(operation);

  QUrl url(apiQuery(operation));
  m_network->xmlPostRequest(url, doc);
}

void Abebooks::queryOrder(const QString &orderId) {
  QString operation("getOrder");
  AbeBooksDocument doc = initDocument();
  doc.createAction(operation);
  QDomElement e = doc.createElement("purchaseOrder");
  e.setAttribute("id", orderId.trimmed());
  doc.documentElement().appendChild(e);

  QUrl url(apiQuery(operation));
  m_network->xmlPostRequest(url, doc);
}

const QString Abebooks::configProvider() const {
  return QString(ABEBOOKS_CONFIG_PROVIDER).toLower();
}

const QString Abebooks::displayName() const {
  return QString(ABEBOOKS_CONFIG_PROVIDER).trimmed();
}

const AntiquaCRM::AProviderOrders Abebooks::getOrders() const {
  AntiquaCRM::AProviderOrders booking;
  QString fileName = configProvider();
  if (!actionsCookie.name().isNull()) {
    fileName = QString(actionsCookie.value());
  }

  if (fileName.isEmpty())
    return booking;

  fileName.append(".xml");
  AntiquaCRM::ASharedCacheFiles cacheFile;
  QString data = cacheFile.getTempFile(fileName.toLower());
  if (data.isEmpty())
    return booking;

  return booking;
}

bool Abebooks::createInterface(QObject *parent) {
  Q_UNUSED(parent);
  return true;
}
