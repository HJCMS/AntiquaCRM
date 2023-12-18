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
#include <QBarCategoryAxis>
#include <QBarSet>
#include <QChartView>
#include <QDateTime>
#include <QMap>
#include <QObject>
#include <QWidget>

class VerticalBarSeries;

class ANTIQUACRM_LIBRARY MonthBarSet final : public QBarSet {
  Q_OBJECT;

private:
  int p_year;
  QMap<int, double> p_sales;

private Q_SLOTS:
  void showToolTip(bool, int);

public Q_SLOTS:
  void setSales(const QMap<int, double> &sales);

public:
  enum Type { Volume = 0, Sales = 1 };
  explicit MonthBarSet(int year, QChart *parent = nullptr,
                       MonthBarSet::Type type = MonthBarSet::Type::Volume);
  int year() const;
};

class ANTIQUACRM_LIBRARY SalesInMonth final : public QChartView {
  Q_OBJECT

private:
  QString p_currency;
  mutable QMap<int, QMap<int, qint64>> p_voluMap;
  mutable QMap<int, QMap<int, double>> p_soldMap;
  AntiquaCRM::ASqlCore *m_sql;
  QChart *m_chart;
  QBarCategoryAxis *m_label;
  VerticalBarSeries *m_numsBar;
  VerticalBarSeries *m_paidBar;
  bool initMaps();
  MonthBarSet *createBarset(int year,
                            MonthBarSet::Type type = MonthBarSet::Type::Volume);
  inline const QDateTime fsepoch(qint64) const;
  bool initialChartView();

private Q_SLOTS:
  void updateHeight();

public:
  explicit SalesInMonth(QWidget *parent = nullptr);
  virtual ~SalesInMonth();
};

#endif // ANTIQUACRM_STATISTICS_SALESINMONTH_H
