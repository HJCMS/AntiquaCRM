// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagelistview.h"

#include <QDebug>

ImageListView::ImageListView(QWidget *parent) : QTreeView{parent} {
  setWordWrap(false);
  setAlternatingRowColors(true);
  setAnimated(false);
  setSortingEnabled(true);
  setItemsExpandable(true);
  setRootIsDecorated(false);
  setExpandsOnDoubleClick(true);

  p_directory = QDir::home();
  p_directory.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
  p_directory.setNameFilters(viewFilter());

  m_model = new QFileSystemModel(this);
  m_model->setObjectName("images_view_model");
  m_model->setReadOnly(true);
  m_model->setOption(QFileSystemModel::DontWatchForChanges, true);
  m_model->setOption(QFileSystemModel::DontResolveSymlinks, true);
  m_model->setFilter(p_directory.filter());
  setModel(m_model);

  m_headerView = header();
  m_headerView->resizeSections(QHeaderView::ResizeToContents);
  m_headerView->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  setHeader(m_headerView);

  connect(this, SIGNAL(expanded(const QModelIndex &)),
          SLOT(folderChanged(const QModelIndex &)));

  connect(m_model, SIGNAL(rootPathChanged(const QString &)),
          SLOT(targetChanged(const QString &)));
}

void ImageListView::currentChanged(const QModelIndex &current,
                                   const QModelIndex &previous) {
  Q_UNUSED(previous);
  if (!current.isValid())
    return;

  if (m_model->isDir(current))
    return;

  SourceInfo info(m_model->fileInfo(current));
  if (info.isReadable()) {
    bool b;
    qint64 id = info.baseName().toInt(&b);
    if (b && id > 0)
      info.setFileId(id);

    emit imageSelected(info);
  }
}

void ImageListView::targetChanged(const QString &path) {
  p_rootIndex = m_model->index(path);
  if (!p_rootIndex.isValid())
    return;

  emit sendTargetChanged(path);
  expandTopLevel();
}

void ImageListView::folderChanged(const QModelIndex &index) {
  if (!index.isValid())
    return;

  resizeColumnToContents(p_rootIndex.column());
}

void ImageListView::expandTopLevel() { expand(p_rootIndex); }

void ImageListView::setSourceImage(const SourceInfo &image) {
  if (setDirectory(image.absoluteDir())) {
    QModelIndex index = m_model->index(image.filePath());
    if (index.isValid()) {
      setCurrentIndex(index);
    }
  }
}

bool ImageListView::setDirectory(const QDir &dir) {
  if (!dir.exists())
    return false;

  p_directory.setPath(dir.path());
  QModelIndex index = m_model->setRootPath(p_directory.path());
  setRootIndex(index);
  return index.isValid();
}

const QStringList ImageListView::viewFilter() {
  QStringList f("*.jpg");
  f << "*.JPG";
  f << "*.jpeg";
  f << "*.JPEG";
  return f;
}
