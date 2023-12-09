// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PAYMENTSMONTHYEAR_H
#define ANTIQUACRM_PAYMENTSMONTHYEAR_H

#include <AGlobal>
#include <QDate>
#include <QObject>
#include <QScrollArea>
#include <QWidget>
#include <QtCharts>

class AHorizontalBarSeries;

class PaymentsMonthYear final : public QScrollArea {
  Q_OBJECT

private:
  const QDate p_date;
  QtCharts::QChartView *m_view;
  QtCharts::QBarSet *m_lastYear;
  QtCharts::QBarSet *m_currYear;
  QtCharts::QBarCategoryAxis *m_label;
  AHorizontalBarSeries *m_monthyBar;
  bool initialChartView();

public:
  explicit PaymentsMonthYear(const QDate &date, QWidget *parent = nullptr);
  ~PaymentsMonthYear();
};

#endif // ANTIQUACRM_PAYMENTSMONTHYEAR_H
