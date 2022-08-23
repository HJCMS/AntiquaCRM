// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklookerrequester.h"
#include "booklookerconfig.h"

#include <QBuffer>
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonParseError>
#include <QTextCodec>
#include <QTextStream>
#include <QTimer>
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
  p_operation = QString();

  connect(this, SIGNAL(authenticResponse(const QJsonDocument &)), this,
          SLOT(registerAuthentic(const QJsonDocument &)));
}

const QFileInfo BooklookerRequester::operationTempFile(const QString &op) {
  QString fileName("antiqua_booklooker_" + op + ".json");
  QFileInfo fileInfo(QDir::temp(), fileName);
  return fileInfo;
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

const QUrl BooklookerRequester::apiQuery(const QString &section) {
  QUrl url(p_baseUrl);
  QString p("/");
  p.append(BOOKLOOKER_API_VERSION);
  p.append("/");
  p.append(section);
  url.setPath(p);
  return url;
}

const QNetworkRequest BooklookerRequester::newRequest(const QUrl &url) {
  QNetworkRequest req(url);
  req.setPeerVerifyName(url.host());
  req.setRawHeader("User-Agent", userAgentString());
  req.setRawHeader("Accept-Language", languageRange());
  req.setRawHeader("Accept", "text/*;application/json,q=0.1");
  req.setRawHeader("Cache-Control", "private");
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
      qWarning("Booklooker : write token in config.");
      config->beginGroup(CONFIG_GROUP);
      config->setValue("api_token", value);
      config->endGroup();
    } else {
      qInfo("New token Authentication: %s", qPrintable(value));
    }
    /**
     * !!! WARNING !!!
     * Booklooker Anfragen auf Windows machen Probleme wenn zu schnell
     * hintereinander mehrere Anfragen ausgeführt werden.
     * Aus diesem Grund hier eine Zeitverzögerung!
     */
    QTimer::singleShot(1000, this, SIGNAL(authenticFinished()));
  } else if (status == "NOK" && !value.isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qWarning("Authentication Error: %s", qPrintable(value));
#endif
    // Authentic Response Status
    if (value == "API_KEY_MISSING") {
      // Der API Key ist leer oder nicht vorhanden.
      emit errorMessage(Antiqua::ErrorStatus::NOTICE, tr("Missing API Key"));
    } else if (value == "AUTHENTICATION_FAILED") {
      // Der API Key ist nicht bekannt.
      emit errorMessage(Antiqua::ErrorStatus::FATAL,
                        tr("API Key - Authentication failed"));
    } else if (value == "SERVER_DOWN") {
      // Aufgrund von Wartungsarbeiten ist die REST API momentan nicht
      // verfügbar.
      emit errorMessage(Antiqua::ErrorStatus::FATAL, tr("Server down"));
    } else if (value == "INVALID_INTERFACE") {
      // Es wurde eine ungültige Schnittstelle verwendet.
      emit errorMessage(Antiqua::ErrorStatus::FATAL, tr("Invalid Interface"));
    } else if (value == "INVALID_REQUEST_METHOD") {
      // Die Schnittstelle wurde mit einer ungültigen HTTP-Methode aufgerufen.
      emit errorMessage(Antiqua::ErrorStatus::FATAL,
                        tr("Invalid Request Method"));
    } else if (value == "QUOTA_EXCEEDED") {
      // Die maximale Anzahl Abfragen/Minute wurde überschritten.
      emit errorMessage(Antiqua::ErrorStatus::FATAL, tr("Quota Exceeded"));
    }
  } else {
#ifdef ANTIQUA_DEVELOPEMENT
    qWarning("Booklooker::registerAuthentic: Unknown error!");
#endif
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

void BooklookerRequester::slotSslErrors(const QList<QSslError> &list) {
  for (int i = 0; i < list.count(); i++) {
    QSslError ssl_error = list.at(i);
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << ssl_error.errorString();
#endif
  }
}

void BooklookerRequester::replyFinished(QNetworkReply *reply) {
  if (reply->error() == QNetworkReply::NoError) {
    m_reply->deleteLater();
  }
}

void BooklookerRequester::replyReadyRead() {
  if (m_reply == nullptr)
    return;

  bool success = true;
  if (m_reply->error() != QNetworkReply::NoError) {
    slotError(m_reply->error());
    success = false;
  }

  bool check;
  QVector<char> buf;
  QByteArray data;
  qint64 chunk;
  qint64 bufferSize = replyHeaderLength();
  while (m_reply->bytesAvailable() > 0) {
    chunk = m_reply->bytesAvailable();
    if (chunk > bufferSize) {
      chunk = bufferSize;
    }
    buf.resize(chunk + 1);
    memset(&buf[0], 0, chunk + 1);
    if (chunk != m_reply->read(&buf[0], chunk)) {
      qWarning("Booklooker: buffer read error");
    }
    data += &buf[0];
  }
  buf.clear();

  QJsonParseError parser;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parser);
  if (parser.error != QJsonParseError::NoError) {
    qWarning("Json Parse Error:(%s)!", jsonParserError(parser.error));
    QStringList message({"Booklooker", tr("Invalid Document response!")});
    if (qEnvironmentVariable(BOOKLOOKER_ERROR_ENV).isNull()) {
      message << tr("Network request");
      emit errorMessage(Antiqua::ErrorStatus::FATAL, message.join(" "));
      qputenv(BOOKLOOKER_ERROR_ENV, "1");
    } else {
      emit errorMessage(Antiqua::ErrorStatus::NOTICE, message.join(" "));
    }
    writeErrorLog(data);
    return;
  }

  if (m_reply->url().path() == BOOKLOOKER_AUTH_PATH) {
    emit authenticResponse(doc);
    return;
  }

  writeResponseLog(doc);
  emit response(doc);
  emit requestFinished(success);
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

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(replyReadyRead()));
}

