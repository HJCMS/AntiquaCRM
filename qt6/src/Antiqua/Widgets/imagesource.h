// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_IMAGESOURCE_H
#define ANTIQUACRM_WIDGETS_IMAGESOURCE_H

#include <AntiquaCRM>
#include <QByteArray>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QPixmap>
#include <QSize>

namespace AntiquaCRM {

/**
 * @class ImageSource
 * @brief Image Source Info and SQL-Database operations.
 * AntiquaCRM using zero filled id numbers for Image Filenames.
 * This will provide a better view an sorting in System File Managers.
 * @note AntiquaCRM identify Source Images with Article Numbers.
 *
 * This class additionally supports SQL select/insert/remove operation for
 * thumbnails.
 *
 * @code
 *   qint64 _articleId = 123456;
 *   QString fileBasename = DatabaseImage::toBaseName(_articleId);
 *   fileBasename.append(".jpg");
 *   // 00123456.jpg
 * @endcode
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY ImageSource final : public QFileInfo {
private:
  /**
   * @brief Artikel number
   * Needed later for saving!
   */
  qint64 p_fileId;

  /**
   * @brief Where to save the image!
   */
  QString p_target;

  /**
   * @brief cached Pixmap
   * If the Image is not equal to Source, a copy stored here.
   */
  QPixmap p_pixmap;

  /**
   * @brief Import Thumbnail data from Database
   * @param data - rwa data from database
   */
  bool loadDatabaseImage(const QByteArray &data);

public:
  /**
   * @param target - load/save from source target
   */
  ImageSource(const QString &target = QString());

  /**
   * @param other - get data  from QFileInfo
   */
  ImageSource(const QFileInfo &other);

  ImageSource(const ImageSource &other);

  ImageSource &operator=(const ImageSource &other);

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
  void setTarget(const QDir &dest);

  /**
   * @brief get image destination
   */
  const QString getTarget() const;

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

#endif // ANTIQUACRM_WIDGETS_IMAGESOURCE_H
