// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ISBNREQUEST_H
#define ISBNREQUEST_H

#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

class Network;

class IsbnRequest : public QObject {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

public:
  explicit IsbnRequest(QObject *parent = nullptr);
  void setISBN(const QString &);
};

#endif // ISBNREQUEST_H
