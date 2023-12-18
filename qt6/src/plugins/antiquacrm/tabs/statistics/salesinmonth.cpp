// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "salesinmonth.h"
#include "statsbarseries.h"

#include <QFontMetricsF>
#include <QSqlQuery>
#include <QToolTip>

MonthBarSet::MonthBarSet(int year, QChart *parent, MonthBarSet::Type type)
    : QBarSet{QString(), parent}, p_year{year} {
  if (type == MonthBarSet::Type::Sales) {
    setLabel(tr("Volume (%1)").arg(year));
  } else {
    setLabel(tr("Count (%1)").arg(year));
  }
  connect(this, SIGNAL(hovered(bool, int)), SLOT(showToolTip(bool, int)));
}

void MonthBarSet::showToolTip(bool b, int i) {
  if (b && p_sales.size() == 12) {
    double _cost = p_sales[(i + 1)];
    if (_cost > 0.00) {
      const QString _money = AntiquaCRM::ATaxCalculator::money(_cost);
      const QString _info = tr("Summary %1 (%2)").arg(_money).arg(p_year);
      QToolTip::showText(QCursor::pos(), _info, nullptr);
      return;
    }
  }
  QToolTip::hideText();
}

void MonthBarSet::setSales(const QMap<int, double> &sales) { p_sales = sales; }

int MonthBarSet::year() const { return p_year; }

SalesInMonth::SalesInMonth(QWidget *parent) : QChartView{parent} {
  AntiquaCRM::ASettings cfg(this);
  p_currency = cfg.value("payment/currency", "§").toString();

  m_chart = new QChart;
  m_chart->setTitle(tr("Compare Sales from this to past years."));
  m_chart->setMargins(QMargins(0, 0, 0, 0));
  m_chart->setAnimationOptions(QChart::SeriesAnimations);

  m_label = new QBarCategoryAxis(m_chart);
  QFont barFont = m_label->labelsFont();
  barFont.setPointSize(m_label->labelsFont().pointSize() - 4);
  m_chart->addAxis(m_label, Qt::AlignBottom);

  m_numsBar = new VerticalBarSeries(this);
  m_numsBar->setBarWidth(0.95);

  m_paidBar = new VerticalBarSeries(this);
  m_paidBar->setBarWidth(0.95);
  m_paidBar->setLabelsFormat(p_currency);

  m_sql = new AntiquaCRM::ASqlCore(this);
  if (initialChartView()) {
    setChart(m_chart);
  } else {
    qWarning("No Sales in Month Charts data");
  }
}

SalesInMonth::~SalesInMonth() {
  if (m_chart != nullptr)
    m_chart->deleteLater();

  // p_voluMap.clear();
}

MonthBarSet *SalesInMonth::createBarset(int year, MonthBarSet::Type type) {
  MonthBarSet *bs = new MonthBarSet(year, m_chart, type);
  bs->setLabelFont(m_label->labelsFont());
  bs->setLabelColor(Qt::black);
  return bs;
}

bool SalesInMonth::initMaps() {
  QString _sql("SELECT DISTINCT");
  _sql.append(" date_part('year', o_delivered)::NUMERIC AS year");
  _sql.append(" FROM inventory_orders WHERE (o_delivered IS NOT NULL)");
  _sql.append(" GROUP BY year ORDER BY year");
  QSqlQuery _q = m_sql->query(_sql);
  if (_q.size() < 1) {
    qWarning("Sales in Month chart, without ranges!");
    return false;
  }

  // Create existing years maps
  while (_q.next()) {
    int _year = _q.value("year").toInt();
    QMap<int, qint64> _vol;
    QMap<int, double> _sel;
    for (int i = 1; i < 12; i++) {
      _vol.insert(i, 0);
      _sel.insert(i, 0.00);
    }
    p_voluMap.insert(_year, _vol);
    p_soldMap.insert(_year, _sel);
  }
  _q.clear();
  return true;
}

const QDateTime SalesInMonth::fsepoch(qint64 t) const {
  return QDateTime::fromSecsSinceEpoch(t, Qt::LocalTime);
}

bool SalesInMonth::initialChartView() {
  QString _query; // query statement
  // init months range map
  if (!initMaps())
    return false;

  // create volume chart data
  _query = AntiquaCRM::ASqlFiles::queryStatement("statistics_payments_month");
  if (_query.isEmpty())
    return false;

  QSqlQuery _q = m_sql->query(_query);
  if (_q.size() < 1)
    return false;

  while (_q.next()) {
    int _c = _q.value("counts").toInt();
    double _s = _q.value("sell").toDouble();
    QDateTime _dt = fsepoch(_q.value("sepoch").toInt());
    bool _b;
    int _y = _dt.toString("yyyy").toInt(&_b);
    if (_b) {
      int _month = _dt.date().month();
      // verkäufe
      QMap<int, qint64> _mv = p_voluMap[_y];
      _mv[_month] = (_mv[_month] + _c);
      p_voluMap[_y] = _mv;
      // preise
      QMap<int, double> _ms = p_soldMap[_y];
      _ms[_month] = (_ms[_month] + _s);
      p_soldMap[_y] = _ms;
    }
  }
  _query.clear();
  _q.clear();

  // finally insert chart data
  foreach (int y, p_voluMap.keys()) {
    QMap<int, qint64> _m = p_voluMap[y];
    MonthBarSet *m_counts = createBarset(y);
    QMap<int, double> _s = p_soldMap[y];
    MonthBarSet *m_solded = createBarset(y, MonthBarSet::Type::Sales);
    m_solded->setSales(_s);
    for (int m = 1; m <= 12; m++) {
      QDate _curr(y, m, 1);
      m_counts->append(_m[m]);
      m_solded->append(_s[m]);
      m_label->insert((m - 1), _curr.toString("MMMM"));
      // qDebug() << _curr.toString("MMMM") << _m[m];
    }
    m_numsBar->insert(0, m_counts);
    m_paidBar->insert(0, m_solded);
  }
  m_chart->addSeries(m_numsBar);
  m_chart->addSeries(m_paidBar);
  // updateHeight();
  return true;
}

void SalesInMonth::updateHeight() {
  QFontMetricsF fm(m_chart->font());
  setMinimumHeight(qRound(fm.height() * (12 * 4)));
}
