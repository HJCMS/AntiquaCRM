// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imageview.h"
#include "sourceinfo.h"

#ifdef ANTIQUA_DEVELOPEMENT
#include <QDebug>
#endif
#include <QApplication>
#include <QBuffer>
#include <QDataStream>
#include <QDir>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QImageReader>
#include <QPixmap>
#include <QScreen>
#include <QtMath>

ImageView::ImageView(QSize maxsize, QWidget *parent)
    : QGraphicsView(parent), p_max(maxsize), p_format("jpeg") {
  setObjectName("ImagePreview");
  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  setBackgroundRole(QPalette::Base);
  setCacheMode(QGraphicsView::CacheNone);
  setMinimumWidth((p_max.width() / 2));
  setToolTip(tr("Right mouse button with mouse wheel to zoom the image!"));
  m_sql = new AntiquaCRM::ASqlCore(this);

  m_scene = new QGraphicsScene(this);
  /** @warning Lese die ImageView::clear() Dokumentation */
  clear();
}

const QSize ImageView::screenSize() const {
  QScreen *m_screen = qApp->primaryScreen();
  return m_screen->virtualSize();
}

void ImageView::zoomWith(qreal f) {
  // vertical shearing factor
  const qreal yf = transform().m11();
  if ((f < 1 && yf < 0.1) || (f > 1 && yf > 10))
    return;

  scale(f, f);
}

