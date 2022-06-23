// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ABEBOOKS_PLUGIN_CONFIG_H
#define ABEBOOKS_PLUGIN_CONFIG_H

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

#ifndef PLUGIN_ABEBOOKS_DEBUG
#define PLUGIN_ABEBOOKS_DEBUG true
#endif

#endif // ABEBOOKS_PLUGIN_CONFIG_H
