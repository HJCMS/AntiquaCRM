// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ISBNREQUEST_H
#define ISBNREQUEST_H

#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslError>

class IsbnData {
private:
  const QString p_isbn;
  QUrl p_url;
  QString p_title;
  QString p_subtitle;
  QStringList p_authors;
  QString p_publisher;
  QString p_publish_places;
  QString p_pages;
  QString p_year;
  bool p_images;
  bool compareAuthors(const QString &a);

public:
  explicit IsbnData(const QString &isbn);
  void addArray(const QString &key, const QJsonArray &array);
  void addValue(const QString &key, const QJsonValue &data);
  QMap<QString, QVariant> data();
};

/**
   @brief The IsbnRequest Klasse
    @li https://openlibrary.org/developers/api
    @li https://openlibrary.org/dev/docs/api/books
    @li https://openlibrary.org/dev/docs/api/covers
   @code
    {"ISBN:{ISBN}":
      {
        "url": "",
        "title": "",
        "authors": [{
          "url": "",
          "name": ""
        }],
        "number_of_pages": 0,
        "publishers": [{
          "name": ""
        }],
        "publish_places": [{
          "name": ""
        }],
        "publish_date": "Year",
        "cover": {
          "small": "https://covers.openlibrary.org/b/id/%-S.jpg",
          "medium": "https://covers.openlibrary.org/b/id/%-M.jpg",
          "large": "https://covers.openlibrary.org/b/id/%-L.jpg"
        }
      }
    }
    @endcode
 */
class IsbnRequest : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  IsbnData *m_isbn;
  const QString p_isbnKey;
  QUrl p_url;
  QNetworkReply *m_reply;
  QNetworkAccessManager *m_manager;
  QJsonObject p_respJsonObject;
  bool setManager();
  void read(const QJsonObject &);
  void importResponse(const QByteArray &);

private Q_SLOTS:
  void replyFinished(QNetworkReply *);
  void slotError(QNetworkReply::NetworkError);
  void slotSslErrors(QList<QSslError>);
  void slotReadyRead();

Q_SIGNALS:
  void requestFinished(bool);

public:
  explicit IsbnRequest(const QString &, QObject *parent = nullptr);
  void triggerRequest();
  const QMap<QString, QVariant> getResponse();
  ~IsbnRequest();
};

#endif // ISBNREQUEST_H
