// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SETTINGS_H
#define ANTIQUACRM_SETTINGS_H

#include <AGlobal>
#include <QDir>
#include <QHash>
#include <QObject>
#include <QSettings>
#include <QString>
#include <QVariant>

namespace AntiquaCRM {

/**
 * @brief AntiquaCRM Configuration Class
 * @ingroup core
 */
class ANTIQUACRM_LIBRARAY ASettings : public QSettings {
  Q_OBJECT

private:
  QHash<QString, QVariant> p_hash;

public:
  explicit ASettings(QObject *parent = nullptr);

  /**
   * @brief AntiquaCRM Configuration Domain
   * This is needed to Describe the Configuration path on Linux and Windows
   * Registry, to prevent mistakes with older Versions.
   */
  static const QString configDomain();

  /**
   * @brief get/set Default Values - if value is not set - create it.
   */
  const QVariant defaultValue(const QString &key, const QVariant &val);

  /**
   * @brief Read all Keys from Group
   */
  const QHash<QString, QVariant> &readGroupConfig(const QString &group);

  /**
   * @brief Read all Keys from Section in Group
   */
  const QHash<QString, QVariant> &readGroupSection(const QString &group,
                                                   const QString &section);

  /**
   * @brief Write all Keys in Group
   */
  void writeGroupConfig(const QString &, const QHash<QString, QVariant> &);

  /**
   * @brief Default Filter for all Dir requests!
   */
  static QDir::Filters directoryFilter();

  /**
   * @brief Default Plugin Namefilters
   */
  static const QStringList pluginSearchFilter();

  /**
   * @brief Application Plugin Directory
   */
  static const QDir getPluginDir(const QString &subTarget = QString());

  /**
   * @brief Translations Target
   */
  static const QDir getTranslationDir();

  /**
   * @brief Application Data Directory
   * @li Linux ${install_prefix}/share/antiquacrm
   * @li Windows ${binary_target}/data
   * @see aglobal.h
   */
  static const QDir getDataDir(const QString &name = QString());

  /**
   * @brief get/create Users Data Directory
   */
  static const QDir getUserDataDir();

  /**
   * @brief get Users Temporary Directory
   */
  static const QDir getUserTempDir();
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SETTINGS_H
