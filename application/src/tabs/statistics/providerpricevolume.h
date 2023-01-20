// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PROVIDERPRICEVOLUME_H
#define ANTIQUACRM_PROVIDERPRICEVOLUME_H

#include <QDate>
#include <QObject>
#include <QScrollArea>
#include <QWidget>
#include <QtCharts/QChartView>

class ProviderPriceVolume final : public QScrollArea {
  Q_OBJECT

private:
  QDate p_date;
  QString str_year;
  QtCharts::QChartView *m_chartView;
  void setVolumeChart();

private Q_SLOTS:
  void highlight(bool);

public:
  explicit ProviderPriceVolume(const QDate &date, QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_PROVIDERPRICEVOLUME_H
