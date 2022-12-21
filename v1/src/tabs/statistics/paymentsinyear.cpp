// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "paymentsinyear.h"

#include <AntiquaCRM>
#include <QDate>
#include <QDebug>
#include <QMap>
#include <QVBoxLayout>
#include <QtCharts>

using namespace QtCharts;

PaymentsInYear::PaymentsInYear(const QDate &date, QWidget *parent)
    : QWidget{parent}, p_date(date) {
  setObjectName("payments_from_year");
  setContentsMargins(2, 2, 2, 2);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  int _year = p_date.year();
  QString str_year = QString::number(_year);
  AntiquaCRM::ASqlFiles sqf("statistics_payments_year");
  if (!sqf.openTemplate())
    return;

  sqf.setWhereClause(str_year);
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QSqlQuery q = m_sql->query(sqf.getQueryContent());
  QMap<qint64, int> map;
  QDateTime fdt; // from DateTime
  if (q.size() > 0) {
    while (q.next()) {
      QDateTime dt;
      dt.setSecsSinceEpoch(q.value("since").toInt());
      qint64 epoch = dt.toMSecsSinceEpoch();
      if (!fdt.isValid())
        fdt.setMSecsSinceEpoch(epoch);

      if (map.contains(epoch))
        map.insert(epoch, (map[epoch] + 1));
      else
        map.insert(epoch, 1);
    }
  }

  QChart *m_chart = new QChart;
  m_chart->setTitle(tr("Completed purchases for %1.").arg(str_year));
  m_chart->legend()->hide();

  QLineSeries *series = new QLineSeries;
  series->setName("Payments");
  int counts = 8;
  QMapIterator<qint64, int> it(map);
  while (it.hasNext()) {
    it.next();
    if (it.value() > counts)
      counts = it.value();

    series->append(it.key(), it.value());
  }
  m_chart->addSeries(series);

  QDateTime from_dt(fdt.date().addDays(-1), QTime(0, 1));
  QDateTime to_dt(QDate(_year, 12, 31), QTime(23, 59));

  QDateTimeAxis *axisX = new QDateTimeAxis(this);
  axisX->setFormat("dd.MM.yy");
  axisX->setRange(from_dt, to_dt);
  axisX->setTickCount(12);
  m_chart->addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);

  QValueAxis *axisY = new QValueAxis(this);
  axisY->setRange(0, ((counts % 2 == 0) ? counts : (counts + 1)));
  axisY->setLabelFormat("%d");
  m_chart->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);

  map.clear();

  QChartView *m_view = new QChartView(m_chart, this);
  m_view->setRenderHint(QPainter::Antialiasing);
  layout->addWidget(m_view);

  setLayout(layout);
}
