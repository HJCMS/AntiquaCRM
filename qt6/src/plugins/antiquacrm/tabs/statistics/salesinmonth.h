// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_SALESINMONTH_H
#define ANTIQUACRM_STATISTICS_SALESINMONTH_H

#include "statisticsconfig.h"
#include <AChartView>
#include <AntiquaCRM>
#include <QPointF>
#include <QCategoryAxis>
#include <QDate>
#include <QLineSeries>
#include <QLocale>
#include <QMap>
#include <QObject>
#include <QWidget>

/**
 * @brief The SalesInMonthSeries class
 * @ingroup _chartstat
 */
class ANTIQUACRM_STATISTICS_PLUGIN MonthSeries final : public QLineSeries {
  Q_OBJECT

private:
  qint64 Year;
  QMap<qint16, qint64> MonthData;

private Q_SLOTS:
  void toolTip(const QPointF &, bool);

public Q_SLOTS:
  void updatePointLabels();

public:
  explicit MonthSeries(qint64 year, QMap<qint16, qint64> map, QObject* parent = nullptr);
  bool setPoints();
  QAbstractSeries::SeriesType type() const override;
};

class ANTIQUACRM_STATISTICS_PLUGIN SalesInMonth final : public AntiquaCRM::AChartView {
  Q_OBJECT

private:
  const QLocale p_lc;
  const QDate p_date;
  /**
   * @code
   *  QMap<YEARS, QMap<MONTHS, VOLUME_COUNT>>
   * @endcode
   */
  mutable QMap<qint64, QMap<qint16, qint64>> p_dataMap;
  QChart* m_chart;
  QCategoryAxis* m_monthsAxis;
  QValueAxis* m_valueAxis;
  bool initMaps();
  bool initialChartView(int year = -1) override;

public:
  explicit SalesInMonth(QWidget* parent = nullptr);
  const QList<MonthSeries*> series();
  virtual ~SalesInMonth();
};

#endif // ANTIQUACRM_STATISTICS_SALESINMONTH_H
