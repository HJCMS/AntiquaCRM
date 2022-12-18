// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "networker.h"
#include "networkrequest.h"

Networker::Networker(QObject *parent) : QNetworkAccessManager{parent} {
  m_textCodec = QTextCodec::codecForLocale();
  connect(this, SIGNAL(finished(QNetworkReply *)),
          SLOT(slotFinished(QNetworkReply *)));
}

void Networker::slotFinished(QNetworkReply *reply) {
  if (reply->error() != QNetworkReply::NoError) {
    qWarning("ANTIQUACRM::Network:%s", qPrintable(reply->url().host()));
    slotError(reply->error());
  }
}

void Networker::slotError(QNetworkReply::NetworkError error) {
  switch (error) {
  case QNetworkReply::ConnectionRefusedError:
    qWarning("ANTIQUACRM::Network:Connection Refused Error");
    return;

  case QNetworkReply::TimeoutError:
    qWarning("ANTIQUACRM::Network:Timeout Error");
    return;

  case QNetworkReply::HostNotFoundError:
    qWarning("ANTIQUACRM::Network:Host NotFound Error");
    return;

  case QNetworkReply::RemoteHostClosedError:
    qWarning("ANTIQUACRM::Network:RemoteHost Closed Error");
    return;

  case QNetworkReply::OperationCanceledError:
    qWarning("ANTIQUACRM::Network:Operation Canceled Error");
    return;

  case QNetworkReply::InsecureRedirectError:
    qWarning("ANTIQUACRM::Network:Insecure Redirect Error");
    return;

  case QNetworkReply::InternalServerError:
    qWarning("ANTIQUACRM::Network:Internal Server Error");
    return;

  default:
    qWarning("ANTIQUACRM::Network:Unknown Error (%s)",
             qPrintable(QString::number(error)));
    return;
  }
}

void Networker::slotSslErrors(const QList<QSslError> &list) {
  for (int i = 0; i < list.count(); i++) {
    QSslError ssl_error = list.at(i);
    QString ssl_error_str = ssl_error.errorString();
    qWarning("ANTIQUACRM::Network:SSL-Errors:(%s)!", qPrintable(ssl_error_str));
  }
}

void Networker::slotReadResponse() {
  if (m_reply == nullptr)
    return;

  if (m_reply->error() != QNetworkReply::NoError) {
    slotError(m_reply->error());
  }

  if (!m_reply->bytesAvailable()) {
    qWarning("ANTIQUACRM::Network:No Data responsed!");
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
        } else if (findText.contains(section) && !textContent) {
          textContent = true;
        }
      }
    }
  }

  QByteArray data;
  data = m_reply->readAll();
  if (data.isNull()) {
    qWarning("ANTIQUACRM::Network:No Data responsed!");
    return;
  }

  emit sendResponse(data);
}

QNetworkReply *Networker::loginRequest(const NetworkRequest &request,
                                       const QByteArray &data) {
  NetworkRequest req(request);
  req.setRawHeader("Accept", "text/*");
  req.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
  m_reply = post(req, data);

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadResponse()));

  return m_reply;
}

QNetworkReply *Networker::postRequest(const NetworkRequest &request,
                                      const QByteArray &data) {
  NetworkRequest req(request);
  req.setHeaderContentLength(data.size());

  m_reply = post(req, data);

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadResponse()));

  return m_reply;
}

QNetworkReply *Networker::customRequest(const NetworkRequest &request,
                                      const QByteArray &data) {
  NetworkRequest req(request);
  req.setHeaderContentLength(data.size());

  m_reply = post(req, data);

  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(errorOccurred(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  return m_reply;
}

QNetworkReply *Networker::putRequest(const NetworkRequest &request,
                                     const QByteArray &data) {
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

QNetworkReply *Networker::getRequest(const NetworkRequest &request) {
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
