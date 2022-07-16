// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "draglabel.h"
#include "myicontheme.h"

#include <QDebug>
#include <QFontMetrics>
#include <QImage>
#include <QPainter>
#include <QPalette>
#include <QSize>

DragLabel::DragLabel(const QString &text, QWidget *parent) : QLabel{parent} {
  QFontMetrics metric(font());
  QSize size = metric.size(Qt::TextSingleLine, text);
  QImage image(size.width() + 8, size.height() + 4,
               QImage::Format_ARGB32_Premultiplied);
  image.fill(palette().color(QPalette::Window).rgba());

  QPainter painter;
  painter.begin(&image);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setBrush(Qt::white);
  painter.drawRect(QRectF(0, 0, image.width() - 1, image.height() - 1));
  painter.setFont(font());
  painter.setBrush(Qt::black);
  painter.drawText(QRect(QPoint(4, 2), size), Qt::AlignCenter, text);
  painter.end();

  setPixmap(QPixmap::fromImage(image));
}
