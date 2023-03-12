// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUA_CACHECONFIG_H
#define ANTIQUA_CACHECONFIG_H

#include <QString>
#include <QtGlobal>

/**
 * @brief The CacheConfig class
 * @ingroup _cache
 */
class CacheConfig final {
public:
  QString file;
  QString indicator;
  QString message;
  explicit CacheConfig(const QString &name, const QString &pointer,
                       const QString &info);
};
Q_DECLARE_TYPEINFO(CacheConfig, Q_PRIMITIVE_TYPE);

#endif // ANTIQUA_CACHECONFIG_H