void BooklookerRequester::writeErrorLog(const QByteArray &data) {
  QString logFileName("antiqua_booklooker_errors.log");
  QFileInfo logFilePath(QDir::temp(), logFileName);
  QFile fp(logFilePath.filePath());
  if (fp.open(QIODevice::WriteOnly)) {
    QTextStream stream(&fp);
    stream.setCodec(QTextCodec::codecForName("UTF-8"));
    stream << QString::fromLocal8Bit(data);
    fp.close();
#ifdef ANTIQUA_DEVELOPEMENT
    qInfo("Booklooker:'%s'.", qPrintable(logFilePath.filePath()));
#endif
  }
}

void BooklookerRequester::writeResponseLog(const QJsonDocument &doc) {
  QFileInfo fileInfo = operationTempFile(p_operation);
  QFile fp(fileInfo.filePath());
  if (fp.open(QIODevice::WriteOnly)) {
    QTextStream json(&fp);
    json.setCodec(QTextCodec::codecForName("UTF-8"));
#ifdef ANTIQUA_DEVELOPEMENT
    json << doc.toJson(QJsonDocument::Indented);
    qInfo("Booklooker(%s):'%s'.", qPrintable(p_operation),
          qPrintable(fileInfo.filePath()));
#else
    json << doc.toJson(QJsonDocument::Compact);
#endif
    fp.close();
  }
}

qint64 BooklookerRequester::replyHeaderLength() {
  bool b = false;
  qint64 length =
      m_reply->header(QNetworkRequest::ContentLengthHeader).toLongLong(&b);
  if (b && length > 1024) {
    return length;
  }
  return 1024;
}

bool BooklookerRequester::deleteRequest(const QUrl &url) {
  QNetworkRequest req = newRequest(url);
  m_reply = deleteResource(req);

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(replyReadyRead()));

  return false;
}

