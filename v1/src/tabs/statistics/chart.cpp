// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "chart.h"

Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags)
    : QtCharts::QChart(QtCharts::QChart::ChartTypeCartesian, parent, wFlags) {
  legend()->hide();
  setAnimationOptions(QtCharts::QChart::NoAnimation);
}

bool Chart::sceneEvent(QEvent *event) {
  // event->type()
  return QChart::event(event);
}
