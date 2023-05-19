// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagetreeview.h"

#include <QDebug>

ImageTreeView::ImageTreeView(QWidget *parent) : QTreeView{parent} {
  setWordWrap(false);
  setAlternatingRowColors(true);
  setAnimated(false);
  setSortingEnabled(true);
  setItemsExpandable(true);
  setExpandsOnDoubleClick(true);

  p_directory = QDir::home();
  p_directory.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
  p_directory.setNameFilters(filter());

  m_model = new QFileSystemModel(this);
  m_model->setObjectName("images_view_model");
  m_model->setReadOnly(true);
  m_model->setOption(QFileSystemModel::DontWatchForChanges, true);
  m_model->setOption(QFileSystemModel::DontResolveSymlinks, true);
  m_model->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
  m_model->setNameFilters(filter());
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

void ImageTreeView::currentChanged(const QModelIndex &current,
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

void ImageTreeView::targetChanged(const QString &path) {
  p_rootIndex = m_model->index(path);
  if (!p_rootIndex.isValid())
    return;

  emit sendTargetChanged(path);
  expandTopLevel();
}

void ImageTreeView::folderChanged(const QModelIndex &index) {
  if (!index.isValid())
    return;

  resizeColumnToContents(p_rootIndex.column());
}

void ImageTreeView::expandTopLevel() { expand(p_rootIndex); }

void ImageTreeView::setSourceImage(const SourceInfo &image) {
  const QModelIndex _index = m_model->index(image.filePath());
  if (_index.isValid()) {
    scrollTo(_index, QAbstractItemView::EnsureVisible);
    setCurrentIndex(_index);
  }
}

bool ImageTreeView::setDirectory(const QDir &dir) {
  if (!dir.exists())
    return false;

  p_directory.setPath(dir.path());
  QModelIndex index = m_model->setRootPath(p_directory.path());
  setRootIndex(index);
  return index.isValid();
}

const QStringList ImageTreeView::filter() const {
  return QStringList({"*.jpg", "*.JPG", "*.jpeg", "*.JPEG"});
}

const QDir ImageTreeView::directory() const {
  return QDir(m_model->rootPath());
}
