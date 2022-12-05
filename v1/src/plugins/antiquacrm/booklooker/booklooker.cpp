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
    // TODO SIGNAL
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
  if (actionsCookie.value().contains("authenticate")) {
    QJsonObject obj = js.object();
    if (obj.value("status").toString().toLower() == "ok") {
      QString token = obj.value("returnValue").toString();
      setTokenCookie(token);
    }
    return;
  }

  QString fileName = configProvider();
  if (!actionsCookie.name().isNull()) {
    fileName = QString(actionsCookie.value());
  }

  AntiquaCRM::ASharedCacheFiles cacheFile;
  QString jsonData = js.toJson(QJsonDocument::Indented);
  cacheFile.storeTempFile(fileName.toLower(), jsonData);

  emit sendQueryFinished();
}

void Booklooker::prepareResponse(const QDomDocument &xml) { Q_UNUSED(xml); }

void Booklooker::queryFinished(QNetworkReply *reply) {
  if (reply->error() != QNetworkReply::NoError) {
    emit sendErrorResponse(AntiquaCRM::WARNING,
                           tr("Booklooker response with errors!"));
  }
}

void Booklooker::orderUpdateAction(const QJsonObject &options) {
  if (!options.contains("provider")) {
    qWarning("Invalid caller for Booklooker::postOperation!");
    return;
  }
  // Prüfen ob es für ihn ist!
  if (options.value("provider").toString() != configProvider())
    return;

  if (isCookieExpired())
    authenticate();

  QJsonObject action = options.value("plugin_operation").toObject();
  QString orderId = action.value("orderid").toString();

  if (action.contains("paymentstatus")) {
    // BEGIN PaymentStatus
    QString query_action;
    QUrlQuery query;
    query.addQueryItem("token", QString(authenticCookie.value()));
    query.addQueryItem("orderId", orderId);

    int status = action.value("paymentstatus").toInt();
    switch (static_cast<AntiquaCRM::PaymentStatus>(status)) {
    case AntiquaCRM::PaymentStatus::WAIT_FOR_PAYMENT:
      query_action = "order_status";
      query.addQueryItem("status", "WAITING_FOR_PAYMENT");
      break;

    case AntiquaCRM::PaymentStatus::SHIPMENT_CREATED:
      query_action = "order_status";
      query.addQueryItem("status", "READY_FOR_SHIPMENT");
      break;

    case AntiquaCRM::PaymentStatus::SHIPPED_WAIT_FOR_PAYMENT:
      query_action = "order_status";
      query.addQueryItem("status", "SHIPPED_WAITING_FOR_PAYMENT");
      break;

    case AntiquaCRM::PaymentStatus::SHIPPED_AND_PAID:
      query_action = "order_status";
      query.addQueryItem("status", "SHIPPED_AND_PAID");
      break;

    case AntiquaCRM::PaymentStatus::BUYER_NO_REACTION:
      query_action = "order_status";
      query.addQueryItem("status", "BUYER_NO_REACTION");
      break;

    case AntiquaCRM::PaymentStatus::ORDER_CANCELED:
      query_action = "order_cancel";
      break;

    default:
      break;
    };

    QUrl url = apiQuery(query_action);
    if (!query.isEmpty())
      url.setQuery(query);

#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << Qt::endl << "Abgeschaltet:" << url.toString();
#else
    m_network->putRequest(url, QByteArray());
#endif
  } // END PaymentStatus
}

AntiquaCRM::UpdateDialog *Booklooker::actionsDialog(QWidget *parent) {
  BooklookerDialog *m_d = new BooklookerDialog(parent);
  return m_d;
}

bool Booklooker::authenticationRequired() { return isCookieExpired(); }

const QString Booklooker::configProvider() const {
  return QString(BOOKLOOKER_CONFIG_PROVIDER).toLower();
}

const QString Booklooker::displayName() const {
  return QString(BOOKLOOKER_CONFIG_PROVIDER).trimmed();
}

bool Booklooker::createInterface(QObject *parent) {
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
