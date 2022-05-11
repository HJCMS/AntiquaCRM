// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "networker.h"
#include "applsettings.h"

#include <QHttpMultiPart>
#include <QHttpPart>
#include <QLocale>

Networker::Networker(QObject *parent) : QNetworkAccessManager{parent} {
  setObjectName("antiquacrm_networker");

  config = new ApplSettings(this);

  m_codec = QTextCodec::codecForLocale();
  QString charset = QString::fromLocal8Bit(m_codec->name());
  if (!charset.isEmpty()) {
    p_charset = charset.replace("-", "").toLower();
  }

  connect(this, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(slotFinished(QNetworkReply *)));
}

const QSslConfiguration Networker::sslConfigguration() {
  QSslConfiguration cfg;
  QString ca_bundle = config->value("ssloptions/ssl_bundle").toString();
  if (!cfg.addCaCertificates(ca_bundle, QSsl::Pem)) {
    qWarning("Missing ca-bundle to import!");
  }
  cfg.setProtocol(QSsl::DtlsV1_2OrLater);
  return cfg;
}

const QByteArray Networker::languageRange() {
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

const QByteArray Networker::acceptJson() {
  QString accept("application/ld+json,application/json,*/*;q=0.1");
  return accept.toLocal8Bit();
}

const QByteArray Networker::headerJson() {
  QString header("application/json");
  if (!p_charset.isEmpty())
    header.append("; charset=" + p_charset);

  return header.toLocal8Bit();
}

void Networker::slotError(QNetworkReply::NetworkError error) {
  switch (error) {
  case QNetworkReply::ConnectionRefusedError:
    qWarning("Networker: Connection Refused Error");
    return;

  case QNetworkReply::TimeoutError:
    qWarning("Networker: Timeout Error");
    return;

  case QNetworkReply::HostNotFoundError:
    qWarning("Networker: Host NotFound Error");
    return;

  default:
    qWarning("Networker: Unknown Error:%s", qPrintable(QString::number(error)));
    return;
  }
}

void Networker::slotFinished(QNetworkReply *reply) {
  bool errors = false;
  if (reply->error() != QNetworkReply::NoError) {
    slotError(reply->error());
    errors = true;
  }
  emit requestFinished(errors);
}

void Networker::slotSslErrors(const QList<QSslError> &list) {
  for (int i = 0; i < list.count(); i++) {
    QSslError ssl_error = list.at(i);
    qDebug() << Q_FUNC_INFO << ssl_error.errorString();
  }
}

QNetworkReply *Networker::jsonPostRequest(const QUrl &url, const QString &name,
                                          const QJsonDocument &body) {
  QNetworkRequest request(url);
  request.setRawHeader(QByteArray("Accept-Language"), languageRange());
  request.setRawHeader(QByteArray("Accept"), acceptJson());

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

QNetworkReply *Networker::jsonGetRequest(const QUrl &url) {
  // https://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
  QNetworkRequest request(url);
  request.setRawHeader(QByteArray("Accept-Language"), languageRange());
  request.setRawHeader(QByteArray("Accept"), acceptJson());

  reply = get(request);
  connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  return reply;
}

Networker::~Networker() {
  if (reply != nullptr)
    reply->deleteLater();
}
