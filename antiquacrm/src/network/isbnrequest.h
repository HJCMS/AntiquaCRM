// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ISBNREQUEST_H
#define ISBNREQUEST_H

#include <QtCore/QJsonObject>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QSslError>

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
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  const QString p_isbnKey;
  QUrl p_url;
  QNetworkReply *m_reply;
  QNetworkAccessManager *m_manager;
  QMap<QString,QVariant> p_data;
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
  const QMap<QString,QVariant> getResponse();
};

#endif // ISBNREQUEST_H
