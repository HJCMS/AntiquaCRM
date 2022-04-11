// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "imgviewer.h"
#include "version.h"

#include <QtCore/QDebug>

ImgViewer::ImgViewer(QWidget *parent) : QLabel{parent} {
  setObjectName("ImgViewer");
  setBackgroundRole(QPalette::Base);
  setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  setScaledContents(true);
}

const QImage ImgViewer::image() {
  QPixmap pixmap(QLabel::pixmap(Qt::ReturnByValue));
  return pixmap.toImage();
}
