// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACMD_NETWORKER_H
#define ANTIQUACMD_NETWORKER_H

#include <QAbstractNetworkCache>
#include <QList>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkDiskCache>
#include <QNetworkReply>
#include <QObject>
#include <QSslError>
#include <QTextCodec>
#include <QUrl>

// subclass usage
#include "aglobal.h"

class NetworkRequest;

class NetworkCache final : public QAbstractNetworkCache {
  Q_OBJECT

private:
  static QMutex s_mutex;
  static QNetworkDiskCache *s_cache;

public:
  explicit NetworkCache(QObject *parent = nullptr);
  qint64 cacheSize() const;
  QIODevice *data(const QUrl &url);
  void insert(QIODevice *device);
  QNetworkCacheMetaData metaData(const QUrl &url);
  QIODevice *prepare(const QNetworkCacheMetaData &metaData);
  bool remove(const QUrl &url);
  void updateMetaData(const QNetworkCacheMetaData &metaData);
  void clear();
};

class Networker final : public QNetworkAccessManager {
  Q_OBJECT

private:
  QNetworkReply *m_reply;
  QTextCodec *m_textCodec;

private Q_SLOTS:
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

#endif // ANTIQUACMD_NETWORKER_H
