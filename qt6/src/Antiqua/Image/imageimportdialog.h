// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_IMAGE_IMPORTDIALOG_H
#define ANTIQUACRM_IMAGE_IMPORTDIALOG_H

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

class Splitter;
class ImageFileSource;
class ImageViewer;
class ImageViewToolBar;
class ImageTreePathView;
class ImageTreeView;

/**
 * @class ImageImportDialog
 * @brief The ImageImportDialog class
 * @ingroup ImageWidgets
 */
class ANTIQUACRM_LIBRARY ImageImportDialog final : public QDialog {
  Q_OBJECT

private:
  bool p_sourceOnload = false;
  int p_article_id;         /**< @brief Article Id */
  const QString p_category; /**< @brief where to save new idFile */
  ASettings *config;        /**< @brief Configreader */

  /**
   * @brief Primary store directory for Images
   * Set by Configuration parameter „dirs/images“
   */
  QDir p_target;

  /**
   * @brief Absolute Path to current Image store Directory.
   * This Directory must inside from „p_target“ and will set by „p_category“,
   * if it not exists, this Subdirectory will created automatically.
   */
  QDir p_destination;

  /**
   * @brief Image import Directory
   * Set by Configuration parameter „dirs/import“
   */
  QDir p_import;

  /**
   * @brief current Image file source
   */
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
  void initialConfiguration();

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

  /**
   * @brief find StatusTip events
   */
  bool event(QEvent *) override;

  /**
   * @brief find QEvent::ModifiedChange in GraphicsView
   */
  bool eventFilter(QObject *, QEvent *) override;

  /**
   * @brief set image from imageSelected
   */
  void setViewerImage(const QString &path);

Q_SIGNALS:
  /**
   * @brief send Thumbnail after Database import success
   */
  void sendThumbnail(const QPixmap &);

public Q_SLOTS:
  /**
   * @brief A File selected, in the FileSystemModel.
   */
  void imageSelected(const QFileInfo &);

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

#endif // ANTIQUACRM_IMAGE_IMPORTDIALOG_H