const QImage ImageView::toDatabaseSize(const QImage &img) {
  QImage image(img);
  return image.scaled(p_max, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

void ImageView::wheelEvent(QWheelEvent *event) {
  if (event->buttons() & Qt::RightButton)
    zoomWith(qPow(1.2, event->angleDelta().y() / 240.0));
}

void ImageView::resizeEvent(QResizeEvent *event) {
  if (m_pixmap != nullptr && !m_pixmap->pixmap().isNull()) {
    fitInView(m_pixmap, Qt::KeepAspectRatio);
  }
  QGraphicsView::resizeEvent(event);
}

void ImageView::setImage(const QImage &img) {
  QPixmap p = QPixmap::fromImage(img);
  if (p.isNull()) {
    emit sendImageLoadSuccess(false);
    return;
  }

  int w = qMax(screenSize().width(), p_max.width());
  int h = qMax(screenSize().height(), p_max.height());
  p_pixmap = p.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  m_scene->clear();
  m_pixmap = m_scene->addPixmap(p_pixmap);
  setSceneRect(p_pixmap.rect());
  setScene(m_scene);
  emit sendImageLoadSuccess(true);
}

void ImageView::setImageFile(const SourceInfo &file) {
  QImageReader reader(file.filePath());
  QImage img = reader.read();
  if (img.isNull()) {
    emit sendImageLoadSuccess(false);
    return;
  }
  setImage(img);
  p_currentPreview = file;
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO
           << p_currentPreview.getFileId() << p_currentPreview.filePath()
           << Qt::endl
           << file.getFileId() << file.filePath();
#endif
}

void ImageView::setRawImage(const QByteArray &data) {
  QImage img = QImage::fromData(data, "jpeg");
  if (img.isNull()) {
    emit sendImageLoadSuccess(false);
    return;
  }
  setImage(img);
}

void ImageView::zoomIn() { zoomWith(2); }

void ImageView::zoomOut() { zoomWith(0.5); }

void ImageView::zoomReset() {
  if (!qFuzzyCompare(transform().m11(), qreal(1))) {
    resetTransform();
  }
}

void ImageView::rotate() {
  QImage img(getImage());
  if (img.isNull())
    return;

  QTransform transform;
  transform.rotate(90.0);
  QImage out = img.transformed(transform, Qt::SmoothTransformation);
  if (out.isNull())
    return;

  setImage(out);
  update();
}

void ImageView::setModified(bool b) {
  if (modified != b) {
    modified = b;
    emit hasModified(true);
  } else {
    emit hasModified(false);
  }
}

void ImageView::clear() {
  m_scene->clear();
  m_pixmap = m_scene->addPixmap(QPixmap(0, 0));
}

bool ImageView::saveImageTo(const SourceInfo &info) {
  if (!info.dir().exists())
    return false;

  if (p_pixmap.isNull())
    return false;

  QPixmap p = p_pixmap.scaled(screenSize(), /* auf Desktopgröße reduzieren */
                              Qt::KeepAspectRatio, Qt::SmoothTransformation);
  QImage img = p.toImage();
  if (img.isNull())
    return false;

  SourceInfo dest(info);
  if (!dest.isValidSource())
    return false;

  QFile fp(dest.getFileTarget());
  if (fp.open(QIODevice::WriteOnly)) {
    img.save(&fp, p_format.data(), quality());
    fp.close();
    return true;
  }
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << "Failed" << info.filePath()
           << dest.getFileTarget();
#endif
  return false;
}

bool ImageView::readFromDatabase(qint64 articleId) {
  if (articleId < 1)
    return false;

  QString strArticleId = QString::number(articleId);
  if (strArticleId.isEmpty())
    return false;

  QString select("SELECT im_imgdata FROM ");
  select.append("inventory_images WHERE im_id=");
  select.append(strArticleId);
  select.append(" ORDER BY im_id LIMIT 1;");

  QByteArray data;
  QSqlQuery q = m_sql->query(select);
  if (q.next()) {
    data = QByteArray::fromBase64(q.value(0).toByteArray(),
                                  QByteArray::Base64Encoding);
    if (data.size() > 60)
      setRawImage(data);

    return true;
  }
  q.clear();
  data.clear();
  return false;
}

bool ImageView::storeInDatabase(qint64 articleId) {
  if (articleId < 1)
    return false;

  QString strArticleId = QString::number(articleId);
  if (strArticleId.isEmpty())
    return false;

  QImage img = toDatabaseSize(getImage());
  if (img.isNull())
    return false;

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << "Store Image in Database:" << img.size();
#endif

  QByteArray rawimg;
  QBuffer buffer(&rawimg);
  buffer.open(QIODevice::WriteOnly);
  img.save(&buffer, p_format.data(), quality());
  buffer.close();

  QByteArray base64 = rawimg.toBase64();
  QString sql("SELECT im_id FROM inventory_images WHERE im_id=");
  sql.append(strArticleId);
  sql.append(" LIMIT 1;");
  // Aufräumen
  rawimg.clear();

  QSqlQuery q = m_sql->query(sql);
  if (q.next()) {
    sql = QString("UPDATE inventory_images SET ");
    sql.append("im_imgdata='");
    sql.append(base64);
    sql.append("' WHERE im_id=");
    sql.append(strArticleId);
    sql.append(";");
  } else {
    sql = QString("INSERT INTO inventory_images (");
    sql.append("im_id,im_imgdata) VALUES (");
    sql.append(strArticleId);
    sql.append(",'");
    sql.append(base64);
    sql.append("');");
  }
  m_sql->query(sql);
  if (!m_sql->lastError().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << m_sql->lastError() << sql;
#endif
    qWarning("Store Image in Database failed.");
    return false;
  }
  // Aufräumen
  q.clear();
  base64.clear();
  return true;
}

bool ImageView::removeFromDatabase(qint64 articleId) {
  if (articleId < 1)
    return false;

  QString strArticleId = QString::number(articleId);
  if (strArticleId.isEmpty())
    return false;

  QString sql("DELETE FROM inventory_images ");
  sql.append("WHERE im_id=");
  sql.append(strArticleId);
  sql.append(";");

  m_sql->query(sql);
  if (!m_sql->lastError().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << m_sql->lastError() << sql;
#endif
    qWarning("Delete Image from Database failed.");
    return false;
  }
  emit sendImageLoadSuccess(true);
  return true;
}

const QImage ImageView::getImage() {
  if (m_pixmap == nullptr)
    return QImage();

  if (m_pixmap->pixmap().isNull())
    return QImage();

  return m_pixmap->pixmap().toImage();
}

const SourceInfo ImageView::getSource() { return p_currentPreview; }

bool ImageView::isModified() { return modified; }

ImageView::~ImageView() {
  m_pixmap = m_scene->addPixmap(QPixmap(0, 0));
  p_pixmap.detach();
  m_scene->deleteLater();
}
