// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cacheconfig.h"

CacheConfig::CacheConfig(const QString &name, const QString &pointer,
                         const QString &info)
    : file{name}, indicator{pointer}, message{info} {}
