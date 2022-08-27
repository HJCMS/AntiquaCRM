// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef LIB_HOMEBASE_CONFIG_H
#define LIB_HOMEBASE_CONFIG_H

#include <Qt>
#include <QtGlobal>

/**
 * @def HOMEBASE3_EXPORT
 * @brief library export definition
 */
#if defined(ANTIQUACORE_LIBRARAY)
#define HOMEBASE3_EXPORT Q_DECL_EXPORT
#else
#define HOMEBASE3_EXPORT Q_DECL_IMPORT
#endif

#ifndef HOMEBASE3_DATETIME_FORMAT
#define HOMEBASE3_DATETIME_FORMAT Qt::ISODate
#endif

#endif // LIB_HOMEBASE_CONFIG_H
