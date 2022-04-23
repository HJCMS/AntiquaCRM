// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef IMAGEVIEW_IMAGING_H
#define IMAGEVIEW_IMAGING_H

#include <QtCore/QObject>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QWidget>

class ImageView : public QGraphicsView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QPixmap p_pixmap;
  QGraphicsScene *m_scene;
  QGraphicsPixmapItem *m_pixmap;

public Q_SLOTS:
  void setImage(const QString &);
  void setImage(const QImage &);

public:
  ImageView(QWidget *parent = nullptr);
  const QImage image();
};

#endif // IMAGEVIEW_IMAGING_H
