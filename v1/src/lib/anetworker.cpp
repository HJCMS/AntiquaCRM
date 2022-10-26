// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "anetworker.h"
#include "anetworkrequest.h"

#ifndef ANTIQUACRM_NETWORK_DEBUG
#define ANTIQUACRM_NETWORK_DEBUG false
#endif

#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonParseError>

namespace AntiquaCRM {

ANetworker::ANetworker(AntiquaCRM::PluginQueryType type, QObject *parent)
    : QNetworkAccessManager{parent}, queryType{type} {
  setObjectName("antiquacrm_networker");
  connect(this, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(slotFinished(QNetworkReply *)));
}

void ANetworker::slotError(QNetworkReply::NetworkError error) {
  switch (error) {
  case QNetworkReply::ConnectionRefusedError:
    qWarning("Network: Connection Refused Error");
    return;

  case QNetworkReply::TimeoutError:
    qWarning("Network: Timeout Error");
    return;

  case QNetworkReply::HostNotFoundError:
    qWarning("Network: Host NotFound Error");
    return;

  case QNetworkReply::RemoteHostClosedError:
    qWarning("Network: RemoteHost Closed Error");
    return;

  case QNetworkReply::OperationCanceledError:
    qWarning("Network: Operation Canceled Error");
    return;

  case QNetworkReply::InsecureRedirectError:
    qWarning("Network: Insecure Redirect Error");
    return;

  case QNetworkReply::InternalServerError:
    qWarning("Network: Internal Server Error");
    return;

  default:
    qWarning("Network: Unknown Error (%s)", qPrintable(QString::number(error)));
    return;
  }
}

void ANetworker::slotFinished(QNetworkReply *reply) {
  bool errors = false;
  if (reply->error() != QNetworkReply::NoError) {
    slotError(reply->error());
    errors = true;
  }
  emit sendFinishedWithErrors(errors);
}

void ANetworker::slotReadResponse() {
  if (m_reply == nullptr)
    return;

  bool success = true;
  if (m_reply->error() != QNetworkReply::NoError) {
    slotError(m_reply->error());
    success = false;
  }

  if (!m_reply->bytesAvailable()) {
    qWarning("ANetorker: No Data responsed!");
    return;
  }

#if ANTIQUACRM_NETWORK_DEBUG
  foreach (QByteArray a, m_reply->rawHeaderList()) {
    qInfo("%s: %s", a.constData(), m_reply->rawHeader(a).constData());
  }
#endif

  QByteArray data = m_reply->readAll();
  if (data.isNull()) {
    qWarning("ANetorker: No Data responsed!");
    return;
  }

  // JSON Request
  if (queryType == AntiquaCRM::JSON_QUERY) {
    QJsonParseError parser;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parser);
    if (parser.error != QJsonParseError::NoError) {
      qWarning("Json Parse Error:(%s)!", qPrintable(parser.errorString()));
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << QString::fromLocal8Bit(data);
#endif
      return;
    }
    data.clear();
    emit sendJsonResponse(doc);
    return;
  }
  // XML/SOAP Request
  if (queryType == AntiquaCRM::XML_QUERY) {
    QDomDocument xml("response");
    QString errorMsg = QString();
    int errorLine = 0;
    int errorColumn = 0;
    if (!xml.setContent(data, false, &errorMsg, &errorLine, &errorColumn)) {
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << errorMsg << errorLine << errorColumn;
#else
      qWarning("Returned XML is not well formatted!");
#endif
      emit sendFinishedWithErrors(true);
      return;
    }
    data.clear();
    emit sendXmlResponse(xml);
    return;
  }

  qWarning("Unknown Response type!");
}

void ANetworker::slotSslErrors(const QList<QSslError> &list) {
  for (int i = 0; i < list.count(); i++) {
    QSslError ssl_error = list.at(i);
    qDebug() << Q_FUNC_INFO << ssl_error.errorString();
  }
}

QNetworkReply *ANetworker::loginRequest(const QUrl &url,
                                        const QByteArray &data) {
  ANetworkRequest request(url);
  request.setHeaderUserAgent();
  request.setHeaderCacheControl();
  request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
  m_reply = post(request, data);

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadResponse()));

  return m_reply;
}

QNetworkReply *ANetworker::jsonPostRequest(const QUrl &url,
                                           const QJsonDocument &body) {
  ANetworkRequest request(url);
  request.setHeaderUserAgent();
  request.setHeaderAcceptLanguage();
  request.setHeaderAcceptJson();
  request.setHeaderCacheControl();
  request.setHeaderContentTypeJson();

  QByteArray data = body.toJson(QJsonDocument::Compact);
  request.setHeaderContentLength(data.size());

  m_reply = post(request, data);

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadResponse()));

  return m_reply;
}

QNetworkReply *ANetworker::xmlPostRequest(const QUrl &url,
                                          const QDomDocument &body) {
  ANetworkRequest request(url);
  request.setHeaderUserAgent();
  request.setHeaderAcceptLanguage();
  request.setHeaderAcceptXml();
  request.setHeaderCacheControl();
  request.setHeaderContentTypeXml();

  QByteArray data = body.toByteArray(-1);
  request.setHeaderContentLength(data.size());

  m_reply = post(request, data);

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadResponse()));

  return m_reply;
}

QNetworkReply *ANetworker::jsonMultiPartRequest(const QUrl &url,
                                                const QString &name,
                                                const QJsonDocument &body) {
  ANetworkRequest request(url);
  request.setHeaderUserAgent();
  request.setHeaderAcceptLanguage();
  request.setHeaderAcceptJson();
  request.setHeaderCacheControl();
  request.setTransferTimeout((tranfer_timeout * 1000));

  QByteArray data = body.toJson(QJsonDocument::Compact);
  QHttpMultiPart *formData = new QHttpMultiPart(QHttpMultiPart::FormDataType);
  QHttpPart json_part;
  json_part.setHeader(QNetworkRequest::ContentTypeHeader,
                      "application/json; charset=" +
                          ANetworkRequest::antiquaCharset());
  json_part.setHeader(QNetworkRequest::ContentDispositionHeader,
                      "form-data; name=\"" + name + "\"");
  json_part.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
  json_part.setBody(data);
  formData->append(json_part);

  m_reply = post(request, formData);
  // move to parent
  formData->setParent(m_reply);

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadResponse()));

  return m_reply;
}

QNetworkReply *ANetworker::jsonGetRequest(const QUrl &url) {
  ANetworkRequest request(url);
  request.setHeaderUserAgent();
  request.setHeaderAcceptLanguage();
  request.setHeaderAcceptJson();
  request.setHeaderCacheControl();
  request.setTransferTimeout((tranfer_timeout * 1000));

  m_reply = get(request);
  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadResponse()));

  return m_reply;
}

ANetworker::~ANetworker() {
  if (m_reply != nullptr)
    m_reply->deleteLater();
}

}; // namespace AntiquaCRM
