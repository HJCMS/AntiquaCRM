// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "chartsview.h"

#include <QDebug>
#include <QGraphicsView>
#include <QPainter>

ChartsView::ChartsView(QtCharts::QChart *chart, QWidget *parent)
    : QtCharts::QChartView{chart, parent} {
  setRenderHint(QPainter::Antialiasing);
}

void ChartsView::wheelEvent(QWheelEvent *event) {
  QPoint delta(event->angleDelta());
  if (delta.isNull())
    return;

  if (delta.y() > 0)
    chart()->zoomIn();
  else
    chart()->zoomOut();
}

void ChartsView::mousePressEvent(QMouseEvent *event) {

  QChartView::mousePressEvent(event);
}

void ChartsView::mouseMoveEvent(QMouseEvent *event) {
  QChartView::mouseMoveEvent(event);
}

void ChartsView::mouseReleaseEvent(QMouseEvent *event) {
  // chart()->setAnimationOptions(QtCharts::QChart::SeriesAnimations);
  QChartView::mouseReleaseEvent(event);
}

void ChartsView::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
  case Qt::Key_0:
    chart()->zoomReset();
    break;

  case Qt::Key_Plus:
    chart()->zoomIn();
    break;

  case Qt::Key_Minus:
    chart()->zoomOut();
    break;

  case Qt::Key_Left:
    chart()->scroll(-10, 0);
    break;

  case Qt::Key_Right:
    chart()->scroll(10, 0);
    break;

  case Qt::Key_Up:
    chart()->scroll(0, 10);
    break;

  case Qt::Key_Down:
    chart()->scroll(0, -10);
    break;

  default:
    QGraphicsView::keyPressEvent(event);
    break;
  }
}
