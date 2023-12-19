// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "dailyaverage.h"
#include "countaxis.h"
#include "statsutils.h"
#include "yearaxis.h"

#include <QLineSeries>

DailyAverage::DailyAverage(QWidget *parent)
    : AntiquaCRM::AChartView{parent}, p_date{QDate::currentDate()} {
  setObjectName("daily_average_chart");

  m_chart = new QChart(itemAt(0, 0));
  m_chart->legend()->hide();
  m_chart->setTitleFont(headersFont);
  m_chart->setAnimationOptions(QChart::NoAnimation);
}

bool DailyAverage::initialChartView(int year) {
  if (year < 2000)
    return false;

  const QString strYear(QString::number(year));
  AntiquaCRM::ASqlFiles _tpl("statistics_payments_year");
  if (!_tpl.openTemplate())
    return false;

  const QString _query("date_part('year', o_since)=" + strYear);
  _tpl.setWhereClause(_query);
  QSqlQuery _q = getSqlQuery(_tpl.getQueryContent());
  int _max = 8;
  if (_q.size() < 1)
    return false;

  StatsUtils p_util;
  QMap<qint64, int> _points = p_util.dayRangeFromYear(year);

  while (_q.next()) {
    int _c = _q.value("counts").toInt();
    if (_c > _max)
      _max = _c;

    const QDateTime _dt = getEpoch(_q.value("sepoch").toInt());
    _points.insert(_dt.toMSecsSinceEpoch(), _c);
  }

  YearAxis *m_axisYear = new YearAxis(year, this);
  m_chart->addAxis(m_axisYear, Qt::AlignBottom);

  CountAxis *m_axisCount = new CountAxis(_max, this);
  m_chart->addAxis(m_axisCount, Qt::AlignLeft);

  QLineSeries *_series = new QLineSeries(this);
  _series->setName(tr("Orders"));

  QMapIterator<qint64, int> it(_points);
  while (it.hasNext()) {
    it.next();
    _series->append(it.key(), it.value());
  }
  m_chart->addSeries(_series);
  _series->attachAxis(m_axisYear);
  _series->attachAxis(m_axisCount);

  setChart(m_chart);
  return true;
}
