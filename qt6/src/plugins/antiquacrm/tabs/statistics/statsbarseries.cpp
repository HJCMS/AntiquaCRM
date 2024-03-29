// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statsbarseries.h"

HorizontalBarSeries::HorizontalBarSeries(QObject *parent)
    : QHorizontalBarSeries{parent} {
  setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
  setLabelsVisible(true);
  setLabelsFormat("@value");
  setBarWidth(1.0);
  setVisible(true);
}

QAbstractSeries::SeriesType HorizontalBarSeries::type() const {
  return QAbstractSeries::SeriesTypeHorizontalBar;
}

VerticalBarSeries::VerticalBarSeries(QObject *parent) : QBarSeries{parent} {
  setLabelsPosition(QAbstractBarSeries::LabelsInsideEnd);
  setLabelsVisible(true);
  setLabelsFormat("@value");
  setBarWidth(1.0);
  setVisible(true);
}

QAbstractSeries::SeriesType VerticalBarSeries::type() const {
  return QAbstractSeries::SeriesTypeBar;
}
