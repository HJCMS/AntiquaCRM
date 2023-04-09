// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imageviewer.h"
#include "private/rubberband.h"

#include <QColor>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyle>
#include <QStylePainter>

namespace AntiquaCRM {

ImageViewer::ImageViewer(const QSize &max, QWidget *parent)
    : QGraphicsView{parent}, p_maxSize{max} {
  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  setBackgroundRole(QPalette::Base);
  setCacheMode(QGraphicsView::CacheNone);
  setMinimumWidth((p_maxSize.width() / 2));
  m_scene = new QGraphicsScene(rect(), this);
  m_pixItem = nullptr;
  m_rubberband = nullptr;
}

ImageViewer::~ImageViewer() {
  setPixmapItem(QPixmap(0, 0));
  p_pixCache.clear();
  m_scene->deleteLater();
}

bool ImageViewer::setPixmapItem(const QPixmap &pixmap) {
  m_scene->clear();
  m_pixItem = m_scene->addPixmap(pixmap);
  setSceneRect(pixmap.rect());
  setScene(m_scene);
  return (m_pixItem != nullptr);
}

const QSize ImageViewer::getMaxScaleSize() const {
  QSize maxSize(1366, 768);
  return maxSize;
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
    p_startRect = event->pos();
    m_rubberband->setGeometry(QRect(p_startRect, QSize(1, 1)));
    m_rubberband->show();
  }
  QGraphicsView::mousePressEvent(event);
}

void ImageViewer::mouseMoveEvent(QMouseEvent *) {}

void ImageViewer::zoom(qreal f) {
  // vertical shearing factor
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

void ImageViewer::reset() {
  QPixmap _pixmap;
  if (p_pixCache.find("source", &_pixmap)) {
    setPixmap(_pixmap);
  }
  if (m_rubberband != nullptr && m_rubberband->isValid())
    m_rubberband->reset();
}

void ImageViewer::clear() { setPixmapItem(); }

void ImageViewer::setPixmap(const QPixmap &pixmap) {
  bool _success = setPixmapItem(pixmap);
  p_pixCache.insert("source", pixmap);
  emit sendSetViewSuccess(_success);
}

void ImageViewer::setImage(const QImage &image) {
  QPixmap _px = QPixmap::fromImage(image);
  if (_px.isNull()) {
    emit sendSetViewSuccess(false);
    return;
  }

  const QSize _s = getMaxScaleSize();
  qreal _w = qMax(_s.width(), p_maxSize.width());
  qreal _h = qMax(_s.height(), p_maxSize.height());
  QPixmap _opx = _px.scaled(_w, _h,              // size
                            Qt::KeepAspectRatio, // ratio
                            Qt::SmoothTransformation);
  if (_opx.isNull()) {
    _opx.detach();
    return;
  }
  _px.detach();
  setPixmap(_opx);
}

const QImage ImageViewer::getImage() {
  if (m_pixItem == nullptr)
    return QImage();

  if (m_pixItem->pixmap().isNull())
    return QImage();

  return m_pixItem->pixmap().toImage();
}

} // namespace AntiquaCRM
