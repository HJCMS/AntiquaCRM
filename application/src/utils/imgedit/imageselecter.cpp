// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imageselecter.h"

#include <ASettings>
#include <QDebug>
#include <QFileDialog>
#include <QFrame>
#include <QLayout>

ImageSelecter::ImageSelecter(QWidget *parent) : QWidget{parent} {
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(2, 0, 0, 0);

  QIcon openIcon("://icons/view_image.png");
  QIcon viewIcon("://icons/view_list.png");

  QFrame *m_pathFrame = new QFrame(this);
  QHBoxLayout *m_pathFrameLayout = new QHBoxLayout(m_pathFrame);
  m_pathFrameLayout->setContentsMargins(0, 0, 0, 0);
  m_dirPathEdit = new QLineEdit(m_pathFrame);
  m_pathFrameLayout->addWidget(m_dirPathEdit);
  m_fileDialogBtn = new QPushButton(m_pathFrame);
  m_fileDialogBtn->setText(tr("Image Library"));
  m_fileDialogBtn->setToolTip(tr("Setting Imagelibrary"));
  m_fileDialogBtn->setIcon(openIcon);
  m_pathFrameLayout->addWidget(m_fileDialogBtn);
  m_pathFrame->setLayout(m_pathFrameLayout);
  layout->addWidget(m_pathFrame);

  m_listView = new ImageListView(this);
  layout->addWidget(m_listView);

  m_toolbar = new QToolBar(this);
  m_toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  // Archivpfad festlegen
  ac_set_archive = m_toolbar->addAction(viewIcon, tr("Set Archive path"));
  // Kamerapfad festlegen
  ac_set_camera = m_toolbar->addAction(viewIcon, tr("Set Camera path"));
  // Bild importieren
  ac_set_image = m_toolbar->addAction(openIcon, tr("Import Image"));
  layout->addWidget(m_toolbar);
  setLayout(layout);

  connect(m_fileDialogBtn, SIGNAL(clicked()), SLOT(selectImageTarget()));
  connect(m_listView, SIGNAL(imageSelected(const SourceInfo &)),
          SIGNAL(sendSelection(const SourceInfo &)));
  connect(m_listView, SIGNAL(sendTargetChanged(const QString &)), m_dirPathEdit,
          SLOT(setText(const QString &)));
  connect(ac_set_archive, SIGNAL(triggered()), SLOT(setDefaultTarget()));
  connect(ac_set_camera, SIGNAL(triggered()), SLOT(setImportTarget()));
  connect(ac_set_image, SIGNAL(triggered()), SLOT(selectSingleImage()));
}

void ImageSelecter::setDefaultTarget() {
  AntiquaCRM::ASettings cfg(this);
  QDir d(cfg.value("dirs/images").toString());
  if (d.isReadable())
    setDirectory(d.path());
}

void ImageSelecter::setImportTarget() {
  AntiquaCRM::ASettings cfg(this);
  QDir d(cfg.value("dirs/import").toString());
  if (d.isReadable())
    setDirectory(d.path());
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

void ImageSelecter::selectSingleImage() {
  QFileDialog *d = new QFileDialog(this, tr("Open Image Library"));
  d->setFilter(QDir::AllDirs | QDir::Drives);
  d->setAcceptMode(QFileDialog::AcceptOpen);
  d->setFileMode(QFileDialog::ExistingFile);
  d->setDirectory(p_dir.path());
#ifdef Q_OS_WIN
  d->setSidebarUrls(sideBarUrls());
#endif
  int ret = d->exec();
  if (ret == QDialog::Accepted) {
    QStringList l = d->selectedFiles();
    if (l.size() < 1)
      return;

    QFileInfo info(p_dir.path());
    info.setFile(l.first());
    if (info.isFile() && info.isReadable()) {
      emit sendImportImage(info.filePath());
    }
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