bool BooklookerRequester::putRequest(const QUrl &url, const QByteArray &data) {
#ifdef ANTIQUA_DEVELOPEMENT
  QUrlQuery dispUrl(url);
  qDebug() << Q_FUNC_INFO << dispUrl.toString(QUrl::FullyEncoded);
  return true;
#endif

  QNetworkRequest req = newRequest(url);
  req.setRawHeader("Content-Type", "text/plain");
  m_reply = put(req, data);

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(replyReadyRead()));

  return true;
}

bool BooklookerRequester::getRequest(const QUrl &url) {
  QNetworkRequest req = newRequest(url);
  m_reply = get(req);

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(replyReadyRead()));

  return true;
}

void BooklookerRequester::authenticationRefresh() { authentication(); }

void BooklookerRequester::queryList() {
  p_operation = "orders_list";
  if (getToken().isEmpty()) {
    authentication();
    connect(this, SIGNAL(authenticFinished()), this, SLOT(queryList()));
    return;
  }

  QUrl url = apiQuery("order");
  QDate past = QDate::currentDate().addDays(ANTIQUA_QUERY_PASTDAYS);
  QUrlQuery q;
  q.addQueryItem("token", getToken());
  q.addQueryItem("dateFrom", past.toString(DATE_FORMAT));
  q.addQueryItem("dateTo", QDate(QDate::currentDate()).toString(DATE_FORMAT));
  url.setQuery(q);
  getRequest(url);
}

const QStringList BooklookerRequester::orderMessageTypes() {
  QStringList l({"PAYMENT_INFORMATION", "PAYMENT_REMINDER", "SHIPPING_NOTICE"});
  return l;
}

void BooklookerRequester::queryUpdateOrderStatus(const QString &orderId,
                                                 const QString &status) {
  if (orderId.length() < 4)
    return;

  if (status.isEmpty())
    return;

  if (getToken().isEmpty()) {
    authentication();
    return;
  }

  QUrlQuery q;
  q.addQueryItem("token", getToken());
  q.addQueryItem("orderId", orderId);
  q.addQueryItem("status", status);

  p_operation = "order_status";
  QUrl url = apiQuery(p_operation);
  url.setQuery(q);

  putRequest(url, QByteArray());
}

void BooklookerRequester::queryUpdateOrderCancel(const QString &orderId) {
  if (orderId.length() < 4)
    return;

  if (getToken().isEmpty()) {
    authentication();
    return;
  }

  QUrlQuery q;
  q.addQueryItem("token", getToken());
  q.addQueryItem("orderId", orderId);

  p_operation = "order_cancel";
  QUrl url = apiQuery(p_operation);
  url.setQuery(q);

  putRequest(url, QByteArray());
}

void BooklookerRequester::queryPushMessage(const QString &orderId,
                                           const QString &messageType,
                                           const QString &additionalText) {
  if (orderId.length() < 4)
    return;

  if (messageType.isEmpty())
    return;

  if (additionalText.isEmpty())
    return;

  if (getToken().isEmpty()) {
    authentication();
    return;
  }

  QStringList types = orderMessageTypes();
  if (!types.contains(messageType)) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "INVALID:" << messageType;
#endif
    emit errorMessage(Antiqua::ErrorStatus::NOTICE,
                      tr("Invalid Messagetype, operation rejected!"));
    return;
  }

  QUrlQuery q;
  q.addQueryItem("order_message", getToken());
  q.addQueryItem("orderId", orderId);
  q.addQueryItem("messageType", messageType);
  q.addQueryItem("additionalText", additionalText);

  QUrlQuery body;
  body.addQueryItem("additionalText", additionalText);
  QString message = body.toString(QUrl::FullyEncoded);

  p_operation = "order_message";
  QUrl url = apiQuery(p_operation);
  url.setQuery(q);
  putRequest(url, message.toLocal8Bit());
}

