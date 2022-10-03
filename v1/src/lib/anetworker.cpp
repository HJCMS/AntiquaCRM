// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "anetworker.h"
#include "anetworkrequest.h"

#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonParseError>

namespace AntiquaCRM {

ANetworker::ANetworker(QObject *parent) : QNetworkAccessManager{parent} {
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
  emit requestFinished(errors);
}

void ANetworker::readResponse() {
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

  QByteArray data = m_reply->readAll();
  if (data.isNull()) {
    qWarning("ANetorker: No Data responsed!");
    return;
  }

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

  emit jsonResponse(doc);
}

void ANetworker::slotSslErrors(const QList<QSslError> &list) {
  for (int i = 0; i < list.count(); i++) {
    QSslError ssl_error = list.at(i);
    qDebug() << Q_FUNC_INFO << ssl_error.errorString();
  }
}

QNetworkReply *ANetworker::jsonPostRequest(const QUrl &url, /* Response URL */
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

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(readResponse()));

  return m_reply;
}

QNetworkReply *ANetworker::jsonMultiPartRequest(/* Multipart */
                                                const QUrl &url,
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

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(readResponse()));

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

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(readResponse()));

  return m_reply;
}

ANetworker::~ANetworker() {
  if (m_reply != nullptr)
    m_reply->deleteLater();
}

}; // namespace AntiquaCRM
