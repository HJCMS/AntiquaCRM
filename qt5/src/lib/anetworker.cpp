// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "anetworker.h"
#include "anetworkcache.h"
#include "anetworkrequest.h"

#ifdef ANTIQUA_DEVELOPEMENT
#define ANTIQUACRM_NETWORK_DEBUG true
#endif

#include <ASettings>
#include <QBuffer>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonParseError>
#include <QTextCodec>
#include <QTextStream>

namespace AntiquaCRM {

ANetworker::ANetworker(AntiquaCRM::NetworkQueryType type, QObject *parent)
    : QNetworkAccessManager{parent}, queryType{type} {
  setObjectName("antiquacrm_networker");
  setCache(new AntiquaCRM::ANetworkCache(this));

  m_reply = nullptr;
  m_textCodec = QTextCodec::codecForLocale();

  AntiquaCRM::ASettings cfg(this);
  transfer_timeout = cfg.value("transfer_timeout", 15).toInt();
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

void ANetworker::slotReadResponse() {
  if (m_reply == nullptr)
    return;

  // Don't re-emit this signal for redirect replies.
  if (m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute)
          .isValid()) {
    qWarning("Redirected not supported in this section!");
    return;
  }

  if (m_reply->error() != QNetworkReply::NoError) {
    slotError(m_reply->error());
  }

  if (!m_reply->bytesAvailable()) {
    qWarning("Network: No Data responsed!");
    return;
  }

  const QUrl _url = m_reply->url();
  bool textContent = false;
  QStringList findText("application/json");
  findText << "text/plain";
  findText << "text/html";
  findText << "text/*";
  findText << "text/";

  QByteArray decodeWith(m_textCodec->name());
  // Content-Type
  if (m_reply->hasRawHeader("Content-Type")) {
    QString cth(m_reply->rawHeader("Content-Type"));
    QRegExp stripParam("^Content\\-Type\\b\\:\\s+", Qt::CaseInsensitive);
    cth.replace(stripParam, "");
    if (cth.contains("charset")) {
      // Leerzeichen entfernen
      cth.replace(QRegExp(";\\s*"), " ");
      cth.replace(QRegExp("\\s*=\\s*"), "=");
      foreach (QString section, cth.trimmed().split(" ")) {
        if (section.contains("charset=")) {
          section.replace("charset=", "");
          QString charset = section.trimmed().toUpper();
          QTextCodec *c = QTextCodec::codecForName(charset.toLocal8Bit());
          decodeWith = c->name();
          emit sendContentCodec(c);
        } else if (findText.contains(section) && !textContent) {
          textContent = true;
        }
      }
    }
  }

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
      qWarning("ANetworker: buffer read error");
    }
    data += &buf[0];
  }
  buf.clear();

#ifdef ANTIQUACRM_NETWORK_DEBUG
  qInfo("Request to:\"%s\" {", qPrintable(_url.host()));
  qInfo("-- Url: %s", qPrintable(m_reply->url().toString()));
  foreach (QByteArray a, m_reply->rawHeaderList()) {
    qInfo("-- %s: %s", a.constData(), m_reply->rawHeader(a).constData());
  }
  qInfo("}");
#else
  qInfo("Host: %s, response with %d bytes.", qPrintable(_url.host()),
        data.size());
#endif

  // JSON Request
  if (queryType == AntiquaCRM::JSON_QUERY) {
    QJsonParseError parser;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parser);
    if (parser.error != QJsonParseError::NoError) {
      qWarning("%s: Responsed json is not well format:(%s)!",
               qPrintable(_url.host()), qPrintable(parser.errorString()));
      emit sendFinishedWithErrors();
      return;
    }
    data.clear();
    emit sendJsonResponse(doc);
    return;
  }

  /**
   * @short XML/SOAP Request
   * @warning AbeBooks using IS0-8859-1 with invalid Content-Header
   */
  if (queryType == AntiquaCRM::XML_QUERY) {
    QDomDocument xml("response");
    if (decodeWith != m_textCodec->name()) {
      QTextCodec *codec = QTextCodec::codecForName(decodeWith);
      QString _str = codec->toUnicode(data);
      _str.replace(decodeWith, m_textCodec->name());
      data = _str.toLocal8Bit();
    }
    QString errorMsg = QString();
    int errorLine = 0;
    int errorColumn = 0;
    if (!xml.setContent(data, false, &errorMsg, &errorLine, &errorColumn)) {
      qWarning("Network: Responsed XML is not well format!");
      emit sendFinishedWithErrors();
      return;
    }
    data.clear();
    emit sendXmlResponse(xml);
    return;
  }

  qWarning("Network: Unknown response from %s!", qPrintable(_url.host()));
}

