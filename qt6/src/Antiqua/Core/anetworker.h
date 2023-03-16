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
#include <QObject>
#include <QSslError>
#include <QUrl>

namespace AntiquaCRM {

/**
 * @brief Standard Klasse für Netzwerkabfragen.
 * @ingroup core
 *
 * Hier werden Sprache, Zeichensatz und Fehlermeldungen verabeitet.
 * Die Netzwerk antworten müssen in den Abfrage-Klassen verarbeitet werden!
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
  void sendFinishedWithErrors();
  void sendJsonResponse(const QJsonDocument &);
  void sendXmlResponse(const QDomDocument &);

public:
  explicit ANetworker(AntiquaCRM::NetworkQueryType type,
                      QObject *parent = nullptr);

  QNetworkReply *loginRequest(const QUrl &url, const QByteArray &data);

  /**
   * @brief Erstelle eine HTTP_POST Anfrage
   * @note Wenn es eine https Verbindung ist und der
   *    Host mit QUrl:setHost() in der URL gesetzt ist.
   *    Wird SSL Verschlüsselung verwendet (sonst nicht)!
   * @param url   Anfrage URL
   * @param body  HTTP_FORM_DATA_BODY
   * @return NetworkReply
   */
  QNetworkReply *jsonPostRequest(const QUrl &url, const QJsonDocument &body);
  QNetworkReply *xmlPostRequest(const QUrl &url, const QDomDocument &body);

  QNetworkReply *jsonMultiPartRequest(const QUrl &url, const QString &name,
                                      const QJsonDocument &body);

  /**
   * @brief Standard PUT Senden
   * @param url
   * @param data
   * @return
   */
  QNetworkReply *putRequest(const QUrl &url, const QByteArray &data);

  /**
   * @brief Standard GET Anfrage
   * @note Wenn es eine https Verbindung ist und der
   *    Host mit QUrl:setHost() in der URL gesetzt ist.
   *    Wird SSL Verschlüsselung verwendet (sonst nicht)!
   * @param url
   * @return
   */
  QNetworkReply *getRequest(const QUrl &url);

  /**
   * @brief Aufräumen
   */
  virtual ~ANetworker();
};
}; // namespace AntiquaCRM

#endif // ANTIQUACRM_NETWORKER_H
