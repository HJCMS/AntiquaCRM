// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_SALESINMONTH_H
#define ANTIQUACRM_STATISTICS_SALESINMONTH_H

#include <AGlobal>
#include <QBarCategoryAxis>
#include <QBarSet>
#include <QChartView>
#include <QObject>
#include <QWidget>

class HorizontalBarSeries;

class ANTIQUACRM_LIBRARY SalesInMonth final : public QChartView {
  Q_OBJECT

private:
  QChart *m_chart;
  QBarCategoryAxis *m_label;
  HorizontalBarSeries *m_monthBar;
  QBarSet *createBarset(int year);
  bool initialChartView();

public:
  explicit SalesInMonth(QWidget *parent = nullptr);
  virtual ~SalesInMonth();
};

#endif // ANTIQUACRM_STATISTICS_SALESINMONTH_H
