// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKSQUERY_PLUGIN_H
#define ABEBOOKSQUERY_PLUGIN_H

#include <QDomDocument>
#include <QDomElement>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QSslError>
#include <QUrl>

#include <AntiquaCRM>
#include <AntiquaInterface>

#include "abebooksdocument.h"

class ANTIQUACORE_EXPORT AbeBooksRequester final
    : public QNetworkAccessManager {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  ApplSettings *config;
  QNetworkReply *m_reply;

  /**
   * @brief create Default NetworkRequest
   * @return NetworkRequest
   */
  const QNetworkRequest initRequest();

  /**
   * @brief Default Request Document
   * @return DomDocument
   */
  AbeBooksDocument createDocument();

  /**
   * @brief AbeBooks using ISO-8859-1
   */
  const QString toISO88591(const QString &str);

  bool createRequest(const QDomDocument &document);

private Q_SLOTS:
  void slotError(QNetworkReply::NetworkError error);
  void slotFinished(QNetworkReply *reply);
  void slotSslErrors(const QList<QSslError> &list);
  void replyFinished(QNetworkReply *reply);
  void replyReadyRead();

Q_SIGNALS:
  /**
   * @brief requestFinished
   * @param success - False when errors, otherwise false!
   */
  void requestFinished(bool success);

  /**
   * @brief Query Response Document
   * @param DomDocument
   */
  void response(const QDomDocument &document);

public:
  explicit AbeBooksRequester(QObject *parent = nullptr);
  void queryList();
  void queryOrder(const QString &purchaseId);
  void updateOrderStatus(const QString &purchaseId,
                         const QString &action,
                         const QStringList &articles);
};

#endif // ABEBOOKSQUERY_PLUGIN_H
