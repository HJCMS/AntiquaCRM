// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "anetworkrequest.h"
#include "aglobal.h"
#include "asettings.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QLocale>
#include <QRegularExpression>

namespace AntiquaCRM {

ANetworkRequest::ANetworkRequest(const QUrl &remoteUrl)
    : QNetworkRequest{remoteUrl} {
  setPriority(QNetworkRequest::HighPriority);
  setAttribute(QNetworkRequest::RedirectPolicyAttribute,
               QNetworkRequest::NoLessSafeRedirectPolicy);

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

const QString ANetworkRequest::findCaBundleFile() const {
  QString ca_bundle;
  QRegularExpression pattern("^(curl\\-)?ca[\\-_](bundle|certificate)$");
  QStringList filter({"*.crt", "*.pem"});
  QStringList dirs(qApp->applicationDirPath());
#ifdef Q_OS_UNIX
  dirs << "/etc/pki/tls/certs";
  dirs << "/var/lib/ca-certificates";
  dirs << "/etc/ssl/certs";
#endif
  foreach (QString p, dirs) {
    foreach (QFileInfo i, QDir(p).entryInfoList(filter, QDir::Files)) {
      if (i.baseName().contains(pattern) && i.isReadable()) {
        ca_bundle = i.filePath();
        break;
      }
    }
  }
  return ca_bundle;
}

const QSslConfiguration ANetworkRequest::sslConfigguration() {
  QString ca_bundle = findCaBundleFile();
  if (ca_bundle.isEmpty()) {
    AntiquaCRM::ASettings cfg;
    ca_bundle = cfg.value("Default/ssl_bundle").toString();
  }

  QSslConfiguration cfg;
  if (!cfg.addCaCertificates(ca_bundle, QSsl::Pem)) {
    qWarning("ANetworkRequest: Missing ssl/ca_bundle PEM to import!");
    qDebug() << Q_FUNC_INFO << ca_bundle;
  }
  return cfg;
}

void ANetworkRequest::setHeaderAcceptLanguage(const QByteArray &charset) {
  QLocale locale = QLocale::system();
  QString str = locale.bcp47Name();
  str.append(", ");
  str.append(locale.name());
  str.append(".");
  str.append(charset);
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

void ANetworkRequest::setHeaderAcceptText() {
  QStringList list;
  list << "text/plain; q=0.1";
  list << "text/html; q=0.2";
  list << "text/xml; q=0.3";
  list << "application/json; q=0.4";
  QByteArray accept = list.join(", ").toLocal8Bit();
  setRawHeader(QByteArray("Accept"), accept);
}

void ANetworkRequest::setHeaderAcceptJson() {
  QStringList list;
  list << "text/plain; q=0.1";
  list << "application/json; q=0.2";
  QByteArray accept = list.join(", ").toLocal8Bit();
  setRawHeader(QByteArray("Accept"), accept);
}

void ANetworkRequest::setHeaderContentTypeJson(const QByteArray &charset) {
  QByteArray contentType("application/json charset=");
  contentType.append(charset);
  setRawHeader(QByteArray("Content-Type"), contentType);
}

void ANetworkRequest::setHeaderContentTypeXml(const QByteArray &charset) {
  QByteArray contentType("application/xml charset=");
  contentType.append(charset);
  setRawHeader(QByteArray("Content-Type"), contentType);
}

void ANetworkRequest::setHeaderCacheControl(const QByteArray &cache) {
  QByteArray data = cache.isNull() ? ("no-cache,private") : cache;
  if (QString(data).contains("no-cache", Qt::CaseInsensitive)) {
    setAttribute(QNetworkRequest::CacheLoadControlAttribute,
                 QNetworkRequest::PreferNetwork);
  }
  setRawHeader(QByteArray("Cache-Control"), data);
}

void ANetworkRequest::setHeaderContentLength(qint64 size) {
  QByteArray arr = QByteArray::number(size);
  setRawHeader(QByteArray("Content-Length"), arr);
}

}; // namespace AntiquaCRM
