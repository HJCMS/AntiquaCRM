// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_NETWORKREQUEST_H
#define ANTIQUACRM_NETWORKREQUEST_H

#include <AGlobal>
#include <QByteArray>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QString>
#include <QUrl>

namespace AntiquaCRM {

/**
 * @brief The ANetworkRequest class
 * @ingroup CoreLibrary
 */
class ANTIQUACRM_LIBRARY ANetworkRequest final : public QNetworkRequest {
private:
  const QString findCaBundleFile() const;
  const QSslConfiguration sslConfigguration();

public:
  explicit ANetworkRequest(const QUrl &remoteUrl);
  void setHeaderAcceptLanguage(const QByteArray &charset = ANTIQUACRM_CHARSET);
  void setHeaderUserAgent();
  void setHeaderAcceptText();
  void setHeaderAcceptJson();
  void setHeaderContentTypeJson(const QByteArray &charset = ANTIQUACRM_CHARSET);
  void setHeaderContentTypeXml(const QByteArray &charset = ANTIQUACRM_CHARSET);

  /**
   * @brief Header Cache Control Settings
   * @param cache (Cache-Control "Request,Response")
   * @note default: If param cache.isNull() then "no-cache,private" will set.
   * @code
   * - Request "no-cache"
   * The no-cache response directive indicates that the response can be stored
   * in caches, but the response must be validated with the origin server before
   * each reuse, even when the cache is disconnected from the origin server.
   *
   * If you want caches to always check for content updates while reusing stored
   * content, no-cache is the directive to use. It does this by requiring caches
   * to revalidate each request with the origin server.
   *
   * - Response "private"
   * You should add the private directive for user-personalized content,
   * especially for responses received after login and for sessions managed via
   * cookies. If you forget to add private to a response with personalized
   * content, then that response can be stored in a shared cache and end up
   * being reused for multiple users, which can cause personal information to
   * leak.
   * @endcode
   */
  void setHeaderCacheControl(const QByteArray &cache = QByteArray());

  void setHeaderContentLength(qint64 size);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_NETWORKREQUEST_H
