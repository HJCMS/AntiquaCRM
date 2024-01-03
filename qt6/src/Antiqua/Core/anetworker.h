// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_NETWORKER_H
#define ANTIQUACRM_NETWORKER_H

#include <AGlobal>
#include <QDomDocument>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QSslError>
#include <QUrl>

namespace AntiquaCRM {

/**
 * @class ANetworker
 * @brief Standard class for network queries.
 *
 * Language, character set and error messages are processed here.
 *
 * The network answers must be processed in the query classes!
 *
 * @ingroup CoreLibrary
 */
class ANTIQUACRM_LIBRARY ANetworker final : public QNetworkAccessManager {
  Q_OBJECT

private:
  /**
   * @brief Network Timeout in seconds
   */
  int transfer_timeout = 5;

  /**
   * @brief Querytype (JSON|XML)
   */
  const AntiquaCRM::NetworkQueryType queryType;

  /**
   * @brief Internal Network reply
   */
  QNetworkReply *m_reply;

private Q_SLOTS:
  void slotReadResponse();
  void slotError(QNetworkReply::NetworkError error);
  void slotSslErrors(const QList<QSslError> &list);

Q_SIGNALS:
  /**
   * @brief The signal is emitted if the query responses with errors.
   */
  void sendFinishedWithErrors();

  /**
   * @brief The signal is emitted if valid Json data responses.
   */
  void sendJsonResponse(const QJsonDocument &);

  /**
   * @brief The signal is emitted if valid Soap data responses.
   */
  void sendXmlResponse(const QDomDocument &);

public:
  /**
   * @param type   - NetworkQueryType
   * @param parent - parent object
   */
  explicit ANetworker(AntiquaCRM::NetworkQueryType type,
                      QObject *parent = nullptr);
  virtual ~ANetworker();

  /**
   * @brief Create a Login Request
   * @param url  - query url
   * @param body - HTTP_FORM_DATA_BODY
   */
  QNetworkReply *loginRequest(const QUrl &url, const QByteArray &data);

  /**
   * @brief Create a Json HTTP_POST request
   * @param url  - query url
   * @param body - HTTP_FORM_DATA_BODY
   *
   * If it's an https connection and the host is set with QUrl:setHost() in the
   * URL. Is SSL encryption used (otherwise not)!
   */
  QNetworkReply *
  jsonPostRequest(const QUrl &url, const QJsonDocument &body,
                  const QByteArray &charset = ANTIQUACRM_CHARSET);

  /**
   * @brief Create a Soap HTTP_POST request
   * @param url  - query url
   * @param body - HTTP_FORM_DATA_BODY
   * @param charset - some providers didn't support utf8!
   *
   * If it's an https connection and the host is set with QUrl:setHost() in the
   * URL. Is SSL encryption used (otherwise not)!
   */
  QNetworkReply *xmlPostRequest(const QUrl &url, const QDomDocument &body,
                                const QByteArray &charset = ANTIQUACRM_CHARSET);

  /**
   * @brief Create a Multipart request
   * @param url
   * @param name
   * @param body
   */
  QNetworkReply *jsonMultiPartRequest(const QUrl &url, const QString &name,
                                      const QJsonDocument &body);

  /**
   * @brief Default PUT send
   * @param url
   * @param data
   */
  QNetworkReply *putRequest(const QUrl &url, const QByteArray &data);

  /**
   * @brief Default GET request
   * @param url
   *
   * If it's an https connection and the host is set with QUrl:setHost() in the
   * URL. Is SSL encryption used (otherwise not)!
   */
  QNetworkReply *getRequest(const QUrl &url);
};
}; // namespace AntiquaCRM

#endif // ANTIQUACRM_NETWORKER_H
