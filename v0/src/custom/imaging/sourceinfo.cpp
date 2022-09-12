// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sourceinfo.h"

SourceInfo::SourceInfo(const QFileInfo &other) : QFileInfo{other} {
  fileId = -1;
  p_target = QString();
}

bool SourceInfo::isValidSource() {
  if (fileId > 0 && isFile() && isReadable()) {
    QFileInfo t(getFileTarget());
    return t.dir().exists();
  }
  return false;
}

void SourceInfo::setFileId(int id) { fileId = id; }

int SourceInfo::getFileId() { return fileId; }

const QString SourceInfo::imageBaseName(int id) {
  return QString::number(id).rightJustified(8, '0');
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
