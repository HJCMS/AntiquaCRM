// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "isbnrequest.h"

#include <QtCore/QDebug>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QJsonValueRef>
#include <QtCore/QVector>
#include <QtNetwork/QNetworkRequest>

/**
 @brief stringsList()
 Alle Stringvariablen
*/
static const QStringList stringsList() {
  QStringList l;
  l << "url";             /**< Externe Referenz URL */
  l << "title";           /**< Buchtitel */
  l << "number_of_pages"; /**< Seitenanzahl */
  l << "publish_date";    /**< Jahr */
  l << "medium";          /**< Image Url medium */
  l << "large";           /**< Image Url large */
  return l;
}

/**
   @brief arrayList()
   Alle Arrayvariablen
*/
static const QStringList arrayList() {
  QStringList l;
  l << "publishers";     /**< Verlag  */
  l << "publish_places"; /**< Verlag Herkunft  */
  l << "authors";        /**< Autoren  */
  return l;
}

IsbnRequest::IsbnRequest(const QString &isbn, QObject *parent)
    : QObject{parent}, p_isbnKey("ISBN:" + isbn) {
  setObjectName("IsbnRequest");

  QString req("http://openlibrary.org/api/books?bibkeys=ISBN:");
  req.append(isbn);
  req.append("&jscmd=data&format=json");
  p_url = QUrl(req);
}

bool IsbnRequest::setManager() {
  m_manager = new QNetworkAccessManager(this);
  m_manager->setObjectName("NetworkAccessManager");
  connect(m_manager, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(replyFinished(QNetworkReply *)));

  return (m_manager != nullptr);
}

void IsbnRequest::read(const QJsonObject &obj) {
  QJsonObject sobj(obj);
  QJsonObject::iterator it;
  for (it = sobj.begin(); it != sobj.end(); ++it) {
    if (it.value().isObject()) {
      QJsonObject child = it.value().toObject();
      foreach (const QString &key, child.keys()) {
        QJsonValue v = child[key];
        if (stringsList().contains(key)) {
          if (v.toString().isEmpty())
            continue;

          if (key == "number_of_pages") {
            p_data.insert(key, v.toInt());
          } else if (key == "publish_date") {
            p_data.insert(key, v.toString());
          } else if (key == "medium") {
            p_data.insert("medium_image", v.toString());
          } else if (key == "large") {
            p_data.insert("large_image", v.toString());
          } else {
            p_data.insert(key, v.toString());
          }
        } else if (arrayList().contains(key)) {
          // Array
          QJsonArray ar = v.toArray();
          if (ar.size() < 1)
            continue;

          for (int i = 0; i < ar.size(); i++) {
            if (ar[i].toObject().contains("name")) {
              QString value = ar[i].toObject().take("name").toString();
              p_data.insert(key, value);
            }
          }
        }
      } // end foreach
      read(child);
    }
  }
}

void IsbnRequest::importResponse(const QByteArray &b) {
  QJsonDocument json = QJsonDocument::fromJson(b);
  if (json.isNull()) {
    qDebug("IsbnRequest::importResponse = failed");
    return;
  }
  p_respJsonObject = json.object();
  if (p_respJsonObject.count() >= 1)
    read(p_respJsonObject);
}

void IsbnRequest::replyFinished(QNetworkReply *reply) {
  qDebug() << "IsbnRequest::replyFinished" << reply->url();
  if (reply->error() == QNetworkReply::NoError) {
    m_reply->deleteLater();
  }

  emit requestFinished((p_data.size() > 1));
}

void IsbnRequest::slotError(QNetworkReply::NetworkError err) {
  switch (err) {
  case QNetworkReply::ConnectionRefusedError:
    qDebug() << "Access ConnectionRefusedError";
    return;

  case QNetworkReply::TimeoutError:
    qDebug() << "Access TimeoutError";
    return;

  case QNetworkReply::HostNotFoundError:
    qDebug() << "Access HostNotFoundError";
    return;

  default:
    qDebug() << "Unknown Error:" << err;
    return;
  }
}

void IsbnRequest::slotSslErrors(QList<QSslError>) {
  qDebug() << "slotSslErrors"
           << "TODO";
}

void IsbnRequest::slotReadyRead() {
  QVector<char> buf;
  QByteArray response;
  qint64 chunk;
  while (m_reply->bytesAvailable() > 0) {
    chunk = m_reply->bytesAvailable();
    if (chunk > 4096) {
      chunk = 4096;
    }
    buf.resize(chunk + 1);
    memset(&buf[0], 0, chunk + 1);
    if (chunk != m_reply->read(&buf[0], chunk)) {
      qDebug("IsbnRequest -> read error");
    } else {
      qDebug("IsbnRequest -> read ok");
    }
    response += &buf[0];
  }
  buf.clear();
  importResponse(response);
}

/*
curl -v \
'http://openlibrary.org/api/books?bibkeys=ISBN:3921524679&jscmd=data&format=json'
*/
void IsbnRequest::triggerRequest() {
  QNetworkRequest request(p_url);
  if (setManager()) {
    qDebug() << "IsbnRequest::startRequest" << request.url();
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
            SLOT(slotError(QNetworkReply::NetworkError)));

    connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
            SLOT(slotSslErrors(QList<QSslError>)));

    connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
  }
}

const QMap<QString, QVariant> IsbnRequest::getResponse() {
  if (p_data.isEmpty())
    p_data.clear();

  return p_data;
}
