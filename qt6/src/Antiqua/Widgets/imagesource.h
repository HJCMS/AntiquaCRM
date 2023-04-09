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
#include <QDir>
#include <QFileInfo>
#include <QPixmap>
#include <QString>

namespace AntiquaCRM {

/**
 * @class ImageSource
 * @brief Image File information
 * AntiquaCRM using zero filled id numbers for Image Filenames.
 * This will provide a better view an sorting in System File Managers.
 * Additional, AntiquaCRM identify Source Images with Article Numbers.
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
  void setFileId(qint64 id);

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
   * @brief save image destination
   */
  const QString getTarget() const;

  /**
   * @brief insert modief pixmap
   */
  void setPixmap(const QPixmap &);

  /**
   * @brief get current pixmap
   */
  const QPixmap getPixmap();

  /**
   * @brief remove pixmap
   */
  void removePixmap();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_IMAGESOURCE_H
