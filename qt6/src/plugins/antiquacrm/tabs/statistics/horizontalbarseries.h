// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_HORIZONTAL_BARSERIES_H
#define ANTIQUACRM_HORIZONTAL_BARSERIES_H

#include <AGlobal>
#include <QAbstractSeries>
#include <QHorizontalBarSeries>
#include <QObject>
#include <QWidget>

/**
 * @brief The AHorizontalBarSeries class
 * @ingroup _chartstat
 */
class ANTIQUACRM_LIBRARY HorizontalBarSeries final
    : public QHorizontalBarSeries {
  Q_OBJECT

public:
  explicit HorizontalBarSeries(QObject *parent = nullptr);
  QAbstractSeries::SeriesType type() const override;
  ~HorizontalBarSeries();
};

#endif // ANTIQUACRM_HORIZONTAL_BARSERIES_H
