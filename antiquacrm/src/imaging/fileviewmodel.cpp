// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "fileviewmodel.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>

FileViewModel::FileViewModel(QObject *parent) : QFileSystemModel(parent) {
  setOption(QFileSystemModel::DontWatchForChanges, true);
  setOption(QFileSystemModel::DontResolveSymlinks, true);
  setReadOnly(true);
  setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

  connect(this, SIGNAL(rootPathChanged(const QString &)), this,
          SLOT(currentTarget(const QString &)));
  connect(this, SIGNAL(directoryLoaded(const QString &)), this,
          SLOT(currentTarget(const QString &)));
}

void FileViewModel::currentTarget(const QString &path) {
  p_curDir = QDir(path);
}

const QDir FileViewModel::last() { return p_curDir; }
