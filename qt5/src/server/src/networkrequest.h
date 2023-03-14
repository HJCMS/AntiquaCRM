// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACMD_NETWORK_REQUEST_H
#define ANTIQUACMD_NETWORK_REQUEST_H

#include <QByteArray>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QUrl>

class NetworkRequest : public QNetworkRequest {
private:
  int tranfer_timeout = 5;
  const QSslConfiguration sslConfigguration();

public:
  explicit NetworkRequest(const QUrl &url);
  void setHeaderContentTypeJson();
  void setHeaderContentTypeXml();
  void setHeaderContentLength(qint64 size);
};

#endif // ANTIQUACMD_NETWORK_REQUEST_H
