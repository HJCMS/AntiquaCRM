// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklookerrequester.h"
#include "booklookerconfig.h"

#include <QDate>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonParseError>
#include <QMutex>
#include <QUrlQuery>

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

static const char *jsonParserError(int err) {
  switch (err) {
  case (QJsonParseError::UnterminatedObject):
    return "Object is not correctly terminated";

  case (QJsonParseError::MissingNameSeparator):
    return "A comma separating different items is missing";

  case (QJsonParseError::UnterminatedArray):
    return "Array is not correctly terminated";

  case (QJsonParseError::MissingValueSeparator):
    return "A colon separating key from values is missing";

  case (QJsonParseError::IllegalValue):
    return "The value is illegal";

  case (QJsonParseError::TerminationByNumber):
    return "The input stream ended while parsing a number";

  case (QJsonParseError::IllegalNumber):
    return "The number is not well formed";

  case (QJsonParseError::IllegalEscapeSequence):
    return "An illegal escape sequence occurred";

  case (QJsonParseError::IllegalUTF8String):
    return "An illegal UTF8 sequence occurred";

  case (QJsonParseError::UnterminatedString):
    return "String wasn't terminated with a quote";

  case (QJsonParseError::MissingObject):
    return "Object was expected but couldn't be found";

  case (QJsonParseError::DeepNesting):
    return "Document is too deeply nested";

  case (QJsonParseError::DocumentTooLarge):
    return "Document is too large";

  case (QJsonParseError::GarbageAtEnd):
    return "Document contains additional garbage characters";

  default:
    return "Unknown";
  };
}

BooklookerRequester::BooklookerRequester(QObject *parent)
    : QNetworkAccessManager{parent} {
  config = new ApplSettings(this);
  initConfigurations();
  m_reply = nullptr;

  connect(this, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(slotFinished(QNetworkReply *)));
}

void BooklookerRequester::initConfigurations() {
  QUrl url;
  config->beginGroup(CONFIG_GROUP);
  url.setScheme(config->value("api_scheme", "https").toString());
  url.setHost(config->value("api_host", "api.booklooker.de").toString());
  p_apiKey = config->value("api_key", "fuck").toString();
  config->endGroup();
  p_baseUrl = url;
}

const QNetworkRequest BooklookerRequester::newRequest(const QUrl &url) {
  QNetworkRequest req(url);
  req.setPeerVerifyName(url.host());
  req.setRawHeader("User-Agent", userAgentString());
  req.setRawHeader("Accept-Language", languageRange());
  req.setRawHeader("Accept",
                   "application/ld+json,application/json,text/*;q=0.1");
  req.setRawHeader("Cache-Control", "no-cache, private");
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

void BooklookerRequester::registerAuthentic(const QJsonDocument &doc) {
  QString status = QJsonValue(doc["status"]).toString();
  QString value = QJsonValue(doc["returnValue"]).toString();
  if (status == "OK" && !value.isEmpty()) {
    if (!qputenv(BOOKLOOKER_TOKEN_ENV, value.toLocal8Bit())) {
      qInfo("Booklooker : write token in config.");
      config->beginGroup(CONFIG_GROUP);
      config->setValue("api_token", value);
      config->endGroup();
    }
    qInfo("Authenticated: %s", qPrintable(value));
    emit authenticFinished();
  } else if (status == "NOK" && !value.isEmpty()) {
    qWarning("Authentication Error: %s", qPrintable(value));
    // Authentic Response Status
    if (value == "API_KEY_MISSING") {
      emit errorMessage(1, tr("Missing API Key"));
    } else if (value == "AUTHENTICATION_FAILED") {
      emit errorMessage(1, tr("Authentication failure"));
    } else if (value == "SERVER_DOWN") {
      emit errorMessage(1, tr("Server down"));
    }
  } else {
    qWarning("registerAuthentic: Unknown error!");
  }
}

void BooklookerRequester::slotError(QNetworkReply::NetworkError error) {
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

  case QNetworkReply::RemoteHostClosedError:
    qWarning("Networker: RemoteHost Closed Error");
    return;

  case QNetworkReply::OperationCanceledError:
    qWarning("Networker: Operation Canceled Error");
    return;

  case QNetworkReply::InsecureRedirectError:
    qWarning("Networker: Insecure Redirect Error");
    return;

  default:
    qWarning("Networker: Unknown Error:%s", qPrintable(QString::number(error)));
    return;
  }
}

void BooklookerRequester::slotFinished(QNetworkReply *reply) {
  bool success = true;
  if (reply->error() != QNetworkReply::NoError) {
    slotError(reply->error());
    success = false;
  }
  emit requestFinished(success);
}

