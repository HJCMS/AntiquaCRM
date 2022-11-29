// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "anetworker.h"
#include "anetworkrequest.h"

#ifdef ANTIQUA_DEVELOPEMENT
#ifndef ANTIQUACRM_NETWORK_DEBUG
#define ANTIQUACRM_NETWORK_DEBUG false
#endif
#endif

#include <QBuffer>
#include <QHttpMultiPart>
#include <QHttpPart>
#include <QJsonParseError>
#include <QTextCodec>
#include <QTextStream>

namespace AntiquaCRM {

ANetworker::ANetworker(AntiquaCRM::PluginQueryType type, QObject *parent)
    : QNetworkAccessManager{parent}, queryType{type} {
  setObjectName("antiquacrm_networker");
  m_textCodec = QTextCodec::codecForLocale();
  connect(this, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(slotFinished(QNetworkReply *)));
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

void ANetworker::slotFinished(QNetworkReply *reply) {
  if (reply->error() != QNetworkReply::NoError) {
    slotError(reply->error());
    emit sendFinishedWithErrors();
  }
}

void ANetworker::slotReadResponse() {
  if (m_reply == nullptr)
    return;

  if (m_reply->error() != QNetworkReply::NoError) {
    slotError(m_reply->error());
  }

  if (!m_reply->bytesAvailable()) {
    qWarning("Network: No Data responsed!");
    return;
  }

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

  QByteArray data;
  data = m_reply->readAll();
  if (data.isNull()) {
    qWarning("Network: No Data responsed!");
    return;
  }

#if (ANTIQUACRM_NETWORK_DEBUG == true)
  qInfo("Host: %s", qPrintable(m_reply->url().host()));
  foreach (QByteArray a, m_reply->rawHeaderList()) {
    qInfo("-- %s: %s", a.constData(), m_reply->rawHeader(a).constData());
  }
#endif

  // JSON Request
  if (queryType == AntiquaCRM::JSON_QUERY) {
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
  if (queryType == AntiquaCRM::XML_QUERY) {
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
  request.setTransferTimeout((tranfer_timeout * 1000));
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
  request.setTransferTimeout((tranfer_timeout * 1000));

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
                                          const QDomDocument &body) {
  ANetworkRequest request(url);
  request.setHeaderUserAgent();
  request.setHeaderAcceptLanguage();
  request.setHeaderAcceptText();
  request.setHeaderCacheControl();
  request.setHeaderContentTypeXml();
  request.setTransferTimeout((tranfer_timeout * 1000));

  QByteArray data = body.toByteArray(-1);
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

QNetworkReply *ANetworker::jsonMultiPartRequest(const QUrl &url,
                                                const QString &name,
                                                const QJsonDocument &body) {
  ANetworkRequest request(url);
  request.setHeaderUserAgent();
  request.setHeaderAcceptLanguage();
  request.setHeaderAcceptText();
  request.setHeaderCacheControl();
  request.setTransferTimeout((tranfer_timeout * 1000));

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
  request.setTransferTimeout((tranfer_timeout * 1000));

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
  request.setHeaderAcceptText();
  request.setTransferTimeout((tranfer_timeout * 1000));
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
  if (m_reply != nullptr)
    m_reply->deleteLater();
}

}; // namespace AntiquaCRM
