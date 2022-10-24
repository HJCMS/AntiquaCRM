// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "anetworkrequest.h"
#include "aglobal.h"
#include "asettings.h"

#include <QLocale>

namespace AntiquaCRM {

#ifdef Q_OS_WIN
static const QString applCaBundlePath() {
  QFileInfo info(QDir::current(), "curl-ca-bundle.crt");
  if (info.isReadable()) {
    return info.filePath();
  }
  return QString();
}
#endif

ANetworkRequest::ANetworkRequest(const QUrl &remoteUrl)
    : QNetworkRequest{remoteUrl} {

  QUrl checkUrl = url();
  if (checkUrl.host().isEmpty() || checkUrl.scheme().isEmpty()) {
    qWarning("Invalid URL parameter for class ANetworker request!");
    qInfo("- use setHost() and setScheme() in QUrl for ANetworker!");
  }
  if (checkUrl.scheme().contains("https") && !checkUrl.host().isEmpty()) {
    QSslConfiguration sslConfig = sslConfigguration();
    setSslConfiguration(sslConfig);
  }
}

const QByteArray ANetworkRequest::antiquaCharset() {
  return QByteArray(ANTIQUACRM_TEXTCODEC).toLower();
}

const QSslConfiguration ANetworkRequest::sslConfigguration() {
  ASettings config;
  QSslConfiguration cfg;
  QString ca_bundle;
#ifdef Q_OS_WIN
  ca_bundle = config.value("ssl/ca_bundle", applCaBundlePath()).toString();
#else
  ca_bundle = config.value("ssl/ca_bundle").toString();
#endif
  if (!cfg.addCaCertificates(ca_bundle, QSsl::Pem)) {
    qWarning("ANetworkRequest: Missing ssl/ca_bundle PEM to import!");
  }
  return cfg;
}

void ANetworkRequest::setHeaderAcceptLanguage() {
  QLocale locale = QLocale::system();
  QString str = locale.bcp47Name();
  str.append(", ");
  str.append(locale.name());
  str.append(".");
  str.append(antiquaCharset());
  str.append("; q=0.8, en;q=0.7");
  setRawHeader(QByteArray("Accept-Language"), str.toLocal8Bit());
}

void ANetworkRequest::setHeaderUserAgent() {
  QLocale locale = QLocale::system();
  QString iso639_1 = locale.bcp47Name();
  QString iso3166 = locale.name();
  QString str("Mozilla/5.0 (compatible; ");
  str.append(ANTIQUACRM_USERAGENT);
  str.append(" " + iso639_1 + ", " + iso3166);
  str.append(")");
  setRawHeader(QByteArray("User-Agent"), str.toLocal8Bit());
}

void ANetworkRequest::setHeaderAcceptJson() {
  QByteArray accept("application/ld+json,application/json,text/*;q=0.1");
  setRawHeader(QByteArray("Accept"), accept);
}

void ANetworkRequest::setHeaderContentTypeJson() {
  QByteArray contentType("application/json; charset=");
  contentType.append(antiquaCharset());
  setRawHeader(QByteArray("Content-Type"), contentType);
}

void ANetworkRequest::setHeaderCacheControl(const QByteArray &cache) {
  setRawHeader(QByteArray("Cache-Control"), cache);
}

void ANetworkRequest::setHeaderContentLength(qint64 size) {
  QByteArray arr = QByteArray::number(size);
  setRawHeader(QByteArray("Content-Length"), arr);
}

}; // namespace AntiquaCRM
