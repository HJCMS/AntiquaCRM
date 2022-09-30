// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKLOOKERQUERY_PLUGIN_H
#define BOOKLOOKERQUERY_PLUGIN_H

#include <QFileInfo>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QSslError>
#include <QUrl>

#include <AntiquaCRM>
#include <AntiquaInterface>

/**
 * @brief Booklooker Requester
 * @ingroup Booklooker
 * https://www.booklooker.de/pages/api_orders.php?version=2.0
 */
class ANTIQUACORE_EXPORT BooklookerRequester final
    : public QNetworkAccessManager {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  ApplSettings *config;
  QNetworkReply *m_reply;

  /**
   * @brief Default API Url
   */
  QUrl p_baseUrl;

  /**
   * @brief REST_API_KEY
   */
  QString p_apiKey;

  /**
   * @brief Letze Anfrage Operation
   * Wird für das Logfile verwendet.
   */
  QString p_operation;
  const QFileInfo operationTempFile(const QString &op = QString("order_list"));

  /**
   * @brief read configuration
   */
  void initConfigurations();

  /**
   * @brief Standard Anfrage Url erstellen
   * @param section  - Abfrageteil im Url Pfad
   * @return Rest API URL
   */
  const QUrl apiQuery(const QString &section);

  /**
   * @brief create a NetworkRequest
   */
  const QNetworkRequest newRequest(const QUrl &url);

  /**
   * @brief Authentication with REST_API_KEY
   */
  void authentication();

  /**
   * @brief Wenn replyReadyRead mit Fehlern behaftet ist.
   * Den Datenstrom zum Debuggen in ein Logfile schreiben.
   */
  void writeErrorLog(const QByteArray &data);

  /**
   * @brief Den Datenstrom zum Debuggen in ein Logfile schreiben.
   */
  void writeResponseLog(const QJsonDocument &doc);

  /**
   * @brief Rückgabegröße der Anfrage
   */
  qint64 replyHeaderLength();

  /**
   * @brief Standard DELETE Protokoll
   */
  bool deleteRequest(const QUrl &);

  /**
   * @brief Standard PUT Protokoll
   */
  bool putRequest(const QUrl &, const QByteArray &data);

  /**
   * @brief Standard GET Abfrage
   */
  bool getRequest(const QUrl &);

private Q_SLOTS:
  void registerAuthentic(const QJsonDocument &doc);
  void slotError(QNetworkReply::NetworkError error);
  void slotSslErrors(const QList<QSslError> &list);
  void replyFinished(QNetworkReply *reply);
  void replyReadyRead();

Q_SIGNALS:
  /**
   * @brief Authentic Process is Required
   */
  void authenticFinished();

  /**
   * @brief Error Messages
   */
  void errorMessage(Antiqua::ErrorStatus type, const QString &msg);

  /**
   * @brief requestFinished
   * @param success - False when errors, otherwise false!
   */
  void requestFinished(bool success);

  /**
   * @brief responsed data
   */
  void response(const QJsonDocument &doc);

public Q_SLOTS:
  /**
   * @brief Methode zum Aktualisieren des Tokens
   */
  void authenticationRefresh();

  /**
   * @brief Bestellungen der letzten 4 Tage
   * @see ANTIQUA_QUERY_PASTDAYS
   * Download aller Bestellungen eines bestimmten Tages bzw. Zeitintervalls.
   * https://api.booklooker.de/2.0/order?token=REST_API_TOKEN
   */
  void queryList();

public:
  explicit BooklookerRequester(QObject *parent = nullptr);

  /**
   * @brief Zulässige Nachrichten Typen
   * @code
   *  https://api.booklooker.de/2.0/order_message?token=REST_API_TOKEN
   * @endcode
   * @return QStringList
   */
  Q_DECL_DEPRECATED static const QStringList orderMessageTypes();

  /**
   * @brief Sende Statusänderungen für Bestellung
   * @param orderId
   * @param status
   * @code
   *  https://api.booklooker.de/2.0/order_status?token=REST_API_TOKEN
   * @endcode
   */
  void queryUpdateOrderStatus(const QString &orderId, const QString &status);

  /**
   * @brief Stornieren einer kompletten Bestellung
   * @param orderId
   * @code
   *  https://api.booklooker.de/2.0/order_cancel?token=REST_API_TOKEN
   * @endcode
   */
  void queryUpdateOrderCancel(const QString &orderId);

  /**
   * @brief Bestellung abfragen
   * Download aller Bestellungen eines bestimmten Tages bzw. Zeitintervalls.
   * https://api.booklooker.de/2.0/order?token=REST_API_TOKEN
   */
  void queryOrder(const QString &orderId);

  /**
   * @brief Artikel mit Nummer zurücksetzen
   * Sofortiges Löschen eines einzelnen Artikels.
   * https://api.booklooker.de/2.0/article?token=REST_API_TOKEN
   */
  void queryArticleReset(const QString &orderNo);

  /**
   * @brief get REST_API_TOKEN
   */
  const QString getToken();

  /**
   * @brief Bei Status "NOK" die Standard Fehlermeldungen abfangen!
   * @param doc
   */
  static const QString getResponseErrors(const QJsonDocument &doc);
};

#endif // BOOKLOOKERQUERY_PLUGIN_H
