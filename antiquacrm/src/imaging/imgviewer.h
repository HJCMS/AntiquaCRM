// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef IMGVIEWER_H
#define IMGVIEWER_H

#include <QtCore/QObject>
#include <QtGui/QImage>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

class ImgViewer : public QLabel {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

public:
  explicit ImgViewer(QWidget *parent = nullptr);
  const QImage image();
};

#endif // IMGVIEWER_H
