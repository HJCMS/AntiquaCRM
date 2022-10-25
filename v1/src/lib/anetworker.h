// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_NETWORKER_H
#define ANTIQUACRM_NETWORKER_H

#include <AGlobal>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QSslError>
#include <QUrl>

namespace AntiquaCRM {

/**
 * @brief Standard Klasse für Netzwerkabfragen.
 * @class ANetworker
 * @ingroup Network
 *
 * Hier werden Sprache, Zeichensatz und Fehlermeldungen verabeitet.
 * Die Netzwerk antworten müssen in den Abfrage-Klassen verarbeitet werden!
 */
class ANTIQUACRM_LIBRARAY ANetworker final : public QNetworkAccessManager {
  Q_OBJECT

private:
  int tranfer_timeout = 20;
  QNetworkReply *m_reply;

private Q_SLOTS:
  /**
   * @brief Wenn die Anfrage beendet wurde.
   */
  void slotFinished(QNetworkReply *reply);

  /**
   * @brief readyRead() => readResponse()
   */
  void readResponse();

public Q_SLOTS:
  void slotError(QNetworkReply::NetworkError error);
  void slotSslErrors(const QList<QSslError> &list);

Q_SIGNALS:
  /**
   * @brief Anfrage abgeschlossen
   * @param errors - Wenn ja true
   */
  void requestFinished(bool errors);

  void jsonResponse(const QJsonDocument &json);

public:
  explicit ANetworker(QObject *parent = nullptr);

  QNetworkReply *loginRequest(const QUrl &url, const QByteArray &data);

  /**
   * @brief Erstelle eine Json HTTP_POST Anfrage
   * @note Wenn es eine https Verbindung ist und der
   *    Host mit QUrl:setHost() in der URL gesetzt ist.
   *    Wird SSL Verschlüsselung verwendet (sonst nicht)!
   * @param url   Anfrage URL
   * @param name  HTTP_FORM_DATA_NAME "form-data; name={key}"
   * @param body  HTTP_FORM_DATA_BODY
   * @return NetworkReply
   */
  QNetworkReply *jsonPostRequest(const QUrl &url, const QJsonDocument &body);

  QNetworkReply *jsonMultiPartRequest(const QUrl &url, const QString &name,
                                      const QJsonDocument &body);

  /**
   * @brief Standard GET Anfrage
   * @note Wenn es eine https Verbindung ist und der
   *    Host mit QUrl:setHost() in der URL gesetzt ist.
   *    Wird SSL Verschlüsselung verwendet (sonst nicht)!
   * @param url
   * @return
   */
  QNetworkReply *jsonGetRequest(const QUrl &url);

  /**
   * @brief Aufräumen
   */
  virtual ~ANetworker();
};
}; // namespace AntiquaCRM

#endif // ANTIQUACRM_NETWORKER_H
