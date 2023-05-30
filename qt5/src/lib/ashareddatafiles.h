// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_SHAREDDATAFILES_H
#define ANTIQUACRM_SHAREDDATAFILES_H

#include <AGlobal>
#include <ASettings>
#include <QDir>
#include <QDomDocument>
#include <QJsonDocument>
#include <QString>
#include <QStringList>

namespace AntiquaCRM {

/**
 * @class ASharedDataFiles
 * @brief Data handling in the user directory
 * @ingroup core
 */
class ANTIQUACRM_LIBRARY ASharedDataFiles : public QDir {

public:
  explicit ASharedDataFiles(const QDir &d = ASettings::getUserDataDir());

  /**
   * @brief Returns a list of writable files ...
   */
  const QFileInfoList listFileInfos() const;

  /**
   * @brief Default for search, By file extension.
   * normally:
   * @li *.xml
   * @li *.sql
   * @li *.json
   */
  static const QStringList fileSuffixes();

  /**
   * @brief Basename list of Files which only updated once per week.
   */
  static const QStringList weeklyFileUpdate();

  /**
   * @brief Check whether this file is available and/or requires an update.
   * @return true if update is required
   */
  bool needsUpdate(const QString &basename,
                   const QStringList &ext = fileSuffixes());

  /**
   * @brief Search by base name and optional extension for availability!
   * @note Only writable files are considered!
   */
  bool fileExists(const QString &basename,
                  const QStringList &ext = fileSuffixes());

  /**
   * @brief Remove a file from destination
   * @param basename
   * @param ext
   */
  bool removeFile(const QString &basename,
                  const QStringList &ext = fileSuffixes());

  /**
   * @brief Save json document.
   */
  bool storeJson(const QString &basename, const QJsonDocument &doc);

  /**
   * @brief Get json document from file.
   */
  const QJsonDocument getJson(const QString &basename);

  /**
   * @brief Save XML document.
   */
  bool storeXml(const QString &basename, const QDomDocument &xml);

  /**
   * @brief Take XML document from file.
   */
  const QDomDocument getXML(const QString &basename);
};

/**
 * @class ASharedCacheFiles
 * @brief Data handling in user cache/temp directory
 * @ingroup core
 */
class ANTIQUACRM_LIBRARY ASharedCacheFiles : public QDir {
public:
  explicit ASharedCacheFiles(const QDir &d = ASettings::getUserTempDir());

  /**
   * @brief Save cache file.
   * @note Conversion QTextStream
   */
  bool storeTempFile(const QString &filename, const QByteArray &data);

  /**
   * @brief Save cache file.
   * @note QTextStream
   */
  bool storeTempFile(const QString &filename, const QString &data);

  /**
   * @brief Open cache file
   */
  const QString getTempFile(const QString &filename);

  /**
   * @brief Open Json Cache Document
   * @param md5sum - The CacheBuilder stores the filenames with md5 hash
   */
  Q_DECL_DEPRECATED const QJsonObject getTempJson(const QString &md5sum);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SHAREDDATAFILES_H
