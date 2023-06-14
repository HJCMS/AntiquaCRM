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
 * @class ANetworkCache
 * @brief The ANetworkCache class
 *
 * @ingroup CoreLibrary
 */
class ANTIQUACRM_LIBRARY ANetworkCache final : public QAbstractNetworkCache {
  Q_OBJECT

private:
  static QMutex s_mutex;
  static QNetworkDiskCache *s_cache;

public:
  /**
   * @param parent - parent object
   */
  explicit ANetworkCache(QObject *parent = nullptr);

  /**
   * @brief get cache size
   */
  qint64 cacheSize() const;

  /**
   * @brief get device
   * @param url
   */
  QIODevice *data(const QUrl &url);

  /**
   * @brief insert cache
   * @param device
   */
  void insert(QIODevice *device);

  /**
   * @brief get meta data
   * @param url
   */
  QNetworkCacheMetaData metaData(const QUrl &url);

  /**
   * @brief prepare io device
   * @param metaData
   */
  QIODevice *prepare(const QNetworkCacheMetaData &metaData);

  /**
   * @brief remove from cache
   * @param url
   */
  bool remove(const QUrl &url);

  /**
   * @brief update meta data
   * @param metaData
   */
  void updateMetaData(const QNetworkCacheMetaData &metaData);

  void clear();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_ANETWORKCACHE_H
