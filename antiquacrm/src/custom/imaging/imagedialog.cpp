// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagedialog.h"
#include "applsettings.h"
#include "imageview.h"
#include "myicontheme.h"

#include <QDebug>
#include <QDialogButtonBox>
#include <QDirIterator>
#include <QSplitter>
#include <QVBoxLayout>

ImageDialog::ImageDialog(int articleId, QWidget *parent)
    : QFileDialog{parent}, p_articleId{articleId} {
  setObjectName("ImageDialog");
  setWindowTitle(tr("Picture Editor"));
  setWindowIcon(myIcon("image"));
  setSizeGripEnabled(true);
  setMinimumSize(QSize(600, 400));
  setNameFilter(tr("Image jpeg files (*.jpg *.JPG *.jpeg *.JPEG)"));
  setLabelText(QFileDialog::Accept, tr("Save Image"));

  config = new ApplSettings(this);

  QSplitter *splitter = findChild<QSplitter *>();
  if (splitter != nullptr) {
    QWidget *previewArea = new QWidget(splitter);
    QVBoxLayout *paLayout = new QVBoxLayout(previewArea);
    paLayout->setContentsMargins(0, 0, 0, 0);
    m_view = new ImageView(previewArea);
    paLayout->addWidget(m_view);
    m_toolBar = new QToolBar(previewArea);
    m_toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ac_rotate = m_toolBar->addAction(myIcon("redo"), /* drehen */
                                     tr("Rotate Image"));
    paLayout->addWidget(m_toolBar);
    previewArea->setLayout(paLayout);
    splitter->addWidget(previewArea);
    connect(ac_rotate, SIGNAL(triggered()), m_view, SLOT(rotate()));
  }

  QDialogButtonBox *btnBox = findChild<QDialogButtonBox *>();
  if (btnBox != nullptr)
    connect(btnBox, SIGNAL(accepted()), this, SLOT(save()));

  connect(this, SIGNAL(currentChanged(const QString &)), this,
          SLOT(imagePreview(const QString &)));
}

void ImageDialog::findSourceImage() {
  if (!imagesArchiv.exists())
    return;

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

  if (found.isEmpty()) {
    m_view->loadFromDatabase(p_articleId);
    return;
  }

  QString imageFile = found.first();
  QFileInfo info(basePath, imageFile);
  setDirectory(info.path());
  selectFile(info.filePath());
  m_view->setImageFile(info);
}

void ImageDialog::showEvent(QShowEvent *event) {
  QString key("imaging/sourcepath");
  if (config->contains(key)) {
    QString p = config->value(key).toString();
    setDirectory(p);
    imagesArchiv = directory();
    findSourceImage();
  }
  QFileDialog::enterEvent(event);
}

void ImageDialog::accept() { /* FIXME */ }

void ImageDialog::save() {
  if (m_view->getImage().isNull()) {
    reject();
    return;
  }
  // Bild Speichern und beenden
  if (m_view->storeImage(p_articleId))
    done(QDialogButtonBox::Ok);
}

void ImageDialog::imagePreview(const QString &file) {
  QFileInfo info(file);
  if (info.isReadable())
    m_view->setImageFile(info);
}
