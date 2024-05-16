// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_DATACACHECONFIG_H
#define ANTIQUACRM_UTILS_DATACACHECONFIG_H

#include <QString>
#include <QtGlobal>

class DataCacheConfig final {
public:
  QString file;
  QString indicator;
  QString message;
  int pastDays;
  explicit DataCacheConfig(const QString &name, const QString &pointer,
                           const QString &info, int days = 7);
};
Q_DECLARE_TYPEINFO(DataCacheConfig, Q_PRIMITIVE_TYPE);

#endif // ANTIQUACRM_UTILS_DATACACHECONFIG_H
