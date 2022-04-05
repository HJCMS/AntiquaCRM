// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef NETJSONREQUEST_H
#define NETJSONREQUEST_H

#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

class NetJsonRequest : public QNetworkAccessManager {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  QNetworkRequest p_request;
  QNetworkReply *m_networkReply;

private:
  void replyFinished();
  void readReply();

public:
  explicit NetJsonRequest(QObject *parent = nullptr);
  void queryJsonUrl(const QString &);
};

#endif // NETJSONREQUEST_H
