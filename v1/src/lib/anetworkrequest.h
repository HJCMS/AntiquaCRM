// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_NETWORKREQUEST_H
#define ANTIQUACRM_NETWORKREQUEST_H

#include <AGlobal>
#include <QByteArray>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QString>
#include <QUrl>

namespace AntiquaCRM {

class ANTIQUACRM_LIBRARY ANetworkRequest final : public QNetworkRequest {
private:
  const QSslConfiguration sslConfigguration();

public:
  explicit ANetworkRequest(const QUrl &remoteUrl);
  static const QByteArray antiquaCharset();

  void setHeaderAcceptLanguage();
  void setHeaderUserAgent();
  void setHeaderAcceptText();

  void setHeaderContentTypeJson();
  void setHeaderContentTypeXml(const QByteArray &charset = antiquaCharset());

  void setHeaderCacheControl(const QByteArray &cache = QByteArray("no-cache,private"));
  void setHeaderContentLength(qint64 size);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_NETWORKREQUEST_H
