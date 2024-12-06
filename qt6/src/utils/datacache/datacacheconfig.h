// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_DATACACHECONFIG_H
#define ANTIQUACRM_UTILS_DATACACHECONFIG_H

#include <QDateTime>
#include <QString>
#include <QtGlobal>

class DataCacheConfig final {
public:
  const QString file;
  const QString indicator;
  const QString message;
  const int pastDays;
  const QDateTime timeStamp;
  explicit DataCacheConfig(const QString& name, const QString& pointer, const QString& info,
                           int days = 7, const QDateTime& t = QDateTime::currentDateTime());
};
Q_DECLARE_TYPEINFO(DataCacheConfig, Q_PRIMITIVE_TYPE);

#endif // ANTIQUACRM_UTILS_DATACACHECONFIG_H
