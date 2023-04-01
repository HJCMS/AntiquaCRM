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
 * @brief Datenbehandlung im Benutzerverzeichnis
 * @ingroup CoreLibrary
 */
class ANTIQUACRM_LIBRARY ASharedDataFiles : public QDir {
public:
  explicit ASharedDataFiles(const QDir &d = ASettings::getUserDataDir());

  /**
   * @brief Gibt eine Liste der beschreibaren Dateien zurück ...
   */
  const QStringList dataFiles();

  /**
   * @brief Standard für Suche, Anhand der Dateierweiterrung.
   * Üblicherweise:
   * @li *.xml
   * @li *.sql
   * @li *.json
   */
  static const QStringList defaultFilter();

  /**
   * @brief Vergleicht ob die an diesem tag schon erneuert wurde!
   * Wenn dies der fall ist dann gibt die Methode false zurück!
   * @param basename - File baseName
   * @param ext      - File extensions
   */
  bool needsUpdate(const QString &basename,
                   const QStringList &ext = defaultFilter());

  /**
   * @brief Such mit dem Basisnamen und optionaler Erweiterung nach
   * Verfügbarkeit!
   * @note Nur Beschreibbare Dateien werden berücksichtigt!
   * @param basename - File baseName
   * @param ext      - File extensions
   */
  bool fileExists(const QString &basename,
                  const QStringList &ext = defaultFilter());

  /**
   * @brief Json Dokument speichern.
   * @param basename - File baseName
   * @param doc      - Json Document
   */
  bool storeJson(const QString &basename, const QJsonDocument &doc);

  /**
   * @brief Nehme Json Dokument aus Datei.
   * @param basename - File baseName
   */
  const QJsonDocument getJson(const QString &basename);

  /**
   * @brief XML Dokument speichern.
   * @param basename - File baseName
   * @param xml      - XML Document
   */
  bool storeXml(const QString &basename, const QDomDocument &xml);

  /**
   * @brief Nehme XML Dokument aus Datei.
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
 * @brief Datenbehandlung im Benutzer Cache-/ Tempverzeichnis
 * @ingroup CoreLibrary
 */
class ANTIQUACRM_LIBRARY ASharedCacheFiles : public QDir {
public:
  explicit ASharedCacheFiles(const QDir &d = ASettings::getUserTempDir());

  /**
   * @brief Cache Datei speichern.
   * @note Umwandlung QTextStream
   */
  bool storeTempFile(const QString &filename, const QByteArray &data);

  /**
   * @brief Cache Datei speichern.
   * @note QTextStream
   */
  bool storeTempFile(const QString &filename, const QString &data);

  /**
   * @brief Öffne Cache Datei
   */
  const QString getTempFile(const QString &filename);

  /**
   * @brief Öffne Cache Json Dokument
   * @param md5sum - Der CacheBuilder speichert die Dateinamen mit md5 hash
   */
  const QJsonObject getTempJson(const QString &md5sum);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SHAREDDATAFILES_H
