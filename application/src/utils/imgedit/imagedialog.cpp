// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagedialog.h"
#include "imageselecter.h"
#include "imageview.h"
#include "sourceinfo.h"

#ifndef DEBUG_IMAGE_DIALOG
#define DEBUG_IMAGE_DIALOG true
#endif

#include <QDebug>
#include <QDialogButtonBox>
#include <QDirIterator>
#include <QFileInfo>
#include <QIcon>
#include <QMessageBox>
#include <QStandardPaths>
#include <QVBoxLayout>

ImageDialog::ImageDialog(int articleId, QWidget *parent)
    : QDialog{parent}, p_articleId{articleId} {
  setObjectName("image_open_edit_dialog");
  setWindowTitle(tr("Picture Editor"));
  setMinimumSize(QSize(800, 400));
  setSizeGripEnabled(true);

  config = new AntiquaCRM::ASettings(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("image_open_edit_layout");
  layout->setContentsMargins(10, 0, 0, 0);

  // Left (Image Preview)
  QSize mSize = config->value("image/max_size", QSize(320, 320)).toSize();
  m_splitter = new QSplitter(this);
  m_splitter->setObjectName("image_open_edit_splitter");
  m_view = new ImageView(mSize, m_splitter);
  m_view->setObjectName("image_open_edit_preview");
  m_splitter->insertWidget(0, m_view);
  m_splitter->setStretchFactor(0, 40);

  // Right (Image Selecter)
  m_imageSelecter = new ImageSelecter(m_splitter);
  m_imageSelecter->setObjectName("image_folder_view");
  m_splitter->insertWidget(1, m_imageSelecter);
  m_splitter->setStretchFactor(1, 60);

  // add splitter
  layout->insertWidget(0, m_splitter);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setObjectName("image_open_edit_statusbar");
  m_toolBar = new QToolBar(m_statusBar);
  ac_scale = m_toolBar->addAction(QIcon(":icons/view_scale.png"), tr("Scale"));
  m_toolBar->addSeparator();
  ac_rotate = m_toolBar->addAction(tr("Rotate"));
  ac_rotate->setIcon(QIcon(":icons/view_rotate_right.png"));
  m_toolBar->addSeparator();
  m_toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  ac_save = m_toolBar->addAction(QIcon(":icons/action_save.png"), tr("Save"));
  m_toolBar->addSeparator();
  ac_close =
      m_toolBar->addAction(QIcon(":icons/action_quit.png"), tr("Finish"));
  m_statusBar->addPermanentWidget(m_toolBar);

  layout->insertWidget(1, m_statusBar);
  layout->setStretch(0, 1);
  setLayout(layout);

  connect(ac_rotate, SIGNAL(triggered()), m_view, SLOT(rotate()));
  connect(ac_scale, SIGNAL(triggered()), m_view, SLOT(zoomReset()));
  connect(ac_save, SIGNAL(triggered()), SLOT(save()));
  connect(ac_close, SIGNAL(triggered()), SLOT(accept()));
  connect(m_imageSelecter, SIGNAL(sendSelection(const SourceInfo &)),
          SLOT(fileChanged(const SourceInfo &)));
  connect(m_imageSelecter, SIGNAL(sendTargetChanged(const QDir &)),
          SLOT(setHistoryDir(const QDir &)));
}

bool ImageDialog::findSourceImage() {
  if (!imagesArchiv.exists()) {
    notifyStatus(tr("archiv isn't readable!"));
    return false;
  }

  QString imageId = QString::number(p_articleId);
  QString fullImageId = SourceInfo::imageBaseName(p_articleId);

  QString basePath = m_imageSelecter->directory().path();
  QStringList search;
  search << imageId + ".JPG";
  search << imageId + ".jpg";
  search << fullImageId + ".JPG";
  search << fullImageId + ".jpg";

  QStringList found;
  QDirIterator it(basePath, search, QDir::NoFilter,
                  QDirIterator::Subdirectories);
  while (it.hasNext()) {
    QFileInfo f(it.next());
    QString file(f.absoluteFilePath());
    basePath.append(QDir::separator());
    found << file.replace(basePath, "");
  }

  if (found.size() < 1)
    return false;

  QString imageFile = found.first();
  QFileInfo info(basePath, imageFile);
  if (info.exists()) {
    SourceInfo src(info);
    src.setFileId(p_articleId);
    m_imageSelecter->setSelection(src);
    m_view->setImageFile(info);
    return true;
  }
  notifyStatus(tr("no source image exists!"));
  return false;
}

bool ImageDialog::isImageFromArchive(const SourceInfo &info) {
  return info.path().startsWith(imagesArchiv.path());
}

bool ImageDialog::askToCopyFile() {
  QString question = tr("<p>Do you want to copy this Image into the Picture "
                        "Archiv?</p><b>Note:</b> This will replace Images "
                        "with identical Article Filename!");
  QString title(tr("Copy image?"));
  int ret = QMessageBox::question(this, title, question);
  if (ret == QMessageBox::Yes) {
    return true;
  }
  return false;
}

bool ImageDialog::imagePreview(const SourceInfo &image) {
  SourceInfo info(image);
  info.setFileId(p_articleId);
  info.setTarget(imagesArchiv);
  if (info.isValidSource()) {
    m_view->setImageFile(info);
    return true;
  }
  return false;
}

void ImageDialog::setHistoryDir(const QDir &d) {
  if (d.isReadable())
    config->setValue("history_image_target", d.path());
}

void ImageDialog::save() {
  if (m_view->getImage().isNull()) {
    notifyStatus(tr("no valid image found"));
    return;
  }

  SourceInfo info(m_view->getSource());
  if (info.isValidSource() && !isImageFromArchive(info)) {
    qInfo("image about to copy");
    if (askToCopyFile()) {
      notifyStatus(tr("copy image in progress ..."));
      if (m_view->saveImageTo(info)) {
        notifyStatus(tr("successfully - image to archive copied"));
      } else {
        notifyStatus(tr("warning - image not copied"));
        qWarning("image copy failed");
      }
    }
  }
#ifdef ANTIQUA_DEVELOPEMENT
  else {
    qDebug() << Q_FUNC_INFO << "NO COPY" << info.getFileId() << info;
  }
#endif
  // In Datenbank Speichern!
  if (m_view->storeInDatabase(p_articleId))
    notifyStatus(tr("Image saved successfully!"));
}

void ImageDialog::fileChanged(const SourceInfo &image) {
  if (!imagePreview(image))
    notifyStatus(tr("Image preview rejected!"));
}

void ImageDialog::closeEvent(QCloseEvent *e) {
  if (e->type() == QEvent::Close) {
    e->setAccepted(false);
    notifyStatus(tr("Please use the Finish button!"));
    return;
  }
  QDialog::closeEvent(e);
}

void ImageDialog::resizeEvent(QResizeEvent *e) {
  if (e->type() == QEvent::Resize) {
    config->beginGroup("imaging");
    config->setValue("geometry", saveGeometry());
    config->setValue("windowState", m_splitter->saveState());
    config->endGroup();
  }
  QDialog::resizeEvent(e);
}

void ImageDialog::notifyStatus(const QString &str) {
  m_statusBar->showMessage(str, (10 * 1000));
}

int ImageDialog::exec() {
  QString fallback =
      QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
  QString archivPath = config->value("dirs/images", fallback).toString();
  imagesArchiv = QDir(archivPath);

  QString history = config->value("history_image_target", archivPath).toString();
  m_imageSelecter->setDirectory(history);

  if (config->contains("imaging/geometry")) {
    config->beginGroup("imaging");
    restoreGeometry(config->value("geometry").toByteArray());
    if (config->contains("windowState"))
      m_splitter->restoreState(config->value("windowState").toByteArray());

    config->endGroup();
  }

  if (!findSourceImage()) {
    if (m_view->readFromDatabase(p_articleId))
      notifyStatus(tr("image from database!"));
  }

  return QDialog::exec();
}
