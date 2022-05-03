// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "listview.h"
#include "fileviewmodel.h"

#include <QDebug>
#include <QMimeData>
#include <QMimeDatabase>
#include <QMimeType>

ListView::ListView(QWidget *parent) : QListView(parent) {
  setObjectName("ListViewImages");

  m_model = new FileViewModel(this);
  setModel(m_model);

  setRootIndex(m_model->setRootPath(QDir::homePath()));

  connect(this, SIGNAL(clicked(const QModelIndex &)), this,
          SLOT(checkSingleClicked(const QModelIndex &)));
  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(checkDoubleClicked(const QModelIndex &)));
}

bool ListView::checkJpeg(const QFileInfo &info) {
  QMimeDatabase mdb;
  QMimeType mt = mdb.mimeTypeForFile(info);
  // grep image/jpeg /etc/mime.types
  return (mt.inherits("image/jpeg")) ? true : false;
}

void ListView::checkSingleClicked(const QModelIndex &index) {
  QFileInfo info = m_model->fileInfo(index);
  if (checkJpeg(info)) {
    emit s_previewImage(info.filePath());
    return;
  }
}

void ListView::checkDoubleClicked(const QModelIndex &index) {
  if (m_model->isDir(index)) {
    setRootIndex(index);
    return;
  }
  QFileInfo info = m_model->fileInfo(index);
  if (checkJpeg(info)) {
    emit s_selectionChanged(info.filePath());
    return;
  }
}

void ListView::goTo(const QString &dest) {
  setRootIndex(m_model->setRootPath(dest));
}

void ListView::goUpward() {
  QDir d(m_model->last());
  if(!d.exists())
    return;

  if(d.path() == QDir::root().path())
    return;

  d.cdUp();
  if(d.exists()) {
    setRootIndex(m_model->setRootPath(d.path()));
  }
}