void BooklookerRequester::slotSslErrors(const QList<QSslError> &list) {
  for (int i = 0; i < list.count(); i++) {
    QSslError ssl_error = list.at(i);
    qDebug() << Q_FUNC_INFO << ssl_error.errorString();
  }
}

void BooklookerRequester::replyFinished(QNetworkReply *reply) {
  if (reply->error() == QNetworkReply::NoError) {
    m_reply->deleteLater();
  }
}

void BooklookerRequester::replyReadyRead() {
  QMutex mutex(QMutex::NonRecursive);
  mutex.tryLock((1000 * 3));
  QVector<char> buf;
  QByteArray data;
  qint64 chunk;
  while (m_reply->bytesAvailable() > 0) {
    chunk = m_reply->bytesAvailable();
    if (chunk > 4096) {
      chunk = 4096;
    }
    buf.resize(chunk + 1);
    memset(&buf[0], 0, chunk + 1);
    if (chunk != m_reply->read(&buf[0], chunk)) {
      qWarning("BooklookerRequester: buffer read error");
    }
    data += &buf[0];
  }
  buf.clear();
  mutex.unlock();

  QJsonParseError parser;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parser);
  if (parser.error != QJsonParseError::NoError) {
    qWarning("Json Parse Error:(%s)!", jsonParserError(parser.error));
#ifdef PLUGIN_BOOKLOOKER_DEBUG
    QFileInfo logfile(QDir(QDir::currentPath()), "error-file.json");
    QFile fp(logfile.filePath());
    if (fp.open(QIODevice::WriteOnly)) {
      QTextStream in(&fp);
      in << data;
      fp.close();
    }
#endif
    return;
  }

  if (m_reply->url().path() == BOOKLOOKER_AUTH_PATH) {
    emit authenticResponse(doc);
    return;
  }

#ifdef PLUGIN_BOOKLOOKER_DEBUG
  QFileInfo logfile(QDir(QDir::currentPath()), "booklooker.json");
  QFile fp(logfile.filePath());
  if (fp.open(QIODevice::WriteOnly)) {
    QTextStream in(&fp);
    in << doc.toJson(QJsonDocument::Indented);
    fp.close();
  }
#endif

  emit response(doc);
}

void BooklookerRequester::authentication() {
  QUrl url(p_baseUrl);
  url.setPath(BOOKLOOKER_AUTH_PATH);

  QString pd("apiKey=");
  pd.append(p_apiKey);

  QNetworkRequest req = newRequest(url);
  req.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
  m_reply = post(req, pd.toLocal8Bit());

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(replyReadyRead()));

  connect(this, SIGNAL(authenticResponse(const QJsonDocument &)), this,
          SLOT(registerAuthentic(const QJsonDocument &)));
}

bool BooklookerRequester::getRequest(const QUrl &url) {
  QNetworkRequest req = newRequest(url);
  m_reply = get(req);

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(replyReadyRead()));

  return true;
}

void BooklookerRequester::queryList() {
  if (getToken().isEmpty()) {
    authentication();
    // connect(m_reply, SIGNAL(authenticFinished()), this, SLOT(queryList()));
    return;
  }

  QUrl url(p_baseUrl);
  QString p("/");
  p.append(BOOKLOOKER_API_VERSION);
  p.append("/order");
  url.setPath(p);

  QDate past = QDate::currentDate().addDays(-2);
  QUrlQuery q;
  q.addQueryItem("token", getToken());
  q.addQueryItem("dateFrom", past.toString(DATE_FORMAT));
  q.addQueryItem("dateTo", QDate(QDate::currentDate()).toString(DATE_FORMAT));
  url.setQuery(q);
  // qDebug() << Q_FUNC_INFO << url;
  getRequest(url);
}

void BooklookerRequester::queryOrder(const QString &orderId) {
  if (getToken().isEmpty()) {
    authentication();
    return;
  }

  QUrl url(p_baseUrl);
  QString p("/");
  p.append(BOOKLOOKER_API_VERSION);
  p.append("/order");
  url.setPath(p);

  QUrlQuery q;
  q.addQueryItem("token", getToken());
  q.addQueryItem("orderId", orderId);
  url.setQuery(q);
  // qDebug() << Q_FUNC_INFO << url;
  getRequest(url);
}

void BooklookerRequester::authenticationRefresh() { authentication(); }

const QString BooklookerRequester::getToken() {
  QString token = qEnvironmentVariable(BOOKLOOKER_TOKEN_ENV);
  if (token.isEmpty()) {
    qInfo("Booklooker: no token in Environment");
  }
  return token;
}
