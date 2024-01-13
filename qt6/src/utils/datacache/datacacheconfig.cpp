// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "datacacheconfig.h"

DataCacheConfig::DataCacheConfig(const QString &name, const QString &pointer,
                                 const QString &info, int days)
    : file{name}, indicator{pointer}, message{info}, pastDays{days} {}
