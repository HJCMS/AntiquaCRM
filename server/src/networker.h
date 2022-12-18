// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_NETWORKER_H
#define ANTIQUACRM_NETWORKER_H

#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QSslError>
#include <QTextCodec>
#include <QUrl>

class NetworkRequest;

class Networker final : public QNetworkAccessManager {
  Q_OBJECT

private:
  QNetworkReply *m_reply;
  QTextCodec *m_textCodec;

private Q_SLOTS:
  void slotFinished(QNetworkReply *);
  void slotError(QNetworkReply::NetworkError);
  void slotSslErrors(const QList<QSslError> &);

Q_SIGNALS:
  void sendResponse(const QByteArray &);

public Q_SLOTS:
  void slotReadResponse();

public:
  explicit Networker(QObject *parent = nullptr);
  QNetworkReply *loginRequest(const NetworkRequest &, const QByteArray &);
  QNetworkReply *postRequest(const NetworkRequest &, const QByteArray &);
  QNetworkReply *customRequest(const NetworkRequest &, const QByteArray &);
  QNetworkReply *putRequest(const NetworkRequest &, const QByteArray &);
  QNetworkReply *getRequest(const NetworkRequest &);
};

#endif // ANTIQUACRM_NETWORKER_H
