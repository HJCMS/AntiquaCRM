// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imageselecter.h"

#include <QDebug>
#include <QFileDialog>
#include <QFrame>
#include <QLayout>

ImageSelecter::ImageSelecter(QWidget *parent) : QWidget{parent} {
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(2, 0, 0, 0);

  m_listView = new ImageListView(this);
  layout->addWidget(m_listView);

  QFrame *m_pathFrame = new QFrame(this);
  QHBoxLayout *m_pathFrameLayout = new QHBoxLayout(m_pathFrame);
  m_dirPathEdit = new QLineEdit(m_pathFrame);
  m_pathFrameLayout->addWidget(m_dirPathEdit);

  m_fileDialogBtn = new QPushButton(m_pathFrame);
  m_fileDialogBtn->setText(tr("Image Library"));
  m_fileDialogBtn->setToolTip(tr("Setting Imagelibrary"));
  m_fileDialogBtn->setIcon(QIcon(":icons/view_image.png"));
  m_pathFrameLayout->addWidget(m_fileDialogBtn);

  m_pathFrame->setLayout(m_pathFrameLayout);
  layout->addWidget(m_pathFrame);

  setLayout(layout);

  connect(m_listView, SIGNAL(imageSelected(const SourceInfo &)),
          SIGNAL(sendSelection(const SourceInfo &)));

  connect(m_listView, SIGNAL(sendTargetChanged(const QString &)), m_dirPathEdit,
          SLOT(setText(const QString &)));

  connect(m_fileDialogBtn, SIGNAL(clicked()), SLOT(selectImageTarget()));
}

void ImageSelecter::selectImageTarget() {
  QFileDialog *d = new QFileDialog(this, tr("Open Image Library"));
  d->setFilter(QDir::AllDirs | QDir::Drives);
  d->setAcceptMode(QFileDialog::AcceptOpen);
  d->setFileMode(QFileDialog::Directory);
  d->setDirectory(p_dir.path());
#ifdef Q_OS_WIN
  d->setSidebarUrls(sideBarUrls());
#endif
  int ret = d->exec();
  if (ret == QDialog::Accepted) {
    QStringList l = d->selectedFiles();
    if (l.size() < 1)
      return;

    QString p = l.first();
    setDirectory(p);
  }
}

void ImageSelecter::setDirectory(const QString &dirPath) {
  if (dirPath.isEmpty())
    return;

  QFileInfo info(dirPath);
  if (info.isDir() && info.isReadable()) {
    p_dir.setPath(dirPath);
    m_listView->setDirectory(p_dir);
    emit sendTargetChanged(p_dir);
  }
}

void ImageSelecter::setSelection(const SourceInfo &src) {
  if (!src.isValidSource())
    return;

  m_listView->setSourceImage(src);
}

#ifdef Q_OS_WIN
const QList<QUrl> ImageSelecter::sideBarUrls() {
  QList<QUrl> list;
  foreach (QFileInfo i, QDir::drives()) {
    QUrl url;
    url.setPath(i.filePath());
    if (url.isValid())
      list.append(url);
  }
  return list;
}
#endif

const QDir ImageSelecter::directory() { return p_dir; }
