// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagesource.h"

#include <QSize>

namespace AntiquaCRM {

ImageSource::ImageSource(const QString &target) : QFileInfo{} {
  p_fileId = -1;
  p_target = target;
  p_pixmap = QPixmap();
}

ImageSource::ImageSource(const QFileInfo &other) : QFileInfo{other} {
  p_fileId = -1;
  p_target = other.path();
  p_pixmap = QPixmap();
  setFile(other.filePath());
}

ImageSource::ImageSource(const ImageSource &other)
    : QFileInfo{other.filePath()} {
  p_fileId = other.p_fileId;
  p_target = other.p_target;
  p_pixmap = other.p_pixmap;
  setFile(other.filePath());
}

ImageSource &ImageSource::operator=(const ImageSource &other) {
  if (this == &other)
    return *this;

  p_fileId = other.p_fileId;
  p_target = other.p_target;
  p_pixmap = other.p_pixmap;
  setFile(other.filePath());
  return *this;
}

bool ImageSource::isValidSource() const {
  if (p_fileId > 0 && isFile() && isReadable()) {
    return exists();
  }
  return false;
}

bool ImageSource::isOriginal() { return p_pixmap.isNull(); };

void ImageSource::setFileId(qint64 id) {
  if (id < 1) {
    qWarning("ImageSource::setFileId - rejected!");
    return;
  }
  p_fileId = id;
}

qint64 ImageSource::getFileId() const { return p_fileId; }

const QString ImageSource::toBaseName(qint64 id, qint8 zerofill) {
  return AntiquaCRM::AUtil::zerofill(id, zerofill);
}

void ImageSource::setTarget(const QDir &dest) {
  if (!dest.exists() || (p_fileId < 1))
    return;

  p_target = dest.path();
}

const QString ImageSource::getTarget() const { return p_target; }

void ImageSource::setPixmap(const QPixmap &pix) {
  if (pix.isNull() || pix.height() < 10 || pix.width() < 10)
    return;

  p_pixmap = pix;
}

const QPixmap ImageSource::getPixmap() { return p_pixmap; }

void ImageSource::removePixmap() { p_pixmap = QPixmap(); }

} // namespace AntiquaCRM
