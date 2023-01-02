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
#include <QtCharts/QHorizontalBarSeries>

class CategoriesInYear final : public QScrollArea {
  Q_OBJECT

private:
  const QDate p_date;
  QString currency;
  QtCharts::QChartView *m_view;
  QtCharts::QHorizontalBarSeries *m_DoubleSeries;
  QtCharts::QHorizontalBarSeries *m_CountSeries;

private Q_SLOTS:
  void onHoverTip(bool, int);

public:
  explicit CategoriesInYear(const QDate &date, QWidget *parent = nullptr);
  ~CategoriesInYear();
};

#endif // ANTIQUACRM_CATEGORIES_YEAR_H
