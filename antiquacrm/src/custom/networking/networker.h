// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef NETWORKER_NETWORKING_H
#define NETWORKER_NETWORKING_H

#include <QNetworkAccessManager>
#include <QSslConfiguration>
#include <QSslError>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

class ApplSettings;

class Networker : public QNetworkAccessManager {
  Q_OBJECT

private:
  ApplSettings *config;
  QNetworkReply *m_reply;

  const QSslConfiguration sslConfigguration();

public Q_SLOTS:
  void slotReplyFinished(QNetworkReply *reply);
  void slotError(QNetworkReply::NetworkError error);
  void slotSslErrors(const QList<QSslError> &list);

public:
  explicit Networker(QObject *parent = nullptr);
  static const QNetworkRequest getRequest(const QUrl &url);
  ~Networker();
};

#endif // NETWORKER_NETWORKING_H
