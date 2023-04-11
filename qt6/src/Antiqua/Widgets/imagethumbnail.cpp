// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imagethumbnail.h"

#include <QGraphicsScene>

namespace AntiquaCRM {

ImageThumbnail::ImageThumbnail(QWidget *parent)
    : QGraphicsView{parent}, m_thumbnail{nullptr} {
  setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
  setBackgroundRole(QPalette::Base);
  setCacheMode(QGraphicsView::CacheNone);
  setScene(new QGraphicsScene(rect(), this));
}

ImageThumbnail::~ImageThumbnail() {
  setPixmapItem(QPixmap(0, 0));
  p_pixCache.clear();
  scene()->deleteLater();
}

const QSize ImageThumbnail::maxScaleSize() const { return QSize(600, 400); }

bool ImageThumbnail::setPixmapItem(const QPixmap &pixmap) {
  scene()->clear();
  m_thumbnail = scene()->addPixmap(pixmap);
  setSceneRect(pixmap.rect());
  return (m_thumbnail != nullptr);
}

void ImageThumbnail::wheelEvent(QWheelEvent *event) {
  if (event->buttons() & Qt::RightButton)
    zoom(qPow(1.2, event->angleDelta().y() / 240.0));
}

void ImageThumbnail::resizeEvent(QResizeEvent *event) {
  if (m_thumbnail != nullptr && !m_thumbnail->pixmap().isNull()) {
    fitInView(m_thumbnail, Qt::KeepAspectRatio);
  }
  QGraphicsView::resizeEvent(event);
}

void ImageThumbnail::zoom(qreal f) {
  const qreal yf = transform().m11();
  if ((f < 1 && yf < 0.1) || (f > 1 && yf > 10))
    return;

  scale(f, f);
}

void ImageThumbnail::zoomIn() { zoom(2); }

void ImageThumbnail::zoomOut() { zoom(0.5); }

void ImageThumbnail::adjust() {
  if (!qFuzzyCompare(transform().m11(), qreal(1))) {
    resetTransform();
  }
}

void ImageThumbnail::clear() { setPixmapItem(); }

void ImageThumbnail::setPixmap(const QPixmap &pixmap) {
  QPixmap _source(pixmap);
  if (_source.isNull()) {
    return;
  }

  /*
   * Den Speicher möglichst klein halten, wenn das limit überschritten wird eine
   * Größenänderung durchführen.
   */
  const QSize _ms = maxScaleSize();
  if (_source.width() <= _ms.width() || _source.height() <= _ms.height()) {
    if (setPixmapItem(_source))
      p_pixCache.insert("source", _source);

    return;
  }

  qreal _width = qMax(_ms.width(), _source.width());
  qreal _height = qMax(_ms.height(), _source.height());
  QPixmap _scaled = _source.scaled(_width, _height,     // size
                                   Qt::KeepAspectRatio, // ratio
                                   Qt::SmoothTransformation);
  if (_scaled.isNull()) {
    _scaled.detach();
    return;
  }
  _source.detach();

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << "Scaled to" << _scaled.size();
#endif

  if (setPixmapItem(_scaled))
    p_pixCache.insert("source", _scaled);
}

} // namespace AntiquaCRM
