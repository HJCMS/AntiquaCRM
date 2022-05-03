// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imageview.h"

#include <QDebug>
#include <QImageReader>
#include <QPixmap>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>

ImageView::ImageView(QWidget *parent) : QGraphicsView(parent) {
  setObjectName("ImagePreview");
  setBackgroundRole(QPalette::Base);
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  setCacheMode(QGraphicsView::CacheBackground);
  m_scene = new QGraphicsScene(this);
}

void ImageView::setImage(const QString &file) {
  QImageReader reader(file);
  reader.setAutoTransform(true);
  QRect r = rect();
  QImage img =
      reader.read().scaled(QSize(r.width() - 2, r.height() - 2),
                           Qt::KeepAspectRatio, Qt::FastTransformation);
  setImage(img);
}

void ImageView::setImage(const QImage &img) {
  p_pixmap = QPixmap::fromImage(img);
  if (!p_pixmap.isNull()) {
    m_scene->clear();
    m_pixmap = m_scene->addPixmap(p_pixmap);
    setScene(m_scene);
  }
}

const QImage ImageView::image() {
  if (m_pixmap == nullptr)
    return QImage();

  if (m_pixmap->pixmap().isNull())
    return QImage();

  return m_pixmap->pixmap().toImage();
}
