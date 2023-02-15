// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imageview.h"
#include "rubberband.h"
#include "sourceinfo.h"

#ifdef ANTIQUA_DEVELOPEMENT
#include <QDebug>
#endif
#include <ASettings>
#include <QApplication>
#include <QBuffer>
#include <QDataStream>
#include <QDir>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QImageReader>

ImageView::ImageView(QSize maxsize, QWidget *parent)
    : QGraphicsView(parent), p_max(maxsize), p_format("jpeg") {
  setObjectName("ImagePreview");
  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  setBackgroundRole(QPalette::Base);
  setCacheMode(QGraphicsView::CacheNone);
  setMinimumWidth((p_max.width() / 2));
  setToolTip(tr("Right mouse button with mouse wheel to zoom the image!"));
  m_sql = new AntiquaCRM::ASqlCore(this);
  m_rubberband = nullptr;
  m_scene = new QGraphicsScene(rect(), this);
  setPixmapItem();
  setModified(false);
}

void ImageView::setPixmapItem(const QPixmap &pixmap) {
  m_scene->clear();
  m_pixmapItem = m_scene->addPixmap(pixmap);
  setSceneRect(pixmap.rect());
  setScene(m_scene);
}

const QSize ImageView::maxSourceSize() const {
  QSize maxSize(1366, 768);
  return maxSize;
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

const QRect ImageView::sourceRect() const {
  if (m_pixmapItem == nullptr)
    return QRect();

  if (m_pixmapItem->pixmap().isNull())
    return QRect();

  return m_pixmapItem->pixmap().rect();
}

void ImageView::wheelEvent(QWheelEvent *event) {
  if (event->buttons() & Qt::RightButton)
    zoomWith(qPow(1.2, event->angleDelta().y() / 240.0));
}

void ImageView::resizeEvent(QResizeEvent *event) {
  if (m_pixmapItem != nullptr && !m_pixmapItem->pixmap().isNull()) {
    fitInView(m_pixmapItem, Qt::KeepAspectRatio);
  }
  QGraphicsView::resizeEvent(event);
}

void ImageView::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    if (m_rubberband == nullptr) {
      m_rubberband = new RubberBand(this);
    }
    p_startPoint = event->pos();
    m_rubberband->setGeometry(QRect(p_startPoint, QSize(1, 1)));
    m_rubberband->show();
  }
  QGraphicsView::mousePressEvent(event);
}

void ImageView::mouseMoveEvent(QMouseEvent *event) {
  if (m_rubberband != nullptr)
    m_rubberband->setGeometry(QRect(p_startPoint, event->pos()));

  QGraphicsView::mouseMoveEvent(event);
}

void ImageView::setPixmap(const QPixmap &pixmap) {
  setPixmapItem(pixmap);
  p_pixmapCache.insert("source", pixmap);
  emit sendImageLoadSuccess(true);
}

void ImageView::setImage(const QImage &img) {
  QPixmap sp = QPixmap::fromImage(img);
  if (sp.isNull()) {
    emit sendImageLoadSuccess(false);
    return;
  }

  qreal w = qMax(maxSourceSize().width(), p_max.width());
  qreal h = qMax(maxSourceSize().height(), p_max.height());
  QPixmap op = sp.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  if (op.isNull()) {
    op.detach();
    return;
  }
  sp.detach();
  setPixmap(op);
}

void ImageView::setImageFile(const SourceInfo &file) {
  QImageReader reader(file.filePath());
  QImage img = reader.read();
  if (img.isNull()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << file.getFileId() << file.filePath();
#endif
    emit sendImageLoadSuccess(false);
    return;
  }
  setImage(img);
  p_currentPreview = file;
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

void ImageView::adjust() {
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

void ImageView::cutting() {
  if (m_rubberband != nullptr && m_rubberband->isValid()) {
    QRect fromRect(p_startPoint, m_rubberband->size());
    QRectF toRect = mapToScene(fromRect).boundingRect().normalized();
    QPixmap _pixmap = m_pixmapItem->pixmap().copy(toRect.toRect());
    if (_pixmap.isNull())
      return;

    p_pixmapCache.insert("scaled", _pixmap);
    setPixmapItem(_pixmap);
    _pixmap.detach();

    m_rubberband->reset();
  }
}

void ImageView::reset() {
  QPixmap _pixmap;
  if (p_pixmapCache.find("source", &_pixmap)) {
    setPixmap(_pixmap);
  } else if (p_currentPreview.isValidSource()) {
    setImageFile(p_currentPreview);
  }
  if (m_rubberband != nullptr && m_rubberband->isValid())
    m_rubberband->reset();
}

void ImageView::setModified(bool b) {
  modified = b;
  emit hasModified(b);
}

void ImageView::clear() { setPixmapItem(); }

bool ImageView::saveImageTo(const SourceInfo &info) {
  QDir destDir(info.getTarget());
  if (!destDir.exists() || !info.isValidSource())
    return false; // Bei Standardfehlern hier aussteigen!

  /**
   * @brief Bildspeicher einlesen
   * @list Temporäres befüllen in dieser Reihenfolge!
   *  @li 1) Existiert eine Bearbeitete Version?
   *  @li 2) Auf das Quellbild zurückgreifen!
   *  @li 3) Fehler und aussteigen!
   */
  QPixmap _pixmap, _temp;
  if (p_pixmapCache.find("scaled", &_temp)) {
    _pixmap = _temp;
  } else if (p_pixmapCache.find("source", &_temp)) {
    _pixmap = _temp;
  } else {
    qWarning("Pixmap - not found!");
    return false;
  }
  _temp.detach();

  if (_pixmap.isNull()) {
    qFatal("ImageView::saveImageTo - empty pixmap detected!");
    return false;
  }

  qreal w = qMax(maxSourceSize().width(), _pixmap.size().width());
  qreal h = qMax(maxSourceSize().height(), _pixmap.size().height());
  QPixmap p = _pixmap.scaled(w, h, // Speicherplatz reduzieren
                             Qt::KeepAspectRatio, Qt::SmoothTransformation);

  QImage img = p.toImage();
  if (img.isNull())
    return false;

  _pixmap.detach();

  QString filename(info.imageBaseName(info.getFileId()));
  filename.append(".jpg");
  QFileInfo ouput(info.getTarget(), filename);
  QFile fp(ouput.filePath());
  if (fp.open(QIODevice::WriteOnly)) {
    img.save(&fp, p_format.data(), quality());
    fp.close();
    return true;
  }
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
  if (m_pixmapItem == nullptr)
    return QImage();

  if (m_pixmapItem->pixmap().isNull())
    return QImage();

  return m_pixmapItem->pixmap().toImage();
}

const SourceInfo ImageView::getSource() { return p_currentPreview; }

bool ImageView::isModified() { return modified; }

ImageView::~ImageView() {
  setPixmapItem(QPixmap(0, 0));
  p_pixmapCache.clear();
  m_scene->deleteLater();
}
