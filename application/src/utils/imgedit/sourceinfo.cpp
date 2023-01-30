// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sourceinfo.h"

#include <AntiquaCRM>

SourceInfo::SourceInfo(const QString &target) : QFileInfo{} {
  p_fileId = -1;
  p_target = target;
}

SourceInfo::SourceInfo(const QFileInfo &other) : QFileInfo{other} {
  p_fileId = -1;
  p_target = other.path();
  setFile(other.filePath());
}

SourceInfo::SourceInfo(const SourceInfo &other) : QFileInfo{other.filePath()} {
  p_fileId = other.p_fileId;
  p_target = other.p_target;
  setFile(other.filePath());
}

SourceInfo &SourceInfo::operator=(const SourceInfo &other) {
  if (this == &other)
    return *this;

  p_fileId = other.p_fileId;
  p_target = other.p_target;
  setFile(other.filePath());
  return *this;
}

bool SourceInfo::isValidSource() const {
  if (p_fileId > 0 && isFile() && isReadable()) {
    return exists();
  }
  return false;
}

void SourceInfo::setFileId(qint64 id) { p_fileId = id; }

qint64 SourceInfo::getFileId() const { return p_fileId; }

const QString SourceInfo::imageBaseName(qint64 id) {
  return AntiquaCRM::AUtil::zerofill(id, 8);
}

void SourceInfo::setTarget(const QDir &dest) {
  if (!dest.exists() || (p_fileId < 1))
    return;

  p_target = dest.path();
}

const QString SourceInfo::getTarget() const {
  return p_target;
}