void ANetworker::slotSslErrors(const QList<QSslError> &list) {
  for (int i = 0; i < list.count(); i++) {
    QSslError ssl_error = list.at(i);
    QString ssl_error_str = ssl_error.errorString();
    qWarning("SSL-Errors:(%s)!", qPrintable(ssl_error_str));
  }
}

QNetworkReply *ANetworker::loginRequest(const QUrl &url,
                                        const QByteArray &data) {
  ANetworkRequest request(url);
  request.setHeaderUserAgent();
  request.setHeaderCacheControl();
  request.setRawHeader("Accept", "text/*");
  request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
  request.setTransferTimeout((transfer_timeout * 1000));
  m_reply = post(request, data);

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadResponse()));

  return m_reply;
}

QNetworkReply *ANetworker::jsonPostRequest(const QUrl &url,
                                           const QJsonDocument &body) {
  ANetworkRequest request(url);
  request.setHeaderUserAgent();
  request.setHeaderAcceptLanguage();
  request.setHeaderAcceptText();
  request.setHeaderCacheControl();
  request.setHeaderContentTypeJson();
  request.setTransferTimeout((transfer_timeout * 1000));

  QByteArray data = body.toJson(QJsonDocument::Compact);
  request.setHeaderContentLength(data.size());

  m_reply = post(request, data);

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadResponse()));

  return m_reply;
}

QNetworkReply *ANetworker::xmlPostRequest(const QUrl &url,
                                          const QByteArray &charset,
                                          const QDomDocument &body) {
  ANetworkRequest request(url);
  request.setHeaderUserAgent();
  request.setHeaderAcceptLanguage();
  request.setHeaderAcceptText();
  request.setHeaderCacheControl();
  request.setTransferTimeout((transfer_timeout * 1000));
  if (charset.isNull()) // use defaults
    request.setHeaderContentTypeXml(request.antiquaCharset());
  else
    request.setHeaderContentTypeXml(charset);

  QByteArray data = body.toByteArray(-1);
  request.setHeaderContentLength(data.size());

#ifdef ANTIQUACRM_NETWORK_DEBUG
  qDebug() << "Request::ContentTypeHeader="
           << request.header(QNetworkRequest::ContentTypeHeader);
#endif

  m_reply = post(request, data);

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadResponse()));

  return m_reply;
}

QNetworkReply *ANetworker::jsonMultiPartRequest(const QUrl &url,
                                                const QString &name,
                                                const QJsonDocument &body) {
  ANetworkRequest request(url);
  request.setHeaderUserAgent();
  request.setHeaderAcceptLanguage();
  request.setHeaderAcceptText();
  request.setHeaderCacheControl();
  request.setTransferTimeout((transfer_timeout * 1000));

  QByteArray data = body.toJson(QJsonDocument::Compact);
  QHttpPart json_part;
  QString _t("application/json; charset=" + ANetworkRequest::antiquaCharset());
  json_part.setRawHeader("ContentTypeHeader", _t.toLocal8Bit());
  QString _n("form-data; name=\"" + name + "\"");
  json_part.setRawHeader("ContentDispositionHeader", _n.toLocal8Bit());
  json_part.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
  json_part.setBody(data);

  QHttpMultiPart *m_form = new QHttpMultiPart(QHttpMultiPart::FormDataType);
  m_form->append(json_part);

  m_reply = post(request, m_form);
  m_form->setParent(m_reply);

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadResponse()));

  return m_reply;
}

QNetworkReply *ANetworker::putRequest(const QUrl &url, const QByteArray &data) {
  ANetworkRequest request(url);
  request.setHeaderUserAgent();
  request.setHeaderAcceptText();
  request.setHeaderAcceptLanguage();
  request.setHeaderCacheControl();
  request.setRawHeader("Content-Type", "text/plain");
  request.setTransferTimeout((transfer_timeout * 1000));

  m_reply = put(request, data);

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadResponse()));

  return m_reply;
}

QNetworkReply *ANetworker::getRequest(const QUrl &url) {
  ANetworkRequest request(url);
  request.setHeaderUserAgent();
  request.setHeaderAcceptLanguage();
  if (queryType == AntiquaCRM::NetworkQueryType::JSON_QUERY) {
    request.setHeaderAcceptJson();
  } else {
    request.setHeaderAcceptText();
  }
  request.setTransferTimeout((transfer_timeout * 1000));
  request.setHeaderCacheControl();

  m_reply = get(request);
  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadResponse()));

  return m_reply;
}

ANetworker::~ANetworker() {
  if (m_reply != nullptr) {
    m_reply->close();
    m_reply->deleteLater();
  }
}

}; // namespace AntiquaCRM
