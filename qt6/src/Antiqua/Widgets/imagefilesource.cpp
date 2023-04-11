// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagefilesource.h"

#include <QSize>

namespace AntiquaCRM {

#ifndef IMAGE_SAVE_FORMAT
#define IMAGE_SAVE_FORMAT "jpeg"
#endif

#ifndef IMAGE_JPEG_QUALITY
#define IMAGE_JPEG_QUALITY 85
#endif

ImageFileSource::ImageFileSource(const QString &target) : QFileInfo{} {
  p_fileId = -1;
  p_tempTarget = target;
  p_pixmap = QPixmap();
}

ImageFileSource::ImageFileSource(const QFileInfo &other) : QFileInfo{other} {
  p_fileId = -1;
  p_tempTarget = other.path();
  p_pixmap = QPixmap();
  setFile(other.filePath());
}

ImageFileSource::ImageFileSource(const ImageFileSource &other)
    : QFileInfo{other.filePath()} {
  p_fileId = other.p_fileId;
  p_tempTarget = other.p_tempTarget;
  p_pixmap = other.p_pixmap;
  setFile(other.filePath());
}

ImageFileSource &ImageFileSource::operator=(const ImageFileSource &other) {
  if (this == &other)
    return *this;

  p_fileId = other.p_fileId;
  p_tempTarget = other.p_tempTarget;
  p_pixmap = other.p_pixmap;
  setFile(other.filePath());
  return *this;
}

bool ImageFileSource::loadFromDatabase(const QByteArray &data) {
  QImage _img = QImage::fromData(data, IMAGE_SAVE_FORMAT);
  QPixmap _pixmap = QPixmap::fromImage(_img);
  bool _status = _pixmap.isNull() ? false : true;
  if (_status)
    setCachedPixmap(_pixmap);

  return _status;
}

bool ImageFileSource::isValidSource() const {
  if (p_fileId > 0 && isFile() && isReadable()) {
    return exists();
  }
  return false;
}

bool ImageFileSource::isOriginal() { return p_pixmap.isNull(); };

void ImageFileSource::setFileId(qint64 articleId) {
  if (articleId < 1) {
    qWarning("ImageFileSource::setFileId - rejected!");
    return;
  }
  p_fileId = articleId;
}

qint64 ImageFileSource::getFileId() const { return p_fileId; }

const QString ImageFileSource::toBaseName(qint64 id, qint8 zerofill) {
  return AntiquaCRM::AUtil::zerofill(id, zerofill);
}

void ImageFileSource::setTempTarget(const QDir &dest) {
  if (!dest.exists() || (p_fileId < 1))
    return;

  p_tempTarget = dest.path();
}

const QString ImageFileSource::getTempTarget() const { return p_tempTarget; }

void ImageFileSource::setCachedPixmap(const QPixmap &pix) {
  if (pix.isNull() || pix.height() < 10 || pix.width() < 10)
    return;

  p_pixmap = pix;
}

const QPixmap ImageFileSource::getCachedPixmap() { return p_pixmap; }

void ImageFileSource::removeCachedPixmap() { p_pixmap = QPixmap(); }

bool ImageFileSource::findInDatabase(AntiquaCRM::ASqlCore *db, qint64 articleId) {
  Q_CHECK_PTR(db);
  if (articleId < 1 || !db->open()) {
    qWarning("ImageFileSource::findInDatabase - reject - invalid usage!");
    return false;
  }

  QString _sql("SELECT im_imgdata FROM ");
  _sql.append("inventory_images WHERE im_id=");
  _sql.append(QString::number(articleId));
  _sql.append(" ORDER BY im_id LIMIT 1;");

  bool _status = false;
  QByteArray _data;
  QSqlQuery _query = db->query(_sql);
  if (_query.next()) {
    _data = QByteArray::fromBase64(_query.value(0).toByteArray(),
                                   QByteArray::Base64Encoding);
    _status = loadFromDatabase(_data);
  }

#ifdef ANTIQUA_DEVELOPEMENT
  if (!_status) {
    qDebug() << Q_FUNC_INFO << _sql << Qt::endl << db->lastError();
  }
#endif

  _sql.clear();
  _data.clear();
  _query.clear();

  return _status;
}

bool ImageFileSource::storeInDatabase(AntiquaCRM::ASqlCore *db, const QSize &max,
                                  qint64 articleId) {
  Q_CHECK_PTR(db);
  if (articleId < 1 || !db->open()) {
    qWarning("ImageFileSource::storeInDatabase - reject - invalid usage!");
    return false;
  }

  QImage _img = getCachedPixmap().toImage().scaled(max, Qt::KeepAspectRatio,
                                                   Qt::SmoothTransformation);
  if (_img.isNull())
    return false;

  QByteArray _raw;
  QBuffer _buffer(&_raw);
  _buffer.open(QIODevice::WriteOnly);
  _img.save(&_buffer, IMAGE_SAVE_FORMAT, IMAGE_JPEG_QUALITY);
  _buffer.close();

  QString _id = QString::number(articleId);
  QByteArray _base64 = _raw.toBase64();
  QString _sql("SELECT im_id FROM inventory_images");
  _sql.append(" WHERE im_id=" + _id + " LIMIT 1;");

  QSqlQuery _query = db->query(_sql);
  if (_query.next()) {
    _sql = QString("UPDATE inventory_images SET ");
    _sql.append("im_imgdata='");
    _sql.append(_base64);
    _sql.append("' WHERE im_id=");
    _sql.append(_id);
    _sql.append(";");
  } else {
    _sql = QString("INSERT INTO inventory_images (");
    _sql.append("im_id,im_imgdata) VALUES (");
    _sql.append(_id);
    _sql.append(",'");
    _sql.append(_base64);
    _sql.append("');");
  }

  db->query(_sql);
  bool _status = db->lastError().isEmpty();
  if (!_status) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << _sql << Qt::endl << db->lastError();
#else
    qWarning("Store Image to Database failed.");
#endif
  }

  _raw.clear();
  _base64.clear();
  _sql.clear();
  _query.clear();

  return _status;
}

bool ImageFileSource::removeFromDatabase(AntiquaCRM::ASqlCore *db,
                                     qint64 articleId) {
  Q_CHECK_PTR(db);
  if (articleId < 1 || !db->open()) {
    qWarning("ImageFileSource::removeFromDatabase - reject - invalid usage!");
    return false;
  }

  QString _id = QString::number(articleId);
  if (_id.isEmpty())
    return false;

  QString _sql("DELETE FROM inventory_images");
  _sql.append(" WHERE im_id=" + _id + ";");

  db->query(_sql);
  bool _status = db->lastError().isEmpty();
  if (!_status) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << _sql << Qt::endl << db->lastError();
#else
    qWarning("Delete Image from Database failed.");
#endif
  }
  _sql.clear();
  return _status;
}

} // namespace AntiquaCRM
