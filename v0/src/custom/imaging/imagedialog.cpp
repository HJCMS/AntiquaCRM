// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagedialog.h"
#include "applsettings.h"
#include "imageview.h"
#include "myicontheme.h"
#include "sourceinfo.h"

#ifndef DEBUG_IMAGE_DIALOG
#define DEBUG_IMAGE_DIALOG true
#endif

#if DEBUG_IMAGE_DIALOG
#include <QDebug>
#endif
#include <QDialogButtonBox>
#include <QDirIterator>
#include <QFileInfo>
#include <QMessageBox>
#include <QVBoxLayout>

FileBrowser::FileBrowser(QWidget *parent) : QFileDialog{parent,Qt::SubWindow} {
  setObjectName("file_dialog_widget");
  setWindowFlags(Qt::SubWindow); // TODO Windows Testing
  setWindowModality(Qt::NonModal);
  setWindowState(Qt::WindowNoState);
  setSizeGripEnabled(false);
  setNameFilter(tr("Image jpeg files (*.jpg *.JPG *.jpeg *.JPEG)"));
}

void FileBrowser::accept() { /* don't touch me */
}

void FileBrowser::reject() { emit s_close(); }

ImageDialog::ImageDialog(int articleId, QWidget *parent)
    : QDialog{parent}, p_articleId{articleId} {
  setObjectName("image_open_edit_dialog");
  setWindowTitle(tr("Picture Editor"));
  setWindowIcon(myIcon("image"));
  setMinimumSize(QSize(800, 400));
  setSizeGripEnabled(true);

  config = new ApplSettings(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("image_open_edit_layout");
  layout->setContentsMargins(10, 0, 0, 0);

  QSize mSize = config->value("image/max_size", QSize(320, 320)).toSize();
  m_splitter = new QSplitter(this);
  m_splitter->setObjectName("image_open_edit_splitter");
  m_view = new ImageView(mSize, m_splitter);
  m_view->setObjectName("image_open_edit_preview");
  m_splitter->insertWidget(0, m_view);
  m_splitter->setStretchFactor(0, 20);

  browser = new FileBrowser(m_splitter);
  browser->setObjectName("image_open_edit_browser");
  m_splitter->insertWidget(1, browser);
  m_splitter->setStretchFactor(1, 70);
  layout->insertWidget(0, m_splitter);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setObjectName("image_open_edit_statusbar");
  m_toolBar = new QToolBar(m_statusBar);
  ac_scale = m_toolBar->addAction(myIcon("scale"), tr("Scale"));
  m_toolBar->addSeparator();
  ac_rotate = m_toolBar->addAction(tr("rotate clockwise"));
  ac_rotate->setIcon(myIcon("rotate-right"));
  m_toolBar->addSeparator();
  m_toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  ac_save = m_toolBar->addAction(myIcon("filesave"), tr("Save"));
  m_toolBar->addSeparator();
  ac_close = m_toolBar->addAction(myIcon("exit"), tr("Close"));
  m_statusBar->addPermanentWidget(m_toolBar);

  layout->insertWidget(1, m_statusBar);
  layout->setStretch(0, 1);
  setLayout(layout);

  connect(ac_rotate, SIGNAL(triggered()), m_view, SLOT(rotate()));
  connect(ac_scale, SIGNAL(triggered()), m_view, SLOT(zoomReset()));
  connect(ac_save, SIGNAL(triggered()), this, SLOT(save()));
  connect(ac_close, SIGNAL(triggered()), this, SLOT(accept()));
  connect(browser, SIGNAL(currentChanged(const QString &)), this,
          SLOT(fileChanged(const QString &)));
  connect(browser, SIGNAL(s_close()), this, SLOT(reject()));
}

bool ImageDialog::findSourceImage() {
  if (!imagesArchiv.exists()) {
    notifyStatus(tr("archiv isn't readable!"));
    return false;
  }

  QString imageId = QString::number(p_articleId);
  QString fullImageId = SourceInfo::imageBaseName(p_articleId);

  QString basePath = browser->directory().path();
  QStringList search;
  search << imageId + ".JPG";
  search << imageId + ".jpg";
  search << imageId + ".JPEG";
  search << imageId + ".jpeg";
  search << fullImageId + ".JPG";
  search << fullImageId + ".jpg";
  search << fullImageId + ".JPEG";
  search << fullImageId + ".jpeg";

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
    browser->setDirectory(info.path());
    browser->selectFile(info.filePath());
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

bool ImageDialog::imagePreview(const SourceInfo &info) {
  if (info.isFile() && info.isReadable()) {
    m_view->setImageFile(info);
    return true;
  }
  return false;
}

void ImageDialog::save() {
  if (m_view->getImage().isNull()) {
    notifyStatus(tr("no valid image found"));
    reject();
    return;
  }

  // Bild in das Quellenarchiv kopieren!
  QStringList files = browser->selectedFiles();
  QString filePath = files.last();
  if (!filePath.isEmpty()) {
    SourceInfo info(filePath);
    if (!isImageFromArchive(info)) {
      if (askToCopyFile()) {
        notifyStatus(tr("copy image in progress ..."));
        info.setFileId(p_articleId);
        info.setTarget(imagesArchiv);
        if (m_view->saveImageTo(info)) {
          notifyStatus(tr("successfully - image to archive copied"));
        } else {
          notifyStatus(tr("warning - image not copied"));
          qWarning("image copy failed");
        }
      }
    }
    filePath.clear();
  }

  // In Datenbank Speichern!
  if (m_view->storeInDatabase(p_articleId))
    notifyStatus(tr("image saved successfully!"));

  config->beginGroup("bookmarks");
  QListIterator<QUrl> it(browser->sidebarUrls());
  int i = 0;
  while (it.hasNext()) {
    config->setValue(QString::number(i++), it.next());
  }
  config->endGroup();
  // Aufräumen
  files.clear();
}

void ImageDialog::fileChanged(const QString &file) {
  SourceInfo info(file);
  if (!info.isReadable())
    return;

  imagePreview(info);
}

void ImageDialog::closeEvent(QCloseEvent *e) {
  config->beginGroup("imaging");
  config->setValue("geometry", saveGeometry());
  config->setValue("windowState", m_splitter->saveState());
  config->endGroup();
  QDialog::closeEvent(e);
}

void ImageDialog::notifyStatus(const QString &str) {
  m_statusBar->showMessage(str, (10 * 1000));
}

int ImageDialog::exec() {
  QString key("dirs/images");
  if (config->contains(key)) {
    QString p = config->value(key, QDir::homePath()).toString();
    browser->setDirectory(p);
    imagesArchiv = browser->directory();
  }

  if (config->contains("imaging/geometry")) {
    config->beginGroup("imaging");
    restoreGeometry(config->value("geometry").toByteArray());
    if (config->contains("windowState"))
      m_splitter->restoreState(config->value("windowState").toByteArray());

    config->endGroup();
  }

  config->beginGroup("bookmarks");
  QList<QUrl> bookmarks;
  foreach (QString key, config->allKeys()) {
    bookmarks.append(config->value(key).toUrl());
  }
  config->endGroup();
  if (bookmarks.count() > 1)
    browser->setSidebarUrls(bookmarks);

  if (!findSourceImage()) {
    if (m_view->readFromDatabase(p_articleId))
      notifyStatus(tr("image from database!"));
  }

  return QDialog::exec();
}
