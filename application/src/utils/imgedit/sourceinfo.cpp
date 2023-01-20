// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sourceinfo.h"

#include <AntiquaCRM>

SourceInfo::SourceInfo() : QFileInfo{} {
  fileId = -1;
  p_target = QString();
}

SourceInfo::SourceInfo(const QFileInfo &other) : QFileInfo{other} {
  fileId = -1;
  p_target = other.path();
}

SourceInfo::SourceInfo(const SourceInfo &other) : QFileInfo{other.filePath()} {
  fileId = other.fileId;
  p_target = other.p_target;
}

SourceInfo &SourceInfo::operator=(const SourceInfo &other) {
  if (this == &other)
    return *this;

  fileId = other.fileId;
  p_target = other.p_target;
  setFile(other.filePath());
  return *this;
}

bool SourceInfo::isValidSource() const {
  if (fileId > 0 && isFile() && isReadable()) {
    return exists();
  }
  return false;
}

void SourceInfo::setFileId(qint64 id) { fileId = id; }

qint64 SourceInfo::getFileId() const { return fileId; }

const QString SourceInfo::imageBaseName(qint64 id) {
  return AntiquaCRM::AUtil::fileNumber(id, 8);
}

void SourceInfo::setTarget(const QDir &dest) {
  if (!dest.exists() || (fileId < 1))
    return;

  QString p = dest.path();
  QString name = imageBaseName(fileId);
  name.append("." + completeSuffix().toLower());
  p.append(dest.separator());
  p.append(name);
  p_target = p;
}

const QString SourceInfo::getFileTarget() { return p_target; }