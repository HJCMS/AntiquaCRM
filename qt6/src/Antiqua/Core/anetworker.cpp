// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "anetworker.h"
#include "anetworkcache.h"
#include "anetworkrequest.h"

#include <ASettings>
#include <QtCore>
#include <QtNetwork>

namespace AntiquaCRM {

ANetworker::ANetworker(AntiquaCRM::NetworkQueryType type, QObject *parent)
    : QNetworkAccessManager{parent}, queryType{type} {
  setObjectName("antiquacrm_networker");
  setCache(new AntiquaCRM::ANetworkCache(this));

  m_reply = nullptr;

  AntiquaCRM::ASettings cfg(this);
  transfer_timeout = cfg.value("transfer_timeout", 15).toInt();
}

ANetworker::~ANetworker() {
  if (m_reply != nullptr) {
    if (m_reply->isFinished())
      m_reply->close();

    m_reply->deleteLater();
  }
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

  QStringList findText("application/json");
  findText << "text/plain";
  findText << "text/html";
  findText << "text/*";
  findText << "text/";

  //  QByteArray data;
  //  data = m_reply->readAll();
  //  if (data.isNull()) {
  //    qWarning("Network: No Data responsed!");
  //    return;
  //  }

#ifdef ANTIQUA_DEVELOPMENT
  qInfo("Host: %s", qPrintable(m_reply->url().host()));
  foreach (QByteArray a, m_reply->rawHeaderList()) {
    qInfo("-- %s: %s", a.constData(), m_reply->rawHeader(a).constData());
  }
#endif

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

#ifdef ANTIQUA_DEVELOPMENT
  QByteArray byte_info;
  QTextStream(&byte_info) << "bytes=" << data.size();
  qInfo("-- Bytes responses (%s).", byte_info.data());
#endif

  // JSON Request
  if (queryType == AntiquaCRM::NetworkQueryType::JSON_QUERY) {
    QJsonParseError parser;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parser);
    if (parser.error != QJsonParseError::NoError) {
      qWarning("Network: Responsed json is not well format:(%s)!",
               qPrintable(parser.errorString()));
      emit sendFinishedWithErrors();
      return;
    }
    data.clear();
    emit sendJsonResponse(doc);
    return;
  }

  // XML/SOAP Request
  if (queryType == AntiquaCRM::NetworkQueryType::XML_QUERY) {
    QDomDocument xml("response");
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

  qWarning("Network: Unknown response type!");
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

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadResponse()));

  return m_reply;
}

QNetworkReply *ANetworker::jsonPostRequest(const QUrl &url,
                                           const QJsonDocument &body,
                                           const QByteArray &charset) {
  ANetworkRequest request(url);
  request.setHeaderUserAgent();
  request.setHeaderAcceptLanguage(charset);
  request.setHeaderAcceptText();
  request.setHeaderCacheControl();
  request.setHeaderContentTypeJson(charset);
  request.setTransferTimeout((transfer_timeout * 1000));

  QByteArray data = body.toJson(QJsonDocument::Compact);
  request.setHeaderContentLength(data.size());

  m_reply = post(request, data);

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadResponse()));

  return m_reply;
}

QNetworkReply *ANetworker::xmlPostRequest(const QUrl &url,
                                          const QDomDocument &body,
                                          const QByteArray &charset) {
  ANetworkRequest _req(url);
  _req.setHeaderUserAgent();
  _req.setHeaderAcceptLanguage(charset);
  _req.setHeaderAcceptText();
  _req.setHeaderCacheControl();
  _req.setHeaderContentTypeXml(charset);
  _req.setTransferTimeout((transfer_timeout * 1000));

  QByteArray data = body.toByteArray(-1);
  _req.setHeaderContentLength(data.size());

  m_reply = post(_req, data);

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
  ANetworkRequest _req(url);
  _req.setHeaderUserAgent();
  _req.setHeaderAcceptLanguage();
  _req.setHeaderAcceptText();
  _req.setHeaderCacheControl();
  _req.setTransferTimeout((transfer_timeout * 1000));

  QByteArray data = body.toJson(QJsonDocument::Compact);
  QHttpPart json_part;
  QString _t("application/json; charset=" + ANTIQUACRM_CHARSET);
  json_part.setRawHeader("ContentTypeHeader", _t.toLocal8Bit());
  QString _n("form-data; name=\"" + name + "\"");
  json_part.setRawHeader("ContentDispositionHeader", _n.toLocal8Bit());
  json_part.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
  json_part.setBody(data);

  QHttpMultiPart *m_form = new QHttpMultiPart(QHttpMultiPart::FormDataType);
  m_form->append(json_part);

  m_reply = post(_req, m_form);
  m_form->setParent(m_reply);

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
  connect(m_reply, SIGNAL(readyRead()), SLOT(slotReadResponse()));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)),
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)),
          SLOT(slotSslErrors(QList<QSslError>)));

  return m_reply;
}

}; // namespace AntiquaCRM
