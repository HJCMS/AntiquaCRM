// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "paymentsinyear.h"
#include "countaxis.h"
#include "statsutils.h"
#include "yearaxis.h"

#include <AntiquaCRM>
#include <QDate>
#include <QMap>
#include <QPainter>
#include <QVBoxLayout>
#include <QtCharts>

PaymentsInYear::PaymentsInYear(const QDate &date, QWidget *parent)
    : QWidget{parent}, p_date(date) {
  setObjectName("payments_from_year");
  setContentsMargins(2, 2, 2, 2);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  int _year = p_date.year();
  QMap<qint64, int> counts = StatsUtils::daysInYearRange(_year);
  QString str_year = QString::number(_year);
  AntiquaCRM::ASqlFiles sqf("statistics_payments_year");
  if (!sqf.openTemplate())
    return;

  sqf.setWhereClause(str_year);
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QSqlQuery q = m_sql->query(sqf.getQueryContent());
  QDate startDate;
  int max_count = 8;
  if (q.size() > 0) {
    while (q.next()) {
      int count = q.value("amount").toInt();
      if (count > max_count)
        max_count = count;

      QDateTime dt = QDateTime::fromSecsSinceEpoch(q.value("sepoch").toInt(),
                                                   Qt::LocalTime);
      if (!startDate.isValid())
        startDate = dt.date().addDays(-1);

      counts.insert(dt.toMSecsSinceEpoch(), count);
    }
  }

  m_axisYear = new YearAxis(this);
  m_axisYear->setStartDate(startDate);
  m_axisYear->setEndDate(QDate(_year, 12, 31));
  m_axisCount = new CountAxis(max_count, this);

  QtCharts::QChart *m_chart = new QtCharts::QChart;
  m_chart->setTitle(tr("Orders in Year %1.").arg(str_year));
  m_chart->legend()->hide();
  m_chart->setAnimationOptions(QtCharts::QChart::NoAnimation);
  m_chart->addAxis(m_axisYear, Qt::AlignBottom);
  m_chart->addAxis(m_axisCount, Qt::AlignLeft);

  QtCharts::QLineSeries *ordered_series = new QtCharts::QLineSeries;
  ordered_series->setName(tr("Orders"));
  QMapIterator<qint64, int> it(counts);
  while (it.hasNext()) {
    it.next();
    ordered_series->append(it.key(), it.value());
  }
  m_chart->addSeries(ordered_series);
  ordered_series->attachAxis(m_axisYear);
  ordered_series->attachAxis(m_axisCount);

  QtCharts::QChartView *m_view = new QtCharts::QChartView(m_chart, this);
  m_view->setRenderHint(QPainter::Antialiasing);
  layout->addWidget(m_view);

  setLayout(layout);
}
