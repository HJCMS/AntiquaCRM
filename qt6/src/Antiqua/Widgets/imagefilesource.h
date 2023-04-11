// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_IMAGESFILEOURCE_H
#define ANTIQUACRM_WIDGETS_IMAGESFILEOURCE_H

#include <AntiquaCRM>
#include <QByteArray>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QPixmap>
#include <QSize>

namespace AntiquaCRM {

/**
 * @class ImageFileSource
 * @brief this class handles File and SQL-Database operations.
 * AntiquaCRM using zero filled id numbers for Image Filenames.
 * This will provide a better view an sorting in System File Managers.
 * It also supports SQL:SELECT/INSERT/REMOVE operation for thumbnails.
 *
 * AntiquaCRM identify Source Images with Article Numbers in a Filesystem Tree,
 * only Thumbnails will stored in the Database. You can set different targets in
 * the Source tree to categorize a temporary Destination.
 * e.g.: Books, Media or Various.
 *
 * A Server cron job will read this temporary dirs and upload this images with
 * SFTP client to your Web-Storage. When image upload success, the image are
 * moved hierarchical to the „Sources“ tree.
 *
 * You can set some targets in configuration group „dirs“ with:
 * - „dirs/images“ = Primary images source storage for recursive search.
 * - „dirs/import“ = Opens this target first when import pictures. e.g.: Camera
 *
 * This code example will show how AntiquaCRM Server images saved.
 * @code
 * smb://serverhost:445/release/archiv
 * ├── Imported    // temporary destinations
 * │   ├── Prints
 * │   ├── Books
 * │   ├── Media
 * │   ├── Various
 * │   └── ...
 * │
 * ├── Sources    // Sources tree where images stored by cron-job
 * │   ├── 2022   // stored in year
 * │   │   ├── 01 // stored in month of year
 * │   │   ├── 02
 * │   │   ├── 03
 * │   │   ├── 04
 * │   │   ...
 * │   │
 * │   └── 2023
 * │       ├── 01
 * │       ...
 * │
 * └── ...
 * @endcode
 *
 * @warning Do not use the Server Sources directory to store new imports.
 *
 * @note This class will not touch the Server Sources Directory
 *
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY ImageFileSource final : public QFileInfo {
private:
  /**
   * @brief Artikel number
   * Needed later for saving!
   */
  qint64 p_fileId;

  /**
   * @brief Where to save the image!
   */
  QString p_tempTarget;

  /**
   * @brief cached Pixmap
   * If the Image is not equal to Source, a copy stored here.
   */
  QPixmap p_pixmap;

  /**
   * @brief Import Thumbnail data from Database
   * @param data - rwa data from database
   */
  bool loadFromDatabase(const QByteArray &data);

public:
  /**
   * @param target - load/save from source target
   */
  ImageFileSource(const QString &target = QString("Imported"));

  ImageFileSource(const QFileInfo &other);

  ImageFileSource(const ImageFileSource &other);

  ImageFileSource &operator=(const ImageFileSource &other);

  /**
   * @brief all required parameters set and valid?
   */
  bool isValidSource() const;

  /**
   * @brief is it original Source or a modified version exists?
   */
  bool isOriginal();

  /**
   * @brief set Article Number to current Source
   */
  void setFileId(qint64 articleId);

  /**
   * @brief get Article Id from current Source
   */
  qint64 getFileId() const;

  /**
   * @brief Convert to file basename with Article Id
   * @param articleId - Artikel Number
   * @param zerofill - Amount of prepended 0 Digits (file system sorting).
   */
  static const QString toBaseName(qint64 articleId, qint8 zerofill = 8);

  /**
   * @brief set image destination
   */
  void setTempTarget(const QDir &dest);

  /**
   * @brief get image destination
   */
  const QString getTempTarget() const;

  /**
   * @brief Insert or modify cached pixmap
   */
  void setCachedPixmap(const QPixmap &);

  /**
   * @brief Current cached pixmap
   */
  const QPixmap getCachedPixmap();

  /**
   * @brief Remove cached pixmap
   */
  void removeCachedPixmap();

  /**
   * @brief Search - Get/set Thumbnail Image from Database!
   * @param db        - AntiquaCRM::ASqlCore handle
   * @param articleId - Article ID
   */
  bool findInDatabase(AntiquaCRM::ASqlCore *db, qint64 articleId);

  /**
   * @brief Store cached Pixmap in database
   * @param db        - AntiquaCRM::ASqlCore handle
   * @param max       - Restrict to Max Thumbnail size
   * @param articleId - Article ID
   */
  bool storeInDatabase(AntiquaCRM::ASqlCore *db, const QSize &max,
                       qint64 articleId);

  /**
   * @brief Remove existing Thumbnail from Database!
   * @param db        - AntiquaCRM::ASqlCore handle
   * @param articleId - Article ID
   */
  bool removeFromDatabase(AntiquaCRM::ASqlCore *db, qint64 articleId);
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_IMAGESFILEOURCE_H
