// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "anetworker.h"
#include "aglobal.h"
#include "asettings.h"

#include <QHttpMultiPart>
#include <QHttpPart>
#include <QLocale>

namespace AntiquaCRM {

ANetworker::ANetworker(QObject *parent) : QNetworkAccessManager{parent} {
  setObjectName("antiquacrm_networker");

  m_cfg = new ASettings(this);

  m_codec = QTextCodec::codecForLocale();
  QString charset = QString::fromLocal8Bit(m_codec->name());
  if (!charset.isEmpty()) {
    p_charset = charset.replace("-", "").toLower();
  }

  connect(this, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(slotFinished(QNetworkReply *)));
}

const QSslConfiguration ANetworker::sslConfigguration() {
  QSslConfiguration cfg;
  QString ca_bundle = m_cfg->value("ssloptions/ssl_bundle").toString();
  if (!cfg.addCaCertificates(ca_bundle, QSsl::Pem)) {
    qWarning("Missing ca-bundle to import!");
  }
  return cfg;
}

const QByteArray ANetworker::languageRange() {
  // https://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
  QLocale locale = QLocale::system();
  QString al = locale.bcp47Name();
  al.append(", ");
  al.append(locale.name());
  al.append(".");
  al.append(p_charset);
  al.append("; q=0.8, en;q=0.7");
  return al.toLocal8Bit();
}

const QByteArray ANetworker::acceptJson() {
  QString accept("application/ld+json,application/json,text/*;q=0.1");
  return accept.toLocal8Bit();
}

const QByteArray ANetworker::headerJson() {
  QString header("application/json");
  if (!p_charset.isEmpty())
    header.append("; charset=" + p_charset);

  return header.toLocal8Bit();
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

void ANetworker::slotSslErrors(const QList<QSslError> &list) {
  for (int i = 0; i < list.count(); i++) {
    QSslError ssl_error = list.at(i);
    qDebug() << Q_FUNC_INFO << ssl_error.errorString();
  }
}

const QByteArray ANetworker::AntiquaUserAgent() {
  QLocale locale = QLocale::system();
  QString iso639_1 = locale.bcp47Name();
  QString iso3166 = locale.name();
  QString str("Mozilla/5.0 (compatible; ");
  str.append(ANTIQUACRM_USERAGENT);
  str.append(" " + iso639_1 + ", " + iso3166);
  str.append(") AppleWebKit (KHTML, like Gecko)");
  return str.toLocal8Bit();
}

QNetworkReply *ANetworker::jsonPostRequest(const QUrl &url, /* Response URL */
                                           const QJsonDocument &body) {
  QNetworkRequest request(url);
  request.setRawHeader(QByteArray("User-Agent"), AntiquaUserAgent());
  request.setRawHeader(QByteArray("Accept-Language"), languageRange());
  request.setRawHeader(QByteArray("Accept"), acceptJson());
  request.setRawHeader(QByteArray("Cache-Control"),
                       QByteArray("no-cache, private"));
  request.setRawHeader(QByteArray("Content-Type"),
                       QByteArray("application/json"));

  if (url.scheme().contains("https") && !url.host().isEmpty()) {
    QSslConfiguration sslConfig = sslConfigguration();
    request.setSslConfiguration(sslConfig);
  }

  QByteArray data = body.toJson(QJsonDocument::Compact);
  QByteArray size = QString::number(data.size()).toLocal8Bit();
  request.setRawHeader(QByteArray("Content-Length"), size);

  reply = post(request, data);

  connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  return reply;
}

QNetworkReply *ANetworker::jsonMultiPartRequest(/* Multipart */
                                                const QUrl &url,
                                                const QString &name,
                                                const QJsonDocument &body) {
  QNetworkRequest request(url);
  request.setRawHeader(QByteArray("User-Agent"), AntiquaUserAgent());
  request.setRawHeader(QByteArray("Accept-Language"), languageRange());
  request.setRawHeader(QByteArray("Accept"), acceptJson());
  request.setRawHeader(QByteArray("Cache-Control"),
                       QByteArray("no-cache, private"));
  request.setTransferTimeout((tranfer_timeout * 1000));
  if (url.scheme().contains("https") && !url.host().isEmpty()) {
    QSslConfiguration sslConfig = sslConfigguration();
    request.setSslConfiguration(sslConfig);
  }

  QByteArray data = body.toJson(QJsonDocument::Compact);
  QHttpMultiPart *formData = new QHttpMultiPart(QHttpMultiPart::FormDataType);
  QHttpPart json_part;
  json_part.setHeader(QNetworkRequest::ContentTypeHeader, headerJson());
  json_part.setHeader(QNetworkRequest::ContentDispositionHeader,
                      "form-data; name=\"" + name + "\"");
  json_part.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
  json_part.setBody(data);
  formData->append(json_part);

  reply = post(request, formData);
  // move to parent
  formData->setParent(reply);

  connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  return reply;
}

QNetworkReply *ANetworker::jsonGetRequest(const QUrl &url) {
  QNetworkRequest request(url);
  request.setRawHeader(QByteArray("User-Agent"), AntiquaUserAgent());
  request.setRawHeader(QByteArray("Accept-Language"), languageRange());
  request.setRawHeader(QByteArray("Accept"), acceptJson());
  request.setRawHeader(QByteArray("Cache-Control"),
                       QByteArray("no-cache, private"));
  request.setTransferTimeout((tranfer_timeout * 1000));
  if (url.scheme().contains("https") && !url.host().isEmpty()) {
    QSslConfiguration sslConfig = sslConfigguration();
    request.setSslConfiguration(sslConfig);
  }

  reply = get(request);
  connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  return reply;
}

ANetworker::~ANetworker() {
  if (reply != nullptr)
    reply->deleteLater();
}

}; // namespace AntiquaCRM
