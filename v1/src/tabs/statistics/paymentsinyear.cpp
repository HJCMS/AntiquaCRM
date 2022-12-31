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

  QMap<qint64, int> counts = StatsUtils::daysInYearRange(_year);
  AntiquaCRM::ASqlFiles sqf("statistics_payments_year");
  if (sqf.openTemplate()) {
    sqf.setWhereClause(str_year);
    AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
    QSqlQuery q = m_sql->query(sqf.getQueryContent());
    QDate startDate;
    int max_count = 8;
    if (q.size() > 0) {
      while (q.next()) {
        int count = q.value("counts").toInt();
        if (count > max_count)
          max_count = count;

        QDateTime dt = QDateTime::fromSecsSinceEpoch(q.value("sepoch").toInt(),
                                                     Qt::LocalTime);
        if (!startDate.isValid())
          startDate = dt.date().addDays(-1);

        counts.insert(dt.toMSecsSinceEpoch(), count);
      }
    }

    YearAxis *m_axisYear = new YearAxis(m_view);
    m_axisYear->setStartDate(startDate);
    m_axisYear->setEndDate(QDate(_year, 12, 31));
    m_chart->addAxis(m_axisYear, Qt::AlignBottom);

    CountAxis *m_axisCount = new CountAxis(max_count, m_view);
    m_chart->addAxis(m_axisCount, Qt::AlignLeft);

    QLineSeries *ordered_series = new QLineSeries(m_view);
    ordered_series->setName(tr("Orders"));
    QMapIterator<qint64, int> it(counts);
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
