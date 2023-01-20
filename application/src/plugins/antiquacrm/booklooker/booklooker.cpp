// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklooker.h"
#include "booklookerdialog.h"

/**
 * @def DATE_FORMAT
 * @ingroup Providers SQL Statements
 */
#ifndef BOOKLOOKER_DATE_FORMAT
#define BOOKLOOKER_DATE_FORMAT "yyyy-MM-dd"
#endif

/** @brief Konfigurationsgruppe */
#ifndef BOOKLOOKER_CONFIG_GROUP
#define BOOKLOOKER_CONFIG_GROUP "provider/booklooker"
#endif

/** @brief Wird für Menüeintrag und Gruppenzuweisung benötigt! */
#ifndef BOOKLOOKER_CONFIG_PROVIDER
#define BOOKLOOKER_CONFIG_PROVIDER "Booklooker"
#endif

#ifndef BOOKLOOKER_API_VERSION
#define BOOKLOOKER_API_VERSION "2.0"
#endif

// Headers
#include <AntiquaCRM>
#include <QDateTime>
#include <QDebug>
#include <QDialog>
#include <QTimeZone>
#include <QUrlQuery>

Booklooker::Booklooker(QObject *parent) : AntiquaCRM::APluginInterface{parent} {
  setObjectName(BOOKLOOKER_CONFIG_PROVIDER);
  m_network = nullptr;
}

bool Booklooker::initConfigurations() {
  QUrl url;
  AntiquaCRM::ASettings cfg(this);
  cfg.beginGroup(BOOKLOOKER_CONFIG_GROUP);
  url.setScheme("https");
  url.setHost(cfg.value("api_host", "api.booklooker.de").toString());
  apiKey = cfg.value("api_key", QString()).toString();
  cfg.endGroup();
  apiUrl = url;
  return true;
}

const QUrl Booklooker::apiQuery(const QString &section) {
  QString p("/");
  p.append(BOOKLOOKER_API_VERSION);
  p.append("/");
  p.append(section);

  QUrl url(apiUrl);
  url.setPath(p);

  return url;
}

const QString Booklooker::dateString(const QDate &date) const {
  return date.toString(BOOKLOOKER_DATE_FORMAT);
}

void Booklooker::setTokenCookie(const QString &token) {
  QDateTime dt = QDateTime::currentDateTime();
  dt.setTimeSpec(Qt::UTC);
  qint64 cookie_lifetime = (9 * 60);
  authenticCookie = QNetworkCookie("token", token.toLocal8Bit());
  authenticCookie.setDomain(apiUrl.host());
  authenticCookie.setSecure(true);
  authenticCookie.setExpirationDate(dt.addSecs(cookie_lifetime));
  if (!authenticCookie.value().isNull()) {
    qInfo("New Token add (%s)", qPrintable(authenticCookie.value()));
    if (cacheCookie.value().isNull())
      return;

    actionsCookie.setValue(QByteArray());
    QJsonDocument js = QJsonDocument::fromJson(cacheCookie.value());
    orderUpdateAction(js.object());
  }
}

bool Booklooker::isCookieExpired() {
  if (authenticCookie.value().isNull())
    return true;

  QDateTime dt = QDateTime::currentDateTime();
  dt.setTimeSpec(Qt::UTC);
  return (authenticCookie.expirationDate() <= dt);
}

void Booklooker::authenticate() {
  QUrl url = apiQuery("authenticate");
  QString pd("apiKey=");
  pd.append(apiKey);

  actionsCookie = QNetworkCookie("action", "authenticate");
  actionsCookie.setDomain(url.host());
  actionsCookie.setSecure(true);

  m_network->loginRequest(url, pd.toLocal8Bit());
}

void Booklooker::prepareResponse(const QJsonDocument &js) {
  QJsonObject obj = js.object();
  if (actionsCookie.value().contains("authenticate")) {
    if (obj.value("status").toString().toLower() == "ok") {
      QString token = obj.value("returnValue").toString();
      setTokenCookie(token);
    }
    return;
  }

  if (obj.value("status").toString().toLower() != "ok") {
    QString info = obj.value("returnValue").toString();
    emit sendErrorResponse(AntiquaCRM::Message::WARNING, info);
  }

  QJsonValue returnValue = obj.value("returnValue");
  if (returnValue.type() == QJsonValue::String) {
    emit sendActionFinished(returnValue.toString());
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << obj;
#endif

  cacheCookie.setValue(QByteArray());
  actionsCookie.setValue(QByteArray());

  emit sendQueryFinished();
}

void Booklooker::prepareResponse(const QDomDocument &xml) { Q_UNUSED(xml); }

void Booklooker::queryFinished(QNetworkReply *reply) {
  if (reply->error() != QNetworkReply::NoError) {
    emit sendErrorResponse(AntiquaCRM::WARNING,
                           tr("Booklooker response with errors!"));
  }
}

void Booklooker::orderUpdateAction(const QJsonObject &data) {
  if (isCookieExpired()) {
    QJsonDocument js(data);
    cacheCookie = QNetworkCookie("queue", js.toJson(QJsonDocument::Compact));
    cacheCookie.setDomain(apiUrl.host());
    cacheCookie.setSecure(true);
    authenticate();
    return;
  }

  QJsonObject query = data.value("query").toObject();
  if(query.value("orderId").toString().isEmpty()) {
    emit sendQueryAborted();
    return;
  }

  // It's no longer a queued request!
  cacheCookie.setValue(QByteArray());

  QUrlQuery q;
  q.addQueryItem("token", QString(authenticCookie.value()));
  q.addQueryItem("orderId", query.value("orderId").toString());
  q.addQueryItem("status", query.value("status").toString());

  QString action = data.value("action").toString();
  QUrl url = apiQuery(action);
  if (!q.isEmpty())
    url.setQuery(q);

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << data << url.toString();
#endif

  m_network->putRequest(url, QByteArray());
}

bool Booklooker::authenticationRequired() { return isCookieExpired(); }

AntiquaCRM::UpdateDialog *Booklooker::updateDialog(QWidget *parent) {
  BooklookerDialog *m_d = new BooklookerDialog(parent);
  return m_d;
}

const QString Booklooker::configProvider() const {
  return QString(BOOKLOOKER_CONFIG_PROVIDER).toLower();
}

const QString Booklooker::displayName() const {
  return QString(BOOKLOOKER_CONFIG_PROVIDER).trimmed();
}

bool Booklooker::createInterface(QObject *parent) {
  if (parent == nullptr)
    return false;

  m_network = new AntiquaCRM::ANetworker(AntiquaCRM::JSON_QUERY, this);
  connect(m_network, SIGNAL(sendJsonResponse(const QJsonDocument &)),
          SLOT(prepareResponse(const QJsonDocument &)));
  connect(m_network, SIGNAL(sendContentCodec(QTextCodec *)),
          SLOT(setContentDecoder(QTextCodec *)));
  connect(m_network, SIGNAL(finished(QNetworkReply *)),
          SLOT(queryFinished(QNetworkReply *)));

  return initConfigurations();
}
