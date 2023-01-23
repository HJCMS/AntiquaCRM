// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sourceinfo.h"

#include <AntiquaCRM>

SourceInfo::SourceInfo(const QString &prefix) : QFileInfo{} {
  fileId = -1;
  p_target = QString();
  p_prefix = prefix;
}

SourceInfo::SourceInfo(const QFileInfo &other) : QFileInfo{other} {
  fileId = -1;
  p_target = other.path();
  p_prefix = QString();
}

SourceInfo::SourceInfo(const SourceInfo &other) : QFileInfo{other.filePath()} {
  fileId = other.fileId;
  p_target = other.p_target;
  p_prefix = other.p_prefix;
}

SourceInfo &SourceInfo::operator=(const SourceInfo &other) {
  if (this == &other)
    return *this;

  fileId = other.fileId;
  p_target = other.p_target;
  p_prefix = other.p_prefix;
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

void SourceInfo::setPrefix(const QString &prefix) {
  p_prefix = prefix.trimmed();
}

const QString SourceInfo::getPrefix() const { return p_prefix; }

const QString SourceInfo::imageBaseName(qint64 id, const QString &prefix) {
  if (prefix.isEmpty())
    return AntiquaCRM::AUtil::fileNumber(id, 8);

  QString name = AntiquaCRM::AUtil::fileNumber(id, 8);
  name.prepend(prefix);
  return name;
}

void SourceInfo::setTarget(const QDir &dest) {
  if (!dest.exists() || (fileId < 1))
    return;

  QString p = dest.path();
  QString name = imageBaseName(fileId, p_prefix);
  name.append("." + completeSuffix().toLower());
  p.append(dest.separator());
  p.append(name);
  p_target = p;
}

const QString SourceInfo::getFileTarget() { return p_target; }
