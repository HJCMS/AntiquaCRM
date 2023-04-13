// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_IMAGEIMPORTDIALOG_H
#define ANTIQUACRM_WIDGETS_IMAGEIMPORTDIALOG_H

#include <ASettings>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDir>
#include <QEvent>
#include <QLayout>
#include <QObject>
#include <QStatusBar>
#include <QWidget>

namespace AntiquaCRM {

class ImageFileSource;
class Splitter;
class ImageViewer;
class ImageViewToolBar;
class ImageTreePathView;
class ImageTreeView;

/**
 * @class ImageImportDialog
 * @brief The ImageImportDialog class
 * @ingroup AntiquaWidgets
 */
class ANTIQUACRM_LIBRARY ImageImportDialog final : public QDialog {
  Q_OBJECT

private:
  int p_article_id;   /**< @brief Article Id */
  QString p_category; /**< @brief where to save new idFile */
  ASettings *config;  /**< @brief Configreader */

  /**
   * @brief Primary store directory for Images
   * Set by Configuration parameter „dirs/images“
   */
  QDir p_target;

  /**
   * @brief Absolute Path to current Image store Directory.
   * This Directory must inside from „p_target“ and will set by  „p_category“,
   * if it not exists, this Subdirectory will created automatically.
   */
  QDir p_store;

  /**
   * @brief Image import Directory
   * Set by Configuration parameter „dirs/import“
   */
  QDir p_import;

  AntiquaCRM::ImageFileSource *source;

  /**
   * @brief Layout
   * @{
   */
  QBoxLayout *layout;
  Splitter *splitter;
  ImageViewer *viewer;
  ImageTreePathView *pathView;
  ImageTreeView *treeView;
  ImageViewToolBar *toolBar;
  QDialogButtonBox *buttonBox;
  QStatusBar *statusBar;
  /**
   * @}
   */

  /**
   * @brief read all targets from configuration
   */
  bool initialConfiguration();

  /**
   * @brief find previous Source Image with Id in Storage
   * @param inDir - Search in Directory
   * @param id    - Article ID
   */
  ImageFileSource *findSource(QDir inDir, qint64 id);

  /**
   * @brief prevent Window close icon
   */
  void closeEvent(QCloseEvent *) override;

  void setViewerImage(const QString &path);

Q_SIGNALS:
  /**
   * @brief send Thumbnail after Database import success
   */
  void sendThumbnail(const QPixmap &);

public Q_SLOTS:
  /**
   * @brief A File selected, in the FileSystemModel.
   * This Slot do 1 \b ImageViewer operation:
   * @li Insert Image into current Graphicsscene
   * It makes also 3 \b ImageFileSource operations:
   * @li Update with „source->setStoreDirectory“ the store target, this prevents
   * override Images from existing Source location.
   * @li set Filepath to Current image.
   * @li set/replace Pixmap Cache and Thumbnail
   */
  void imageSelected(const AntiquaCRM::ImageFileSource &);

  /**
   * @brief Create/Execute SQL-Queries and File-Save operations.
   */
  void aboutToSave();

  /**
   * @brief Show for unsaved changes and quitting the dialog.
   */
  void aboutToQuit();

public:
  /**
   * @param articleId - Required Article Id for image import
   * @param category  - Store this image in SubCategory
   * @param parent    - parent object
   */
  explicit ImageImportDialog(int articleId, const QString &category,
                             QWidget *parent = nullptr);

  /**
   * @brief cache clean operation
   */
  ~ImageImportDialog();

  /**
   * @brief current Image file source
   */
  ImageFileSource *currentSource();

  /**
   * @brief open Dialog with some predefined settings
   */
  int exec() override;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_IMAGEIMPORTDIALOG_H
