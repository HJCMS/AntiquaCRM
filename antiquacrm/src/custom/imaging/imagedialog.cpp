// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagedialog.h"
#include "imageview.h"
#include "myicontheme.h"
#include "openimagedialog.h"
#include <AntiquaCRM>

#include <QDebug>
#include <QDialogButtonBox>
#include <QDirIterator>
#include <QFileInfo>
#include <QImageReader>
#include <QPixmap>
#include <QPushButton>
#include <QToolBar>
#include <QToolButton>
#include <QVBoxLayout>

ImageDialog::ImageDialog(qulonglong id, QWidget *parent)
    : QDialog{parent}, imageID(id) {
  setObjectName("ImageDialog");
  setWindowTitle(tr("Picture Editor"));
  setWindowIcon(myIcon("image"));
  setSizeGripEnabled(true);
  setMinimumSize(QSize(330, 550));

  QVBoxLayout *m_vLayout = new QVBoxLayout(this);

  m_imgView = new ImageView(this);
  m_vLayout->addWidget(m_imgView);

  QToolBar *toolBar = new QToolBar(this);
  m_vLayout->addWidget(toolBar);

  QPushButton *btn_rotate = new QPushButton(toolBar);
  btn_rotate->setText(tr("Rotate Image"));
  btn_rotate->setIcon(myIcon("redo"));
  toolBar->addWidget(btn_rotate);

  QDialogButtonBox *m_buttonBox = new QDialogButtonBox(Qt::Horizontal, this);
  m_buttonBox->setShortcutEnabled(false);
  m_vLayout->addWidget(m_buttonBox);

  QPushButton *btn_openimg =
      m_buttonBox->addButton(tr("Open Image"), QDialogButtonBox::ActionRole);
  btn_openimg->setObjectName("open_file_dialog");
  btn_openimg->setIcon(myIcon("image"));
  btn_openimg->setShortcut(QKeySequence::Open);

  QPushButton *m_commit =
      m_buttonBox->addButton(tr("Save"), QDialogButtonBox::AcceptRole);
  m_commit->setObjectName("dialog_commit_button");
  m_commit->setToolTip(tr("Close the dialog with saving."));
  m_commit->setIcon(myIcon("filesave"));
  m_commit->setEnabled(false);
  m_commit->setShortcut(QKeySequence::Save);

  QPushButton *m_close =
      m_buttonBox->addButton(tr("Close"), QDialogButtonBox::RejectRole);
  m_close->setObjectName("dialog_close_button");
  m_close->setToolTip(tr("Close the dialog without saving."));
  m_close->setIcon(myIcon("exit"));
  m_close->setShortcut(QKeySequence::Close);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setObjectName("dialog_statusbar");
  m_statusBar->setSizeGripEnabled(false);
  m_vLayout->addWidget(m_statusBar);

  connect(btn_rotate, SIGNAL(clicked()), this, SLOT(rotateRightClicked()));
  connect(btn_openimg, SIGNAL(clicked()), this, SLOT(openFileDialog()));
  connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  connect(this, SIGNAL(s_imageLoaded(bool)), m_commit, SLOT(setEnabled(bool)));

  setSourceTarget();
}

void ImageDialog::rotateImage(qreal r) {
  QImage img(m_imgView->image());
  if (img.isNull())
    return;

  QTransform transform;
  transform.rotate(r);
  QImage out = img.transformed(transform, Qt::SmoothTransformation);
  if (!out.isNull()) {
    m_imgView->setImage(out);
    m_imgView->update();
  }
}

void ImageDialog::setSourceTarget() {
  ApplSettings cfg;
  sourceDir.setPath(cfg.value("dirs/images").toString());
  if (sourceDir.exists()) {
    findImageSourceFiles();
  }
}

void ImageDialog::setSizeMessage(const QSize &s) {
  QString strsize(" ");
  strsize.append(QString::number(s.width()));
  strsize.append("x");
  strsize.append(QString::number(s.height()));
  m_statusBar->showMessage(tr("Current Image Size") + strsize);
}

void ImageDialog::loadFile(const QFileInfo &file) {
  if (!file.exists())
    return;

  QImageReader reader(file.absoluteFilePath());
  reader.setAutoTransform(true);
  QImage image = reader.read();
  if (!image.isNull()) {
    emit s_imageLoaded(true);
    QImage medium = image.scaled(QSize(maximumSize, maximumSize),
                                 Qt::KeepAspectRatio, Qt::SmoothTransformation);
    if (!medium.isNull()) {
      m_imgView->setImage(medium);
      setSizeMessage(medium.size());
      emit s_imageScaled(true);
      return;
    }
    emit s_imageScaled(false);
  }
  emit s_imageLoaded(false);
}

void ImageDialog::findImageSourceFiles() {
  if (!sourceDir.exists())
    return;

  QString basePath = sourceDir.path();
  QStringList search;
  search << QString::number(imageID) + ".JPG";
  search << "P" + QString::number(imageID) + ".JPG";
  search << QString::number(imageID) + ".jpg";
  search << QString::number(imageID) + ".jpeg";
  QDirIterator it(basePath, search, QDir::NoFilter,
                  QDirIterator::Subdirectories);

  search.clear();
  while (it.hasNext()) {
    QFileInfo f(it.next());
    QString file(f.absoluteFilePath());
    basePath.append(QDir::separator());
    search << file.replace(basePath, "");
  }

  if (search.isEmpty()) {
    m_statusBar->showMessage(tr("No Image for this Arcticle found."));
    qWarning("No image were found in archive");
    emit s_imageLoaded(false);
    return;
  }

  if (search.size()) {
    m_statusBar->showMessage(tr("Warning - More then one found!"));
  }

  QString image(search.first());
  QFileInfo info(sourceDir, image);
  if (info.exists()) {
    loadFile(info);
  }
}

void ImageDialog::openFileDialog() {
  ApplSettings cfg;
  m_fileDialog = new OpenImageDialog(this);
  m_fileDialog->setObjectName("imgfile_dialog");
  m_fileDialog->setStart(
      cfg.value("dirs/images", QDir::homePath()).toString());
  if (m_fileDialog->exec()) {
    loadFile(m_fileDialog->file());
  }
}

const QImage ImageDialog::getImage() { return m_imgView->image(); }

const QString ImageDialog::sourceTarget() { return sourceDir.absolutePath(); }
