// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "networkrequest.h"

#include <AGlobal>
#include <QFileInfo>
#include <QLocale>

NetworkRequest::NetworkRequest(const QUrl &url) : QNetworkRequest{url} {
  if (url.host().isEmpty() || url.scheme().isEmpty()) {
    qWarning("Invalid URL parameter for NetworkRequest!");
  }

  QLocale locale = QLocale::system();
  QString iso639_1 = locale.bcp47Name();
  QString iso3166 = locale.name();
  QString str("Mozilla/5.0 (compatible; ");
  str.append(ANTIQUACRM_USERAGENT);
  str.append(" " + iso639_1 + ", " + iso3166);
  str.append(")");
  setRawHeader(QByteArray("User-Agent"), str.toLocal8Bit());

  QStringList list;
  list << "text/plain; q=0.1";
  list << "text/html; q=0.2";
  list << "text/xml; q=0.3";
  list << "application/json; q=0.4";
  QByteArray accept = list.join(", ").toLocal8Bit();
  setRawHeader(QByteArray("Accept"), accept);
  setRawHeader(QByteArray("Cache-Control"), QByteArray("no-cache,private"));

  setSslConfiguration(sslConfigguration());
  setTransferTimeout((tranfer_timeout * 1000));
}

const QSslConfiguration NetworkRequest::sslConfigguration() {
  QString bundle;
  QSslConfiguration cfg;
  QStringList targets;
  // CentOS (7,8)
  targets << "/etc/pki/tls/certs/ca-bundle.crt";
  // OpenSuSE
  targets << "/var/lib/ca-certificates/ca-bundle.pem";
  // Debian|Ubuntu
  targets << "/etc/ssl/certs/ca-certificates.crt";

  foreach (QString p, targets) {
    QFileInfo ca(p);
    if (ca.isReadable()) {
      bundle = ca.filePath();
      break;
    }
  }

  Q_ASSERT(!bundle.isEmpty());

  bool status = cfg.addCaCertificates(bundle, QSsl::Pem);
  Q_ASSERT(status);

  return cfg;
}

void NetworkRequest::setHeaderContentTypeJson() {
  QByteArray contentType("application/json charset=");
  contentType.append("utf-8");
  setRawHeader(QByteArray("Content-Type"), contentType);
}

void NetworkRequest::setHeaderContentTypeXml() {
  QByteArray contentType("application/xml charset=");
  contentType.append("utf-8");
  setRawHeader(QByteArray("Content-Type"), contentType);
}

void NetworkRequest::setHeaderContentLength(qint64 size) {
  QByteArray arr = QByteArray::number(size);
  setRawHeader(QByteArray("Content-Length"), arr);
}
