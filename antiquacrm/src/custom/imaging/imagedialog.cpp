// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagedialog.h"
#include "applsettings.h"
#include "imageview.h"
#include "myicontheme.h"

#ifndef DEBUG_IMAGE_DIALOG
#define DEBUG_IMAGE_DIALOG false
#endif

#if DEBUG_IMAGE_DIALOG
#include <QDebug>
#endif
#include <QFileInfo>
#include <QDialogButtonBox>
#include <QDirIterator>
#include <QSplitter>
#include <QVBoxLayout>

ImageDialog::ImageDialog(int articleId, QWidget *parent)
    : QFileDialog{parent}, p_articleId{articleId} {
  setObjectName("image_open_edit_dialog");
  setWindowTitle(tr("Picture Editor"));
  setWindowIcon(myIcon("image"));
  setSizeGripEnabled(true);
  setMinimumSize(QSize(600, 400));
  setNameFilter(tr("Image jpeg files (*.jpg *.JPG *.jpeg *.JPEG)"));
  setLabelText(QFileDialog::Accept, tr("Save Image"));

  config = new ApplSettings(this);

  QSize maxImageSize =
      config->value("image/max_size", QSize(320, 320)).toSize();

  QSplitter *splitter = findChild<QSplitter *>();
  if (splitter != nullptr) {
    QWidget *previewArea = new QWidget(splitter);
    QVBoxLayout *paLayout = new QVBoxLayout(previewArea);
    paLayout->setContentsMargins(0, 0, 0, 0);
    m_view = new ImageView(maxImageSize, previewArea);
    paLayout->addWidget(m_view);
    m_toolBar = new QToolBar(previewArea);
    m_toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    ac_scale = m_toolBar->addAction(myIcon("scale"),"Scale");
    m_toolBar->addSeparator();
    ac_rotate = m_toolBar->addAction(tr("rotate image clockwise"));
    ac_rotate->setIcon(myIcon("rotate-right"));
    paLayout->addWidget(m_toolBar);
    previewArea->setLayout(paLayout);
    splitter->addWidget(previewArea);
    connect(ac_rotate, SIGNAL(triggered()), m_view, SLOT(rotate()));
    connect(ac_scale, SIGNAL(triggered()), m_view, SLOT(zoomReset()));
  }

  QDialogButtonBox *btnBox = findChild<QDialogButtonBox *>();
  if (btnBox != nullptr)
    connect(btnBox, SIGNAL(accepted()), this, SLOT(save()));

  connect(this, SIGNAL(currentChanged(const QString &)), this,
          SLOT(imagePreview(const QString &)));
}

bool ImageDialog::findSourceImage() {
  if (!imagesArchiv.exists())
    return false;

  QString imageId = QString::number(p_articleId);

  QString basePath = directory().path();
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

  if(found.size() < 1)
    return false;

  QString imageFile = found.first();
  QFileInfo info(basePath, imageFile);
  if(info.exists()) {
    setDirectory(info.path());
    selectFile(info.filePath());
    m_view->setImageFile(info);
    return true;
  }
  return false;
}

void ImageDialog::accept() {
  /**
   * Ich weiss ... :-(
   * Das hier ist nicht gerade die feine Art.
   * Ich bin für Vorschläge und Tips offen.
   */
}

void ImageDialog::save() {
  if (m_view->getImage().isNull()) {
    reject();
    return;
  }
  // Bild Speichern und beenden
  if (m_view->storeInDatabase(p_articleId))
    done(QDialogButtonBox::Ok);
}

void ImageDialog::imagePreview(const QString &file) {
  QFileInfo info(file);
  if (info.isReadable())
    m_view->setImageFile(info);
}

int ImageDialog::exec() {
  QString key("dirs/images");
  if (config->contains(key)) {
    QString p = config->value(key).toString();
    setDirectory(p);
    imagesArchiv = directory();
  }

  if (!findSourceImage()) {
    m_view->readFromDatabase(p_articleId);
  }

  return QFileDialog::exec();
}
