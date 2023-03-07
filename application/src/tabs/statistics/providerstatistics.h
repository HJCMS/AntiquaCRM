// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDERSTATISTICS_H
#define ANTIQUACRM_PROVIDERSTATISTICS_H

#include <QDate>
#include <QObject>
#include <QScrollArea>
#include <QWidget>
#include <QtCharts/QChartView>

/**
 * @brief The ProviderStatistics class
 * @ingroup AntiquaStatistics
 */
class ProviderStatistics : public QScrollArea {
  Q_OBJECT

private:
  const QDate p_date;
  QString str_year;
  QtCharts::QChartView *m_chartView;
  void setOrdersChart();

private Q_SLOTS:
  void highlight(bool);

public:
  explicit ProviderStatistics(const QDate &date, QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_PROVIDERSTATISTICS_H
