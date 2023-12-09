// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "paymentsmonthyear.h"
#include "ahorizontalbarseries.h"

#include <AntiquaCRM>
#include <QFontMetricsF>
#include <QMap>
#include <QMargins>
#include <QVBoxLayout>
#include <cmath>

PaymentsMonthYear::PaymentsMonthYear(const QDate &date, QWidget *parent)
    : QScrollArea{parent}, p_date(date) {
  setWidgetResizable(true);
  setWindowTitle(tr("Sales in Month"));

  m_view = new QChartView(this);
  m_view->setRenderHint(QPainter::Antialiasing);
  setWidget(m_view);

  QtCharts::QChart *m_chart = new QtCharts::QChart;
  m_chart->setTitle(tr("Compare Sales from last and this year."));
  m_chart->setMargins(QMargins(0, 0, 0, 0));
  m_chart->setAnimationOptions(QChart::SeriesAnimations);

  m_label = new QBarCategoryAxis(m_chart);
  QFont barFont = m_label->labelsFont();
  barFont.setPointSize(m_label->labelsFont().pointSize() - 4);

  m_monthyBar = new AHorizontalBarSeries(this);
  m_monthyBar->setBarWidth(0.95);

  m_currYear = new QtCharts::QBarSet(tr("Current Year"), m_chart);
  m_currYear->setObjectName("average_curr_year");
  m_currYear->setLabelFont(barFont);
  m_currYear->setLabelColor(Qt::black);

  m_lastYear = new QtCharts::QBarSet(tr("Last Year"), m_chart);
  m_lastYear->setObjectName("average_last_year");
  m_lastYear->setLabelFont(barFont);
  m_lastYear->setLabelColor(Qt::black);

  if (initialChartView()) {
    m_chart->addSeries(m_monthyBar);
    // Vertikale Achse
    m_chart->addAxis(m_label, Qt::AlignLeft);
    m_monthyBar->attachAxis(m_label);
    m_view->setChart(m_chart);
  }
}

bool PaymentsMonthYear::initialChartView() {
  QMap<int, qint64> _pastMap;
  QMap<int, qint64> _currMap;
  for (int i = 1; i < 12; i++) {
    _pastMap.insert(i, 0);
    _currMap.insert(i, 0);
  }

  const QString _curYear(QString::number(p_date.year()));
  AntiquaCRM::ASqlFiles sqf("statistics_payments_month_in_year");
  if (sqf.openTemplate()) {
    sqf.setWhereClause(_curYear); // @SQL_WHERE_CLAUSE@ = Year
    AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
    QSqlQuery q = m_sql->query(sqf.getQueryContent());
    if (q.size() > 0) {
      while (q.next()) {
        int count = q.value("counts").toInt();
        QDateTime dt = QDateTime::fromSecsSinceEpoch(q.value("sepoch").toInt(),
                                                     Qt::LocalTime);

        const QString _key(dt.toString("yyyy-MM"));
        int _month = dt.date().month();
        if (_key.startsWith(_curYear)) {
          _currMap[_month] = (_currMap[_month] + count);
        } else {
          _pastMap[_month] = (_pastMap[_month] + count);
        }
      }
    }
    q.clear();
  } else {
    qWarning("No Charts data");
    return false;
  }

  for (int m = 1; m <= 12; m++) {
    QDate _curr(p_date.year(), m, 1);
    m_currYear->append(_currMap[m]);
    m_lastYear->append(_pastMap[m]);
    m_label->insert((m - 1), _curr.toString("MMMM"));
    // qDebug() << _curr.toString("MMMM") << _currMap[m] << _pastMap[m];
  }

  m_monthyBar->insert(0, m_currYear);
  m_monthyBar->insert(1, m_lastYear);
  _currMap.clear();
  _pastMap.clear();
  return true;
}

PaymentsMonthYear::~PaymentsMonthYear() {
  if (m_view != nullptr)
    m_view->deleteLater();
}
