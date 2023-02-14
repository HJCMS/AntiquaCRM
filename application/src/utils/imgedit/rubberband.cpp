// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "rubberband.h"

#include <QColor>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QStyle>
#include <QStylePainter>

RubberBand::RubberBand(QGraphicsView *parent)
    : QRubberBand{QRubberBand::Rectangle, parent} {
  setAttribute(Qt::WA_TransparentForMouseEvents);
  setAttribute(Qt::WA_NoSystemBackground);
  setAttribute(Qt::WA_WState_ExplicitShowHide);
  setVisible(false);
}

const QStyleOptionRubberBand RubberBand::styleOption() {
  QStyleOptionRubberBand option;
  option.shape = QRubberBand::Rectangle;
  option.opaque = true;
  return option;
}

const QPen RubberBand::pen() {
  QPen p(Qt::DashLine);
  p.setWidth(1);
  p.setColor(Qt::red);
  return p;
}

void RubberBand::paintEvent(QPaintEvent *event) {
  QRect cp = event->rect();
  QRectF rectF(QPointF(cp.topLeft()), QPointF(cp.bottomRight()));
  QStylePainter sp(this);
  sp.setBackgroundMode(Qt::TransparentMode);
  sp.setRenderHint(QPainter::Antialiasing);
  sp.setPen(pen());
  sp.drawRect(rectF);
  sp.drawControl(QStyle::CE_RubberBand, styleOption());
}

bool RubberBand::scalability() {
  if (size().width() & 1)
    return false;

  if (size().height() & 1)
    return false;

  return true;
}
