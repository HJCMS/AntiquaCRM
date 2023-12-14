// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "salesinmonth.h"
#include "horizontalbarseries.h"

#include <AntiquaCRM>
#include <QSqlQuery>

SalesInMonth::SalesInMonth(QWidget *parent) : QChartView{parent} {
  m_chart = new QChart;
  m_chart->setTitle(tr("Compare Sales from this to past years."));
  m_chart->setMargins(QMargins(0, 0, 0, 0));
  m_chart->setAnimationOptions(QChart::SeriesAnimations);

  m_label = new QBarCategoryAxis(m_chart);
  QFont barFont = m_label->labelsFont();
  barFont.setPointSize(m_label->labelsFont().pointSize() - 4);

  m_monthBar = new HorizontalBarSeries(this);
  m_monthBar->setBarWidth(0.95);

  if (initialChartView()) {
    m_chart->addSeries(m_monthBar);
    // Vertikale Achse
    m_chart->addAxis(m_label, Qt::AlignLeft);
    m_monthBar->attachAxis(m_label);
    setChart(m_chart);
  } else {
    qWarning("No Sales in Month Charts data");
  }
}

SalesInMonth::~SalesInMonth() {
  if (m_chart != nullptr)
    m_chart->deleteLater();
}

QBarSet *SalesInMonth::createBarset(int year) {
  const QString _title(QString::number(year));
  QBarSet *bs = new QBarSet(_title, m_chart);
  bs->setObjectName("average_curr_year");
  bs->setLabelFont(m_label->labelsFont());
  bs->setLabelColor(Qt::black);
  return bs;
}

bool SalesInMonth::initialChartView() {
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QString _sql("SELECT DISTINCT");
  _sql.append(" date_part('year', o_delivered)::NUMERIC AS year");
  _sql.append(" FROM inventory_orders WHERE (o_delivered IS NOT NULL)");
  _sql.append(" GROUP BY year ORDER BY year");
  QSqlQuery _qy = m_sql->query(_sql);
  if (_qy.size() < 1)
    return false;

  // Create existing years maps
  QMap<int, QMap<int, qint64>> _map;
  while (_qy.next()) {
    int _year = _qy.value("year").toInt();
    QMap<int, qint64> _sub;
    for (int i = 1; i < 12; i++) {
      _sub.insert(i, 0);
    }
    _map.insert(_year, _sub);
  }
  _qy.clear();

  _sql = AntiquaCRM::ASqlFiles::queryStatement(
      "statistics_payments_month_in_year");
  if (!_sql.isEmpty()) {
    QSqlQuery q = m_sql->query(_sql);
    if (q.size() > 0) {
      while (q.next()) {
        int count = q.value("counts").toInt();
        QDateTime dt = QDateTime::fromSecsSinceEpoch(q.value("sepoch").toInt(),
                                                     Qt::LocalTime);

        bool b;
        int _y = dt.toString("yyyy").toInt(&b);
        if (b) {
          QMap<int, qint64> _m = _map[_y];
          int _month = dt.date().month();
          _m[_month] = (_m[_month] + count);
          _map[_y] = _m;
        }
      }
    } else {
      return false;
    }
    q.clear();
  } else {
    return false;
  }

  foreach (int y, _map.keys()) {
    QMap<int, qint64> _m = _map[y];
    QBarSet *m_set = createBarset(y);
    for (int m = 1; m <= 12; m++) {
      QDate _curr(y, m, 1);
      m_set->append(_m[m]);
      m_label->insert((m - 1), _curr.toString("MMMM"));
      // qDebug() << _curr.toString("MMMM") << _m[m];
    }
    m_monthBar->insert(0, m_set);
  }

  _map.clear();
  return true;
}
