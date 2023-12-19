// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "salesfromproviders.h"
#include "soldcountchart.h"
#include "salesvolumechart.h"

SalesFromProviders::SalesFromProviders(QWidget *parent)
    : QSplitter{Qt::Horizontal, parent} {
  setObjectName("providers_chart");
  m_soldCountChart = new SoldCountChart(this);
  addWidget(m_soldCountChart);
  m_salesVolumeChart = new SalesVolumeChart(this);
  addWidget(m_salesVolumeChart);
}

bool SalesFromProviders::initView(int year) {
  if (!m_soldCountChart->initialChartView(year))
    return false;

  if (!m_salesVolumeChart->initialChartView(year))
    return false;

  return true;
}
