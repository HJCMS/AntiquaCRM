// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKLOOKERQUERY_PLUGIN_H
#define BOOKLOOKERQUERY_PLUGIN_H

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
   * @brief read configuration
   */
  void initConfigurations();

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
  void writeErrorLog(const QByteArray &);

  /**
   * @brief Default GET Requests
   */
  bool getRequest(const QUrl &);

private Q_SLOTS:
  void registerAuthentic(const QJsonDocument &doc);
  void slotError(QNetworkReply::NetworkError error);
  void slotFinished(QNetworkReply *reply);
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
  void errorMessage(int code, const QString &msg);

  /**
   * @brief requestFinished
   * @param success - False when errors, otherwise false!
   */
  void requestFinished(bool success);

  /**
   * @brief responsed data
   */
  void response(const QJsonDocument &doc);
  void authenticResponse(const QJsonDocument &doc);

public Q_SLOTS:
  void queryList();
  void queryOrder(const QString &purchaseId);
  void authenticationRefresh();

public:
  explicit BooklookerRequester(QObject *parent = nullptr);

  /**
   * @brief get REST_API_TOKEN
   */
  const QString getToken();
};

#endif // BOOKLOOKERQUERY_PLUGIN_H
