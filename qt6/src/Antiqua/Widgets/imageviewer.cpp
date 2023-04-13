// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imageviewer.h"
#include "private/rubberband.h"

#include <QApplication>
#include <QDebug>
#include <QTransform>

namespace AntiquaCRM {

ImageViewer::ImageViewer(QWidget *parent) : QGraphicsView{parent} {
  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  setBackgroundRole(QPalette::Base);
  setCacheMode(QGraphicsView::CacheNone);

  m_scene = new QGraphicsScene(rect(), this);
  m_pixItem = nullptr;
  m_rubberband = nullptr;
}

ImageViewer::~ImageViewer() {
  setPixmapItem(QPixmap(0, 0));
  p_origin.clear();
  m_scene->deleteLater();
}

bool ImageViewer::setPixmapItem(const QPixmap &pixmap) {
  m_scene->clear();
  m_pixItem = m_scene->addPixmap(pixmap);
  setSceneRect(pixmap.rect());
  setScene(m_scene);
  return (m_pixItem != nullptr);
}

void ImageViewer::wheelEvent(QWheelEvent *event) {
  if (event->buttons() & Qt::RightButton)
    zoom(qPow(1.2, event->angleDelta().y() / 240.0));
}

void ImageViewer::resizeEvent(QResizeEvent *event) {
  if (m_pixItem != nullptr && !m_pixItem->pixmap().isNull()) {
    fitInView(m_pixItem, Qt::KeepAspectRatio);
  }
  QGraphicsView::resizeEvent(event);
}

void ImageViewer::mousePressEvent(QMouseEvent *event) {
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

void ImageViewer::mouseMoveEvent(QMouseEvent *event) {
  if (m_rubberband != nullptr)
    m_rubberband->setGeometry(QRect(p_startPoint, event->pos()));

  QGraphicsView::mouseMoveEvent(event);
}

void ImageViewer::zoom(qreal f) {
  // horizontal scaling factor
  const qreal yf = transform().m11();
  if ((f < 1 && yf < 0.1) || (f > 1 && yf > 10))
    return;

  scale(f, f);
}

void ImageViewer::zoomIn() { zoom(2); }

void ImageViewer::zoomOut() { zoom(0.5); }

void ImageViewer::adjust() {
  if (!qFuzzyCompare(transform().m11(), qreal(1))) {
    resetTransform();
  }
}

void ImageViewer::rotate() {
  if (m_pixItem->pixmap().isNull())
    return;

  QTransform transform;
  transform.rotate(90.0);
  QPixmap _pixmap =
      m_pixItem->pixmap().transformed(transform, Qt::SmoothTransformation);
  if (_pixmap.isNull())
    return;

  if (setPixmapItem(_pixmap))
    update();

  _pixmap.detach();
}

void ImageViewer::reset() {
  QPixmap _pixmap;
  if (p_origin.find("source", &_pixmap)) {
    setPixmapItem(_pixmap);
    adjust();
  }
  _pixmap.detach();

  if (m_rubberband != nullptr && m_rubberband->isValid())
    m_rubberband->reset();
}

void ImageViewer::cutting() {
  if (m_rubberband == nullptr || !m_rubberband->isValid())
    return;

  QRect _from(p_startPoint, m_rubberband->size());
  QRectF _to = mapToScene(_from).boundingRect().normalized();
  QPixmap _pixmap = m_pixItem->pixmap().copy(_to.toRect());
  if (_pixmap.isNull())
    return;

  setPixmapItem(_pixmap);
  _pixmap.detach();

  m_rubberband->reset();
}

void ImageViewer::clear() {
  setPixmapItem();
  p_origin.clear();
}

void ImageViewer::setPixmap(const QPixmap &pixmap) {
  bool _success = false;
  const QSize _s = getMaxScaleSize();
  qreal _max_with = qMax(_s.width(), p_maxSize.width());
  qreal _max_height = qMax(_s.height(), p_maxSize.height());
  if (pixmap.width() > _max_with || pixmap.height() > _max_height) {
    // Scale
    QPixmap _scaled = pixmap.scaled(_max_with, _max_height,
                                    Qt::KeepAspectRatio, // ratio
                                    Qt::SmoothTransformation);
    if (_scaled.isNull()) {
      _scaled.detach();
      return;
    }
    _scaled.detach();
    _success = setPixmapItem(_scaled);
    if (_success)
      p_origin.insert("source", _scaled);
  } else {
    _success = setPixmapItem(pixmap);
    if (_success)
      p_origin.insert("source", pixmap);
  }

  emit sendSetSceneView(_success);
}

void ImageViewer::setImage(const QImage &image) {
  QPixmap _pixmap = QPixmap::fromImage(image);
  if (_pixmap.isNull()) {
    emit sendSetSceneView(false);
    return;
  }
  setPixmap(_pixmap);
}

const QSize ImageViewer::getMaxScaleSize() const {
  QSize _size = qApp->screenAt(pos())->size();
  if (!_size.isValid())
    _size = QSize(800, 600);

  return _size;
}

const QPixmap ImageViewer::getPixmap() {
  if (m_pixItem == nullptr)
    return QPixmap();

  if (m_pixItem->pixmap().isNull())
    return QPixmap();

  return m_pixItem->pixmap();
}

} // namespace AntiquaCRM
