// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "isbnrequest.h"
#include "networker.h"

#include <QByteArray>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValueRef>
#include <QNetworkRequest>
#include <QString>
#include <QVector>

#ifndef ISBN_DEBUG_OUTPUT
#define ISBN_DEBUG_OUTPUT false
#endif

IsbnData::IsbnData(const QString &isbn) : p_isbn(isbn) {}

void IsbnData::addArray(const QString &key, const QJsonArray &array) {
  if (array.size() < 1)
    return;

  for (int i = 0; i < array.size(); i++) {
    if (array[i].type() == QJsonValue::Object) {
      if (array[i].toObject().contains("name")) {
        addValue(key, array[i].toObject().take("name"));
      } else {
        addValue(key, array[i].toObject());
      }
    }
  }
}

bool IsbnData::compareAuthors(const QString &a) {
  if (p_authors.isEmpty())
    return true;

  if (p_authors.contains(a))
    return false;

  foreach (const QString n, p_authors) {
    foreach (const QString s, a.split(" ")) {
      if (n.contains(s))
        return false;
    }
  }

  return true;
}

void IsbnData::addValue(const QString &key, const QJsonValue &data) {
  if (key.isEmpty())
    return;

  // No Lineend Dots
  QRegExp nld("\\.$");
  if (data.type() == QJsonValue::Double) {
    if (key == "number_of_pages") {
      // a future year will ignored in BookEditor :)
      double pages = data.toDouble(2999);
      p_pages = QString::number(pages);
    }
  } else if (data.type() == QJsonValue::Object) {
    if ((key == "cover") && (data.toObject().keys().size() > 0)) {
      p_images = true; // Copyright
    }
  } else if (data.type() == QJsonValue::Array) {
    if (key == "authors") {
      addArray(key, data.toArray());
    } else if (key == "publishers") {
      addArray(key, data.toArray());
    }
  } else {
    if (key == "title") {
      p_title = data.toString().replace(nld, "");
    } else if (key == "subtitle") {
      p_subtitle = data.toString().replace(nld, "");
    } else if (key == "url") {
      p_url = QUrl(data.toString());
    } else if (key == "publish_date") {
      QRegExp reg("(\\d{4})");
      QStringList list = data.toString().split(" ");
      if (list.size() > 0) {
        foreach (QString l, list) {
          QString buf = l.trimmed();
          if (buf.contains(reg)) {
            p_year = buf;
            break;
          }
        }
      }
    } else if (key == "by_statement") {
      QString list = data.toString().replace(nld, "").trimmed();
      foreach (QString author, list.split(",")) {
        QString name = author.trimmed().replace(nld, "");
        if (!name.isEmpty() && compareAuthors(name)) {
          p_authors.append(name);
        }
      }
    } else if (key == "authors") {
      QString name = data.toString().replace(nld, "").trimmed();
      if (!name.isEmpty() && compareAuthors(name)) {
        p_authors.append(name);
      }
    } else if (key == "publishers") {
      p_publisher.append(data.toString().trimmed());
    } else if (key == "publish_places") {
      p_publish_places.append(data.toString().trimmed());
    }
  }
}

QMap<QString, QVariant> IsbnData::data() {
  QMap<QString, QVariant> map;
  if (p_title.isEmpty())
    return map;

  map.insert("isbn", p_isbn);
  map.insert("url", p_url);
  map.insert("title", p_title);
  map.insert("title_extended", p_subtitle);
  map.insert("authors", p_authors.join(","));
  map.insert("pages", p_pages);
  map.insert("year", p_year);
  map.insert("images", p_images);

  QString publisher(p_publisher);
  if (!p_publish_places.isEmpty())
    publisher.append("/" + p_publish_places);

  map.insert("publisher", publisher);
  return map;
}

/**
  Alternativen isbnsearch.org oder die Google Books API
  Beispielseite für die Google Books API Nutzung:
    https://www.labnol.org/code/20020-query-book-by-isbn
*/
IsbnRequest::IsbnRequest(const QString &isbn, QObject *parent)
    : QObject{parent}, p_isbnKey("ISBN:" + isbn) {
  setObjectName("IsbnRequest");

  m_isbn = new IsbnData(isbn);

  QString req("https://openlibrary.org/api/books?bibkeys=ISBN:");
  req.append(isbn);
  req.append("&jscmd=data&format=json");
  p_url = QUrl(req);
}

bool IsbnRequest::setManager() {
  m_manager = new Networker(this);
  m_manager->setObjectName("NetworkAccessManager");
  connect(m_manager, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(replyFinished(QNetworkReply *)));

  return (m_manager != nullptr);
}

void IsbnRequest::read(const QJsonObject &obj) {
  QJsonObject sobj(obj);
  QJsonObject::iterator it;
  for (it = sobj.begin(); it != sobj.end(); ++it) {
    if (it.value().type() == QJsonValue::Array) {
      m_isbn->addArray(it.key(), it.value().toArray());
    } else if (it.value().type() == QJsonValue::String) {
      m_isbn->addValue(it.key(), it.value());
    } else {
      QJsonObject child = it.value().toObject();
      foreach (const QString &key, child.keys()) {
        m_isbn->addValue(key, child[key]);
      } // end foreach
      read(child);
    }
  }
}

void IsbnRequest::importResponse(const QByteArray &b) {
  if (ISBN_DEBUG_OUTPUT) {
    qDebug() << Q_FUNC_INFO << QString::fromLocal8Bit(b);
  }
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
  if (ISBN_DEBUG_OUTPUT) {
    qDebug() << "IsbnRequest::replyFinished" << reply->url();
  }
  if (reply->error() == QNetworkReply::NoError) {
    m_reply->deleteLater();
  }

  emit requestFinished((m_isbn->data().size() > 1));
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
      qDebug("IsbnRequest: buffer read error");
    }
    response += &buf[0];
  }
  buf.clear();
  importResponse(response);
}

void IsbnRequest::triggerRequest() {
  QNetworkRequest request = Networker::getRequest(p_url);
  if (setManager()) {
    m_reply = m_manager->get(request);
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), m_manager,
            SLOT(slotError(QNetworkReply::NetworkError)));

    connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), m_manager,
            SLOT(slotSslErrors(QList<QSslError>)));

    connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
  }
}

const QMap<QString, QVariant> IsbnRequest::getResponse() {
  return m_isbn->data();
}

IsbnRequest::~IsbnRequest() {
  if (m_isbn != nullptr)
    delete m_isbn;
}
