// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_HORIZONTAL_BARSERIES_H
#define ANTIQUACRM_HORIZONTAL_BARSERIES_H

#include <QObject>
#include <QWidget>
#include <QtCharts/QHorizontalBarSeries>

class AHorizontalBarSeries : public QtCharts::QHorizontalBarSeries {
  Q_OBJECT

public:
  explicit AHorizontalBarSeries(QObject *parent = nullptr);
  QAbstractSeries::SeriesType type() const override;
  ~AHorizontalBarSeries();
};

#endif // ANTIQUACRM_HORIZONTAL_BARSERIES_H