void BooklookerRequester::queryOrder(const QString &orderId) {
  QString fileName("antiqua_booklooker_orders_list.json");
  QFileInfo fileInfo(QDir::temp(), fileName);
  if (fileInfo.isReadable()) {
    QString data;
    QFile fp(fileInfo.filePath());
    if (fp.open(QIODevice::ReadOnly)) {
      QTextStream json(&fp);
      json.setCodec(QTextCodec::codecForName("UTF-8"));
      while (!json.atEnd()) {
        data.append(json.readLine());
      }
      fp.close();
    }
    QJsonParseError parser;
    QJsonDocument doc = QJsonDocument::fromJson(data.toLocal8Bit(), &parser);
    if (parser.error == QJsonParseError::NoError) {
      if (doc["status"].toString().toLower() == "ok") {
        emit response(doc);
        emit requestFinished(true);
        return;
      }
    }
  }

  if (getToken().isEmpty()) {
    authentication();
    return;
  }

  p_operation = "order";
  QUrl url = apiQuery("order");
  QUrlQuery q;
  q.addQueryItem("token", getToken());
  q.addQueryItem("orderId", orderId);
  url.setQuery(q);
  getRequest(url);
}

void BooklookerRequester::queryArticleReset(const QString &orderNo) {
  if (orderNo.length() < 2)
    return;

  if (getToken().isEmpty()) {
    authentication();
    return;
  }
  p_operation = "article_reset";
  QUrl url = apiQuery("article");
  QUrlQuery q;
  q.addQueryItem("token", getToken());
  q.addQueryItem("orderNo", orderNo);
  url.setQuery(q);
  deleteRequest(url);
}

const QString BooklookerRequester::getToken() {
  return qEnvironmentVariable(BOOKLOOKER_TOKEN_ENV);
}

const QString BooklookerRequester::getResponseErrors(const QJsonDocument &doc) {
  QString status = QJsonValue(doc["status"]).toString();
  if (status == "OK")
    return QString();

  QString value = QJsonValue(doc["returnValue"]).toString();
  if (value == "SERVER_DOWN") {
    // Server vorübergehend ausgefallen!
    return tr("Server temporarily down!");
  } else if (value == "INVALID_INTERFACE") {
    // Es wurde eine ungültige Schnittstelle verwendet.
    qWarning("Booklooker: INVALID_INTERFACE");
    return tr("An invalid interface was used.");
  } else if (value == "INVALID_REQUEST_METHOD") {
    // Ungültige HTTP-Methode aufgerufen.
    qWarning("Booklooker: INVALID_REQUEST_METHOD");
    return tr("Invalid HTTP method called.");
  } else if (value == "TOKEN_MISSING") {
    // Der Token ist leer oder nicht vorhanden.
    qWarning("Booklooker: TOKEN_MISSING");
    return tr("The token is empty or does not exist.");
  } else if (value == "TOKEN_UNKNOWN") {
    // Der Token ist dem System nicht bekannt.
    qWarning("Booklooker: TOKEN_UNKNOWN");
    return tr("The token is not known to the system.");
  } else if (value == "TOKEN_EXPIRED") {
    // Der Token ist abgelaufen.
    qWarning("Booklooker: TOKEN_EXPIRED");
    return tr("The token has expired.");
  } else if (value == "QUOTA_EXCEEDED") {
    // Maximale Abfragenanzahl wurde überschritten.
    qWarning("Booklooker: QUOTA_EXCEEDED");
    return tr("Maximum number of queries has been exceeded.");
  } else if (value == "TEMPORARILY_BLOCKED") {
    // Ihr Zugang wurde vorübergehend gesperrt.
    qWarning("Booklooker: TEMPORARILY_BLOCKED");
    return tr("Your access has been temporarily blocked.");
  } else if (value == "ENCODING_ERROR") {
    // Die Antwort enthält ungültige Zeichen.
    qWarning("Booklooker: ENCODING_ERROR");
    return tr("The response contains invalid characters.");
  }
  return QString("UNKNOWN_ERROR");
}
