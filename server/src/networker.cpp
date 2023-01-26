// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "networker.h"
#include "networkrequest.h"

#include <QDir>
#include <QMutexLocker>

QMutex NetworkCache::s_mutex;
QNetworkDiskCache *NetworkCache::s_cache = nullptr;

NetworkCache::NetworkCache(QObject *parent) : QAbstractNetworkCache{parent} {
  QMutexLocker l(&s_mutex);
  if (!s_cache) {
    s_cache = new QNetworkDiskCache;
    QString cachedir = QDir::tempPath() + "/antiquacrm";
    s_cache->setCacheDirectory(cachedir);
  }
}

qint64 NetworkCache::cacheSize() const {
  QMutexLocker l(&s_mutex);
  return s_cache->cacheSize();
}

QIODevice *NetworkCache::data(const QUrl &url) {
  QMutexLocker l(&s_mutex);
  return s_cache->data(url);
}

void NetworkCache::insert(QIODevice *device) {
  QMutexLocker l(&s_mutex);
  s_cache->insert(device);
}

QNetworkCacheMetaData NetworkCache::metaData(const QUrl &url) {
  QMutexLocker l(&s_mutex);
  return s_cache->metaData(url);
}

QIODevice *NetworkCache::prepare(const QNetworkCacheMetaData &data) {
  QMutexLocker l(&s_mutex);
  return s_cache->prepare(data);
}

bool NetworkCache::remove(const QUrl &url) {
  QMutexLocker l(&s_mutex);
  return s_cache->remove(url);
}

void NetworkCache::updateMetaData(const QNetworkCacheMetaData &data) {
  QMutexLocker l(&s_mutex);
  s_cache->updateMetaData(data);
}

void NetworkCache::clear() {
  QMutexLocker l(&s_mutex);
  s_cache->clear();
}

// Networker
Networker::Networker(QObject *parent) : QNetworkAccessManager{parent} {
  setCache(new NetworkCache(this));
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
  QNetworkReply* reply = reinterpret_cast<QNetworkReply*>(sender());
  if (reply == nullptr)
    return;

  if (reply->error() != QNetworkReply::NoError) {
    slotError(reply->error());
  }

  if (!reply->bytesAvailable()) {
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
  if (reply->hasRawHeader("Content-Type")) {
    QString cth(reply->rawHeader("Content-Type"));
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
  data = reply->readAll();
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
