// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKS_PLUGIN_CONFIG_H
#define ABEBOOKS_PLUGIN_CONFIG_H

#include <QString>
#include <QMetaType>
#include <AntiquaInterface>

/**
 * @def DATE_FORMAT
 * @ingroup Providers SQL Statements
 */
#ifndef DATE_FORMAT
#define DATE_FORMAT "yyyy-MM-dd hh:mm:ss"
#endif

/** @brief Konfigurationsgruppe */
#ifndef CONFIG_GROUP
#define CONFIG_GROUP "provider/abebooks"
#endif

/** @brief Wird für Menüeintrag und Gruppenzuweisung benötigt! */
#ifndef CONFIG_PROVIDER
#define CONFIG_PROVIDER "AbeBooks"
#endif

#ifndef PLUGIN_ID_PREFIX
#define PLUGIN_ID_PREFIX "AB-"
#endif

#ifndef PLUGIN_ABEBOOKS_DEBUG
#define PLUGIN_ABEBOOKS_DEBUG true
#endif

struct ANTIQUACORE_EXPORT AbeBooksAccess {
  QString user;
  QString key;
};
Q_DECLARE_METATYPE(AbeBooksAccess);

#endif // ABEBOOKS_PLUGIN_CONFIG_H
