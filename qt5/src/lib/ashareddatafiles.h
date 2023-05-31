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
 * @brief File handling for QStandardPaths::AppDataLocation
 * @ingroup core
 */
class ANTIQUACRM_LIBRARY ASharedDataFiles : public QDir {

public:
  explicit ASharedDataFiles(const QDir &d = ASettings::getUserDataDir());

  /**
   * @brief Returns a list of writable files ...
   * Defined function call with QDir::entryInfoList
   */
  const QFileInfoList listFileInfos() const;

  /**
   * @brief List of file extension suffixes.
   * @return QStringList({"*.xml","*.sql","*.json"})
   */
  static const QStringList fileSuffixes();

  /**
   * @brief A basename list of Files which only updated once per week.
   * @return QStringList({"postalcodes", "publishers", "storagelocations"})
   */
  static const QStringList weeklyFileUpdate();

  /**
   * @brief Check whether this file is available and/or requires an update.
   * @return true if update is required
   */
  bool needsUpdate(const QString &basename,
                   const QStringList &ext = fileSuffixes());

  /**
   * @brief File search with basename and file extension for availability!
   * @note Only writable files are considered!
   */
  bool fileExists(const QString &basename,
                  const QStringList &ext = fileSuffixes());

  /**
   * @brief Remove a file from destination
   * @param basename - search file basename in current target
   * @param ext      - file extension for availability
   * @return boolean - true if file has removed
   */
  bool removeFile(const QString &basename,
                  const QStringList &ext = fileSuffixes());

  /**
   * @brief Save json document.
   * @param basename - file basename for current target
   * @param doc      - json document
   * @return boolean - true when success
   */
  bool storeJson(const QString &basename, const QJsonDocument &doc);

  /**
   * @brief Get json document from file.
   * @param basename - find file basename in current target
   */
  const QJsonDocument getJson(const QString &basename);

  /**
   * @brief Save XML document.
   * @param basename - file basename for current target
   * @param xml      - dom document
   * @return boolean - true when success
   */
  bool storeXml(const QString &basename, const QDomDocument &xml);

  /**
   * @brief Take XML document from file.
   * @param basename - find file basename in current target
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
   * @param filename - file name with extension
   * @param data     - Converted to QTextStream
   */
  bool storeTempFile(const QString &filename, const QByteArray &data);

  /**
   * @brief Save cache file.
   * @param filename - file name with extension
   * @param data     - QTextStream
   */
  bool storeTempFile(const QString &filename, const QString &data);

  /**
   * @brief Open cache file
   * @param filename - file name in temporary target
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
