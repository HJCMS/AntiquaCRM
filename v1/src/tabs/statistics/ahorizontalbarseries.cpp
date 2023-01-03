// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ahorizontalbarseries.h"

#include <QtCharts>

using namespace QtCharts;

AHorizontalBarSeries::AHorizontalBarSeries(QObject *parent)
    : QtCharts::QHorizontalBarSeries{parent} {
  setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
  setLabelsVisible(true);
  setLabelsFormat("@value");
  setBarWidth(1.0);
}

QAbstractSeries::SeriesType AHorizontalBarSeries::type() const {
  return QAbstractSeries::SeriesTypeHorizontalBar;
}

AHorizontalBarSeries::~AHorizontalBarSeries() {}
