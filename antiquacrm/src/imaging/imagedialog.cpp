// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagedialog.h"
#include "filedialog.h"
#include "imgedit.h"
#include "imgviewer.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QDirIterator>
#include <QtCore/QFileInfo>
#include <QtGui/QImageReader>
#include <QtGui/QPixmap>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

ImageDialog::ImageDialog(qulonglong id, QWidget *parent)
    : QDialog{parent}, imageID(id) {
  setObjectName("ImageDialog");
  setWindowTitle(tr("Picture Editor"));
  setWindowIcon(myIcon("image"));
  setSizeGripEnabled(true);
  setMinimumSize(QSize(330, 550));

  QVBoxLayout *m_vLayout = new QVBoxLayout(this);

  m_scrollArea = new QScrollArea(this);
  m_scrollArea->setObjectName("dialog_scrollarea");
  m_scrollArea->setVisible(true);
  m_scrollArea->setWidgetResizable(false);
  m_scrollArea->setBackgroundRole(QPalette::Dark);
  m_vLayout->addWidget(m_scrollArea);

  QToolBar *toolBar = new QToolBar(this);
  m_vLayout->addWidget(toolBar);

  QToolButton *m_rotateRight = new QToolButton(toolBar);
  m_rotateRight->setText(tr("Rotate"));
  toolBar->addWidget(m_rotateRight);

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

  connect(m_rotateRight, SIGNAL(clicked()), this, SLOT(rotateRightClicked()));
  connect(btn_openimg, SIGNAL(clicked()), this, SLOT(openFileDialog()));
  connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void ImageDialog::rotateImage(qreal r) {
  QImage img(m_imgViewer->image());
  QTransform transform;
  transform.rotate(r);
  QImage out = img.transformed(transform, Qt::SmoothTransformation);
  if (!out.isNull()) {
    m_imgViewer->setPixmap(QPixmap::fromImage(out));
    m_imgViewer->update();
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
  QImageReader reader(file.absoluteFilePath());
  reader.setAutoTransform(true);
  qDebug() << reader.format();
  QImage image = reader.read();
  if (!image.isNull()) {
    QImage medium = image.scaled(QSize(maximumSize, maximumSize),
                                 Qt::KeepAspectRatio, Qt::SmoothTransformation);
    if (!medium.isNull()) {
      m_imgViewer = new ImgViewer(m_scrollArea);
      m_imgViewer->setPixmap(QPixmap::fromImage(medium));
      m_scrollArea->setWidget(m_imgViewer);
      setSizeMessage(medium.size());
      emit s_imageScaled(true);
      return;
    }
    emit s_imageScaled(false);
  }
}

void ImageDialog::findImageSourceFiles() {
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
    qWarning("No image were found");
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
  m_fileDialog = new FileDialog(this);
  m_fileDialog->setObjectName("imgfile_dialog");
  m_fileDialog->setImageFilter();
  if (m_fileDialog->exec()) {
    QStringList list = m_fileDialog->selectedFiles();
    loadFile(list.first());
  }
}

const QImage ImageDialog::getImage() { return m_imgViewer->image(); }

bool ImageDialog::setSourceTarget(const QString &path) {
  sourceDir.setPath(path);
  if (sourceDir.exists()) {
    findImageSourceFiles();
    return true;
  }
  return false;
}

const QString ImageDialog::sourceTarget() { return sourceDir.absolutePath(); }
