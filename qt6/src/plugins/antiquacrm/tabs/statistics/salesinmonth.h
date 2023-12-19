// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_SALESINMONTH_H
#define ANTIQUACRM_STATISTICS_SALESINMONTH_H

#include <AntiquaCRM>
#include <AntiquaStatistics>
#include <QBarCategoryAxis>
#include <QBarSet>
#include <QMap>
#include <QObject>
#include <QWidget>

class VerticalBarSeries;
class MonthBarSet;

class ANTIQUACRM_LIBRARY SalesInMonth final : public AntiquaCRM::AChartView {
  Q_OBJECT

private:
  mutable QMap<int, QMap<int, qint64>> p_voluMap;
  mutable QMap<int, QMap<int, double>> p_soldMap;
  QChart *m_chart;
  QBarCategoryAxis *m_label;
  VerticalBarSeries *m_numsBar;
  VerticalBarSeries *m_paidBar;
  bool initMaps();
  MonthBarSet *createBarset(int year, int type = 0);
  bool initialChartView(int year = -1) override;

public:
  explicit SalesInMonth(QWidget *parent = nullptr);
  virtual ~SalesInMonth();
};

#endif // ANTIQUACRM_STATISTICS_SALESINMONTH_H
