// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagedialog.h"
#include "applsettings.h"
#include "imageview.h"
#include "myicontheme.h"

#ifndef DEBUG_IMAGE_DIALOG
#define DEBUG_IMAGE_DIALOG true
#endif

#if DEBUG_IMAGE_DIALOG
#include <QDebug>
#endif
#include <QDialogButtonBox>
#include <QDirIterator>
#include <QFileInfo>
#include <QSplitter>
#include <QVBoxLayout>

FileBrowser::FileBrowser(QWidget *parent) : QFileDialog{parent} {
  setObjectName("file_dialog_widget");
  setWindowFlags(Qt::Widget);
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
  setMinimumSize(QSize(600, 400));
  setSizeGripEnabled(true);

  config = new ApplSettings(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setObjectName("image_open_edit_layout");
  layout->setContentsMargins(0, 0, 0, 0);

  QSize mSize = config->value("image/max_size", QSize(320, 320)).toSize();
  QSplitter *m_splitter = new QSplitter(this);
  m_view = new ImageView(mSize, m_splitter);
  m_splitter->insertWidget(0, m_view);
  m_splitter->setStretchFactor(0, 20);

  browser = new FileBrowser(m_splitter);
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
          SLOT(imagePreview(const QString &)));
  connect(browser, SIGNAL(s_close()), this, SLOT(reject()));
}

bool ImageDialog::findSourceImage() {
  if (!imagesArchiv.exists()) {
    notifyStatus(tr("archiv isn't readable!"));
    return false;
  }

  QString imageId = QString::number(p_articleId);

  QString basePath = browser->directory().path();
  QStringList search;
  search << imageId + ".JPG";
  search << imageId + ".jpg";
  search << imageId + ".JPEG";
  search << imageId + ".jpeg";

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

void ImageDialog::save() {
  if (m_view->getImage().isNull()) {
    notifyStatus(tr("no valid image found"));
    reject();
    return;
  }
  if (m_view->storeInDatabase(p_articleId))
    notifyStatus(tr("image saved successfully!"));
}

void ImageDialog::imagePreview(const QString &file) {
  QFileInfo info(file);
  if (info.isReadable())
    m_view->setImageFile(info);
}

void ImageDialog::notifyStatus(const QString &str) {
  m_statusBar->showMessage(str, (10 * 1000));
}

int ImageDialog::exec() {
  QString key("dirs/images");
  if (config->contains(key)) {
    QString p = config->value(key).toString();
    browser->setDirectory(p);
    imagesArchiv = browser->directory();
  }

  if (!findSourceImage()) {
    if (m_view->readFromDatabase(p_articleId))
      notifyStatus(tr("image from database!"));
  }

  return QDialog::exec();
}
