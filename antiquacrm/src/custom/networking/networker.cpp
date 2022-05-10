// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "networker.h"
#include "applsettings.h"

Networker::Networker(QObject *parent) : QNetworkAccessManager{parent} {
  setObjectName("antiquacrm_networker");

  config = new ApplSettings(this);

  connect(this, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(slotReplyFinished(QNetworkReply *)));
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

void Networker::slotReplyFinished(QNetworkReply *reply) {
}

void Networker::slotError(QNetworkReply::NetworkError error) {
  switch (error) {
  case QNetworkReply::ConnectionRefusedError:
    qDebug() << "Access ConnectionRefusedError";
    return;

  case QNetworkReply::TimeoutError:
    qDebug() << "Access TimeoutError";
    return;

  case QNetworkReply::HostNotFoundError:
    qDebug() << "Access HostNotFoundError";
    return;

  default:
    qDebug() << "Unknown Error:" << error;
    return;
  }
}

void Networker::slotSslErrors(const QList<QSslError> &list) {
  for (int i = 0; i < list.count(); i++) {
    QSslError ssl_error = list.at(i);
    qDebug() << Q_FUNC_INFO << ssl_error.errorString();
  }
}

const QNetworkRequest Networker::getRequest(const QUrl &url) {
  QNetworkRequest request(url);
  // https://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
  request.setRawHeader(QByteArray("Accept-Language"),
                       QByteArray("de, de_DE.utf8;q=0.8, en;q=0.7"));
  // grep -i json /etc/mime.types
  request.setRawHeader(QByteArray("Accept"),
                       QByteArray("text/*, application/json; charset=utf8"));

  return request;
}

Networker::~Networker() {}
