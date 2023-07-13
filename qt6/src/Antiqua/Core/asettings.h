// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_SETTINGS_H
#define ANTIQUACRM_SETTINGS_H

#include <AGlobal>
#include <QDir>
#include <QHash>
#include <QMetaType>
#include <QObject>
#include <QSettings>
#include <QString>
#include <QVariant>

namespace AntiquaCRM {

/**
 * @class ASettings
 * @brief AntiquaCRM Configuration Class
 *
 * This is the Primary Configuration class to get Targets and Configuration Keys
 * and grouped Sections.
 *
 * @ingroup CoreLibrary
 */
class ANTIQUACRM_LIBRARY ASettings : public QSettings {
  Q_OBJECT

public:
  /**
   * @param parent - parent object
   */
  explicit ASettings(QObject *parent = nullptr);

  /**
   * @brief AntiquaCRM Configuration Domain
   *
   * This is needed to Describe the Configuration path on Linux and Windows
   * Registry, to prevent mistakes with older Versions.
   */
  static const QString configDomain();

  /**
   * @brief Config test for if exists and a value is set!
   * @param pkey - absolute path to key
   *
   * This function is a extended call from QSettings::contains
   */
  bool check(const QString &pkey) const;

  /**
   * @brief Get value with given Path and MetaType.
   * @param key - config path to key
   * @param type - required Meta type
   */
  const QVariant getValue(const QString &key, const QMetaType &type) const;

  /**
   * @brief Returns Directory from Section[dirs]
   *
   * if required Directory not exists, a „QStandardPaths“ returned!
   *
   * @code
   *   AntiquaCRM::ASettings config(this);
   *   // ANTIQUACRM_ARCHIVE_IMAGES or QStandardPaths::PicturesLocation
   *   qDebug() << config.getArchivPath(ANTIQUACRM_ARCHIVE_IMAGES);
   * @endcode
   */
  const QDir getArchivPath(const QString &section);

  /**
   * @brief get value from group and key
   * @param group - section group
   * @param key   - key from group
   * @param fallback - what it says
   */
  const QVariant groupValue(const QString &group, const QString &key,
                            const QVariant &fallback = QVariant());

  /**
   * @brief Payment Settings with Default values.
   */
  const QHash<QString, QVariant> payment();

  /**
   * @brief Default Filter for all Dir requests!
   *
   * Default filter is:
   *  @li QDir::Dirs
   *  @li QDir::Files
   *  @li QDir::NoSymLinks
   *  @li QDir::NoDotAndDotDot
   */
  static QDir::Filters directoryFilter();

  /**
   * @brief Default Plugin Search filter extension.
   *
   * Default filter is:
   * @li Windows: *.dll
   * @li Linux: *.so
   */
  static const QStringList pluginSearchFilter();

  /**
   * @brief Application Plugin Directory
   * @param target - plugin sub category
   *
   * Creates a search target from Install Plugin Directory.
   */
  static const QDir getPluginDir(const QString &target = QString());

  /**
   * @brief Translations Target
   *
   * Search Translation directory in @ref ANTIQUACRM_TRANSLATION_TARGET
   */
  static const QDir getTranslationDir();

  /**
   * @brief Get Application data directory
   *
   * Search Datafiles directory in @ref ANTIQUACRM_DATA_TARGET
   */
  static const QDir getDataDir(const QString &name = QString());

  /**
   * @brief Users Data Directory
   *
   * Generate QDir from QStandardPaths::AppLocalDataLocation
   *
   * @note Creates it when not exists and grandet permissions exists.
   * @return QDir filtered by @ref directoryFilter
   */
  static const QDir getUserDataDir();

  /**
   * @brief Temporary User Directory
   *
   * Generate QDir from QStandardPaths::CacheLocation
   *
   * @note Creates it when not exists and grandet permissions exists.
   * @return QDir filtered by @ref directoryFilter
   */
  static const QDir getUserTempDir();
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SETTINGS_H
