// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "anetworkcache.h"
#include "asettings.h"

namespace AntiquaCRM {

QMutex ANetworkCache::s_mutex;
QNetworkDiskCache *ANetworkCache::s_cache = nullptr;

ANetworkCache::ANetworkCache(QObject *parent) : QAbstractNetworkCache{parent} {
  AntiquaCRM::ASettings cfg(this);
  QMutexLocker l(&s_mutex);
  if (!s_cache) {
    s_cache = new QNetworkDiskCache;
    s_cache->setCacheDirectory(cfg.getUserTempDir().path());
  }
}

qint64 ANetworkCache::cacheSize() const {
  QMutexLocker l(&s_mutex);
  return s_cache->cacheSize();
}

QIODevice *ANetworkCache::data(const QUrl &url) {
  QMutexLocker l(&s_mutex);
  return s_cache->data(url);
}

void ANetworkCache::insert(QIODevice *device) {
  QMutexLocker l(&s_mutex);
  s_cache->insert(device);
}

QNetworkCacheMetaData ANetworkCache::metaData(const QUrl &url) {
  QMutexLocker l(&s_mutex);
  return s_cache->metaData(url);
}

QIODevice *ANetworkCache::prepare(const QNetworkCacheMetaData &data) {
  QMutexLocker l(&s_mutex);
  return s_cache->prepare(data);
}

bool ANetworkCache::remove(const QUrl &url) {
  QMutexLocker l(&s_mutex);
  return s_cache->remove(url);
}

void ANetworkCache::updateMetaData(const QNetworkCacheMetaData &data) {
  QMutexLocker l(&s_mutex);
  s_cache->updateMetaData(data);
}

void ANetworkCache::clear() {
  QMutexLocker l(&s_mutex);
  s_cache->clear();
}

} // namespace AntiquaCRM
