// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_ANETWORKCACHE_H
#define ANTIQUACRM_ANETWORKCACHE_H

#include <AGlobal>
#include <QAbstractNetworkCache>
#include <QMutex>
#include <QNetworkDiskCache>
#include <QObject>

namespace AntiquaCRM {

/**
 * @brief The ANetworkCache class
 * @ingroup core
 */
class ANTIQUACRM_LIBRARY ANetworkCache final : public QAbstractNetworkCache {
  Q_OBJECT

private:
  static QMutex s_mutex;
  static QNetworkDiskCache *s_cache;

public:
  explicit ANetworkCache(QObject *parent = nullptr);
  qint64 cacheSize() const;
  QIODevice *data(const QUrl &url);
  void insert(QIODevice *device);
  QNetworkCacheMetaData metaData(const QUrl &url);
  QIODevice *prepare(const QNetworkCacheMetaData &metaData);
  bool remove(const QUrl &url);
  void updateMetaData(const QNetworkCacheMetaData &metaData);
  void clear();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_ANETWORKCACHE_H
