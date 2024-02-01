// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PAYMENTSMONTHYEAR_H
#define ANTIQUACRM_PAYMENTSMONTHYEAR_H

#include <AntiquaCRM>
#include <QDateTime>
#include <QMap>
#include <QObject>
#include <QScrollArea>
#include <QWidget>
#include <QtCharts>

class AHorizontalBarSeries;

class MonthBarSet final : public QtCharts::QBarSet {
  Q_OBJECT

private:
  int p_year;
  QMap<int, double> p_sales;

private Q_SLOTS:
  void showToolTip(bool, int);

public Q_SLOTS:
  void setSales(const QMap<int, double> &sales);

public:
  explicit MonthBarSet(int year, QtCharts::QChart *parent = nullptr);
  int year() const;
};

class PaymentsMonthYear final : public QScrollArea {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;
  QString p_currency;
  mutable QMap<int, QMap<int, qint64>> p_voluMap;
  mutable QMap<int, QMap<int, double>> p_soldMap;
  const QDate p_date;
  QtCharts::QChartView *m_view;
  QtCharts::QChart *m_chart;
  QtCharts::QBarCategoryAxis *m_label;
  AHorizontalBarSeries *m_barSeries;
  void setMiniViewHeight(qreal);
  bool initMaps();
  inline const QDateTime fsepoch(qint64) const;
  MonthBarSet *createBarset(int);
  bool initialChartView();

public:
  explicit PaymentsMonthYear(const QDate &date, QWidget *parent = nullptr);
  ~PaymentsMonthYear();
};

#endif // ANTIQUACRM_PAYMENTSMONTHYEAR_H
