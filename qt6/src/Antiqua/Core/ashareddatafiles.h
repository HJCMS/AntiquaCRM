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
 * @brief Data handling in the user directory
 *
 * This class reads and writes XML, Json files in given user directory.
 *
 * @ingroup CoreLibrary
 */
class ANTIQUACRM_LIBRARY ASharedDataFiles : public QDir {
public:
  /**
   * @param d - Working Directory
   */
  explicit ASharedDataFiles(const QDir &d = ASettings::getUserDataDir());

  /**
   * @brief Returning "_" with "Locale::bcp47Name" in lowercase.
   */
  const QString bcp47Suffix() const;

  /**
   * @brief Returns a list of writable files...
   */
  const QStringList dataFiles();

  /**
   * @brief Search standards, based on file extension.
   * Usually:
   * @li *.xml
   * @li *.sql
   * @li *.json
   */
  static const QStringList defaultFilter();

  /**
   * @brief Compares whether it has already been renewed on this day!
   * @param basename - File baseName
   * @param days     - days to past before updating cache
   * @param ext      - File extensions
   *
   * If this is the case then the method returns false!
   */
  bool needsUpdate(const QString &basename, int days = 7,
                   const QStringList &ext = defaultFilter());

  /**
   * @brief Search with basename and optional extension for availability!
   * @param basename - File baseName
   * @param ext      - File extensions
   *
   * @note Only writable files are considered!
   */
  bool fileExists(const QString &basename,
                  const QStringList &ext = defaultFilter());

  /**
   * @brief Save json document.
   * @param basename - File baseName
   * @param doc      - Json Document
   */
  bool storeJson(const QString &basename, const QJsonDocument &doc);

  /**
   * @brief Get json Document from file.
   * @param basename - File baseName
   */
  const QJsonDocument getJson(const QString &basename);

  /**
   * @brief Save XML document.
   * @param basename - File baseName
   * @param xml      - XML Document
   */
  bool storeXml(const QString &basename, const QDomDocument &xml);

  /**
   * @brief Get XML Document from file.
   * @param basename - File baseName
   */
  const QDomDocument getXML(const QString &basename);

  /**
   * @brief get Completer data from Json Database
   * @param basename - File baseName
   * @param key - Key from Json::Object
   */
  const QStringList getCompleterList(const QString &basename,
                                     const QString &key);
};

/**
 * @brief Data handling in user cache/temp directory
 *
 * This class reads and writes XML, Json files in given temp directory.
 *
 * @ingroup CoreLibrary
 */
class ANTIQUACRM_LIBRARY ASharedCacheFiles : public QDir {
public:
  /**
   * @param d - Working Directory
   */
  explicit ASharedCacheFiles(const QDir &d = ASettings::getUserTempDir());

  /**
   * @brief Save cache file with QTextStream
   * @param filename
   * @param data
   */
  bool storeTempFile(const QString &filename, const QByteArray &data);

  /**
   * @brief Save cache file with QTextStream
   * @param filename
   * @param data
   */
  bool storeTempFile(const QString &filename, const QString &data);

  /**
   * @brief Open cache file with QTextStream
   * @param filename
   */
  const QString getTempFile(const QString &filename);

  /**
   * @brief Open Cache Json Document
   * @param md5sum - The CacheBuilder stores the filenames with md5 hash
   */
  const QJsonObject getTempJson(const QString &md5sum);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SHAREDDATAFILES_H
