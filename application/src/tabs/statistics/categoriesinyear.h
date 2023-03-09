// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CATEGORIES_YEAR_H
#define ANTIQUACRM_CATEGORIES_YEAR_H

#include <AGlobal>
#include <QDate>
#include <QObject>
#include <QScrollArea>
#include <QWidget>
#include <QtCharts/QChartView>

class AHorizontalBarSeries;

/**
 * @brief The CategoriesInYear class
 * @ingroup _chartstat
 */
class CategoriesInYear final : public QScrollArea {
  Q_OBJECT

private:
  const QDate p_date;
  QString currency;
  QtCharts::QChartView *m_view;
  AHorizontalBarSeries *m_doubleSeries;
  AHorizontalBarSeries *m_countSeries;
  double currencyRound(double);

private Q_SLOTS:
  void onHoverTip(bool, int);

public:
  explicit CategoriesInYear(const QDate &date, QWidget *parent = nullptr);
  ~CategoriesInYear();
};

#endif // ANTIQUACRM_CATEGORIES_YEAR_H
