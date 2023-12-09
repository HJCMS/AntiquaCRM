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

using namespace QtCharts;

PaymentsInYear::PaymentsInYear(const QDate &date, QWidget *parent)
    : QScrollArea{parent}, p_date(date) {
  setWidgetResizable(true);

  int _year = p_date.year();
  QString str_year = QString::number(_year);

  m_view = new QChartView(this);
  m_view->setRenderHint(QPainter::Antialiasing);
  setWidget(m_view);

  QChart *m_chart = new QChart;
  m_chart->setTitle(tr("Orders in Year %1.").arg(str_year));
  m_chart->legend()->hide();
  m_chart->setAnimationOptions(QChart::NoAnimation);
  setWindowTitle(m_chart->title());
  m_view->setChart(m_chart);

  StatsUtils p_util;
  QDate startDate(_year, 1, 1);
  QDate endDate(_year, 12, 31);
  QMap<qint64, int> axispoints;
  QString sql_query;
  if (_year == QDate::currentDate().year()) {
    // Nur bis Monat
    sql_query.append("o_since BETWEEN '");
    sql_query.append(p_util.startTimeStamp(startDate));
    sql_query.append("' AND '");
    if (p_date.month() == 12) {
      endDate = QDate(p_date.year(), p_date.month(), 31);
    } else {
      endDate = QDate(p_date.year(), p_date.month() + 1, 1);
    }
    sql_query.append(p_util.endTimeStamp(endDate));
    sql_query.append("'");
    axispoints = p_util.dayRangeFromDate(startDate, endDate);
  } else {
    // Vergangenes Jahr
    sql_query.append("date_part('year', o_since) = ");
    sql_query.append(str_year);
    axispoints = p_util.dayRangeFromYear(_year);
  }

  // statistics_payments_month_in_year.sql
  // @SQL_WHERE_CLAUSE@ = Year

  AntiquaCRM::ASqlFiles sqf("statistics_payments_year");
  if (sqf.openTemplate()) {
    sqf.setWhereClause(sql_query);
    qDebug() << Q_FUNC_INFO << sql_query;
    AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
    QSqlQuery q = m_sql->query(sqf.getQueryContent());
    int max_count = 8;
    if (q.size() > 0) {
      while (q.next()) {
        int count = q.value("counts").toInt();
        if (count > max_count)
          max_count = count;

        QDateTime dt = QDateTime::fromSecsSinceEpoch(q.value("sepoch").toInt(),
                                                     Qt::LocalTime);

        axispoints.insert(dt.toMSecsSinceEpoch(), count);
      }
    }

    YearAxis *m_axisYear = new YearAxis(m_view);
    m_axisYear->setStartDate(startDate);
    m_axisYear->setEndDate(endDate);
    m_chart->addAxis(m_axisYear, Qt::AlignBottom);

    CountAxis *m_axisCount = new CountAxis(max_count, m_view);
    m_chart->addAxis(m_axisCount, Qt::AlignLeft);

    QLineSeries *ordered_series = new QLineSeries(m_view);
    ordered_series->setName(tr("Orders"));
    QMapIterator<qint64, int> it(axispoints);
    while (it.hasNext()) {
      it.next();
      ordered_series->append(it.key(), it.value());
    }
    m_chart->addSeries(ordered_series);
    ordered_series->attachAxis(m_axisYear);
    ordered_series->attachAxis(m_axisCount);
  }
}

PaymentsInYear::~PaymentsInYear() {
  if (m_view != nullptr)
    m_view->deleteLater();
}
