// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksrequester.h"
#include "abebooksconfig.h"
#include "abebooksdocument.h"
#include "applsettings.h"

#include <QFileInfo>
#include <QSslConfiguration>
#include <QTextCodec>
#include <QTextEncoder>

static const QByteArray applCaBundlePath() {
#ifdef Q_OS_WIN
  // Ist Normalerweise im Paket enthalten!
  QFileInfo info(QDir::current(), "curl-ca-bundle.crt");
  if (info.isReadable()) {
    return info.filePath().toLocal8Bit();
  }
#endif
  return QByteArray();
}

static const QByteArray userAgentString() {
  QLocale locale = QLocale::system();
  QString iso639_1 = locale.bcp47Name();
  QString iso3166 = locale.name();
  QString str("Mozilla/5.0 (compatible; ");
  str.append(ANTIQUACRM_DISPLAYNAME);
  str.append("/");
  str.append(ANTIQUACRM_VERSION);
  str.append(" " + iso639_1 + ", " + iso3166);
  str.append(") AppleWebKit (KHTML, like Gecko)");
  return str.toLocal8Bit();
}

const QByteArray languageRange() {
  // https://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
  QLocale locale = QLocale::system();
  QString al = locale.bcp47Name();
  al.append(", ");
  al.append(locale.name());
  al.append("; q=0.8, en;q=0.7");
  return al.toLocal8Bit();
}

AbeBooksRequester::AbeBooksRequester(QObject *parent) : QObject{parent} {
  config = new ApplSettings(this);
  m_networker = new QNetworkAccessManager(this);
}

const QNetworkRequest AbeBooksRequester::initRequest() {
  QUrl url;
  config->beginGroup(CONFIG_GROUP);
  url.setScheme(config->value("api_scheme", "https").toString());
  url.setHost(config->value("api_host", "orderupdate.abebooks.com").toString());
  url.setPort(config->value("api_port", 10003).toInt());
  config->endGroup();

  QNetworkRequest req(url);
  req.setPeerVerifyName(url.host());
  req.setRawHeader("User-Agent", userAgentString());
  req.setRawHeader("Accept-Language", languageRange());
  req.setRawHeader("Accept", "application/xml,text/xml,text/*;q=0.1");
  req.setRawHeader("Cache-Control", "no-cache, private");
  req.setRawHeader("Content-Type", "application/xml;text/xml");
  QSslConfiguration ssl;
  if (!ssl.addCaCertificates(applCaBundlePath(), QSsl::Pem)) {
    QFileInfo info(config->value("ssloptions/ssl_bundle").toString());
    if (info.isReadable()) {
      ssl.addCaCertificates(info.filePath().toLocal8Bit(), QSsl::Pem);
    }
  }
  req.setSslConfiguration(ssl);

  return req;
}

AbeBooksDocument AbeBooksRequester::createDocument() {
  AbeBooksAccess ac;
  ac.user = toISO88591(config->value("api_user").toString());
  ac.key = toISO88591(config->value("api_key").toString());
  if (ac.user.isEmpty() || ac.key.isEmpty())
    qWarning("Invalid Settings to Access 'ABEPO'!");

  return AbeBooksDocument(ac);
}

const QString AbeBooksRequester::toISO88591(const QString &str) {
  QTextCodec *codec = QTextCodec::codecForLocale();
  QTextEncoder encoder(codec);
  return QString(encoder.fromUnicode(str));
}
