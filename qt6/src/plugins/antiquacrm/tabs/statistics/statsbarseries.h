// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_STATISTICS_STATSBARSERIES_H
#define ANTIQUACRM_STATISTICS_STATSBARSERIES_H

#include "statisticsconfig.h"
#include <AGlobal>
#include <QAbstractSeries>
#include <QBarSeries>
#include <QHorizontalBarSeries>
#include <QObject>
#include <QWidget>

/**
 * @brief The HorizontalBarSeries class
 * @ingroup _chartstat
 */
class ANTIQUACRM_STATISTICS_PLUGIN HorizontalBarSeries final : public QHorizontalBarSeries {
  Q_OBJECT

public:
  explicit HorizontalBarSeries(QObject* parent = nullptr);
  QAbstractSeries::SeriesType type() const override;
};

/**
 * @brief The VerticalBarSeries class
 * @ingroup _chartstat
 */
class ANTIQUACRM_STATISTICS_PLUGIN VerticalBarSeries final : public QBarSeries {
  Q_OBJECT

public:
  explicit VerticalBarSeries(QObject* parent = nullptr);
  QAbstractSeries::SeriesType type() const override;
};

#endif // ANTIQUACRM_STATISTICS_STATSBARSERIES_H
