// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "paymentsinyear.h"

#include <AntiquaCRM>
#include <QCalendar>
#include <QDate>
#include <QDebug>
#include <QMap>
#include <QVBoxLayout>
#include <QtCharts>

using namespace QtCharts;

PaymentsInYear::PaymentsInYear(QWidget *parent) : QWidget{parent} {
  setObjectName("payments_from_year");
  setContentsMargins(2, 2, 2, 2);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  QDate date = QDate::currentDate();

  QChart *m_chart = new QChart;
  QString str_year = QString::number(date.year());
  m_chart->setTitle(tr("Payments for") + " " + str_year);
  m_chart->legend()->hide();

  QDateTimeAxis *axisX = new QDateTimeAxis(this);
  axisX->setFormat("dd-MM-yyyy");
  axisX->setTickCount(12);
  m_chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis(this);
  axisY->setRange(0, 20);
  axisY->setLabelFormat("%d");
  m_chart->addAxis(axisY, Qt::AlignLeft);

  QString sql =
      AntiquaCRM::ASqlFiles::selectStatement("statistics_payments_year");
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QSqlQuery q = m_sql->query(sql);
  QMap<int, int> map;
  if (q.size() > 0) {
    while (q.next()) {
      int doy = q.value("doy").toInt();
      // double current = q.value("spline").toDouble();
      if (map.contains(doy)) {
        map.insert(doy, (map.value(doy) + 1));
      } else {
        map.insert(doy, 1);
      }
    }
  }

  QLineSeries *series = new QLineSeries;
  series->setName("Payments");
  QDate fromDate((date.year() - 1), 12, 31);
  for (int i = 1; i < date.daysInYear(); i++) {
    int c = 0;
    if (map.contains(i))
      c = map[i];

    QDateTime dt;
    dt.setTime(QTime(0, 0, 0));
    dt.setDate(fromDate.addDays(i));
    series->append(dt.toMSecsSinceEpoch(), c);
  }
  m_chart->addSeries(series);
  map.clear();

  QChartView *m_view = new QChartView(m_chart, this);
  m_view->setRenderHint(QPainter::Antialiasing);
  layout->addWidget(m_view);

  setLayout(layout);
}
