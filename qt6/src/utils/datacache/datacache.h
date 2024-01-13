// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_DATACACHE_H
#define ANTIQUACRM_UTILS_DATACACHE_H

#include <AntiquaCRM>
#include <QList>
#include <QObject>

class DataCacheConfig;

/**
 * @class DataCache
 */
class DataCache final : public QObject {
  Q_OBJECT

private:
  AntiquaCRM::ASettings *m_cfg;
  AntiquaCRM::ASqlCore *m_sql;
  const QList<DataCacheConfig> configs();

Q_SIGNALS:
  void statusMessage(const QString &);

public:
  explicit DataCache(AntiquaCRM::ASettings *cfg, AntiquaCRM::ASqlCore *pgsql,
                     QObject *parent = nullptr);
  bool createCacheTarget();
  bool createCaches();
};

#endif // ANTIQUACRM_UTILS_DATACACHE_H
