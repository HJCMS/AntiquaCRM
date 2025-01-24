// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "salesinmonth.h"
#include "monthbarset.h"
#include "statsbarseries.h"

#include <QChar>
#include <QFontMetricsF>
#include <QLocale>
#include <QSqlQuery>

SalesInMonth::SalesInMonth(QWidget* parent)
    : AntiquaCRM::AChartView{parent}, p_date{QDate::currentDate()} {
  setObjectName("statistics_sales_in_month");
  m_chart = new QChart(itemAt(0, 0));
  m_chart->setTitleFont(headersFont);
  m_chart->setTitle(tr("Compare sales from past years with current."));
  m_chart->setMargins(QMargins(0, 0, 0, 0));
  m_chart->setAnimationOptions(QChart::SeriesAnimations);

  m_label = new QBarCategoryAxis(m_chart);
  m_label->setLabelsFont(labelsFont);
  m_chart->addAxis(m_label, Qt::AlignBottom);

  m_numsBar = new VerticalBarSeries(this);
  m_numsBar->setBarWidth(0.95);

  m_paidBar = new VerticalBarSeries(this);
  m_paidBar->setBarWidth(0.95);
  m_paidBar->setLabelsFormat(currency);

  if (initialChartView()) {
    setChart(m_chart);
  } else {
    qWarning("No Sales in Month Charts data");
  }
}

SalesInMonth::~SalesInMonth() {
  if (m_chart != nullptr)
    m_chart->deleteLater();
}

MonthBarSet* SalesInMonth::createBarset(int year, int type) {
  MonthBarSet::Type _t = static_cast<MonthBarSet::Type>(type);
  MonthBarSet* bs = new MonthBarSet(year, m_chart, _t);
  bs->setLabelFont(labelsFont);
  bs->setLabelColor(Qt::black);
  bs->setParent(this);
  return bs;
}

void SalesInMonth::setMiniViewWidth(qreal i) {
  qreal _b = 0.5; // border line
  qreal _w = QFontMetricsF(font()).boundingRect("X000X").width();
  m_chart->setMinimumWidth((_w + _b) * (i * 12));
}

bool SalesInMonth::initMaps() {
  const QString _sql = AntiquaCRM::ASqlFiles::queryStatement("statistics_from_until_delivery_year");
  QSqlQuery _q = getSqlQuery(_sql);
  if (_q.size() < 1) {
    qWarning("Sales in Month chart, without ranges!");
    return false;
  }
  // Create existing years maps
  _q.next();
  const QDate _from = _q.value("min").toDate();
  const QDate _until = _q.value("max").toDate();
  for (int _y = _from.year(); _y <= _until.year(); _y++) {
    QMap<int, qint64> _vol;
    QMap<int, double> _sel;
    for (int m = 1; m < 13; m++) {
      _vol.insert(m, 0);
      _sel.insert(m, 0.00);
    }
    // qDebug() << Q_FUNC_INFO << _vol.size()  << _sel.size();
    p_voluMap.insert(_y, _vol);
    p_soldMap.insert(_y, _sel);
  }
  _q.clear();
  setMiniViewWidth(p_voluMap.size());
  return true;
}

bool SalesInMonth::initialChartView(int year) {
  Q_UNUSED(year);
  const QLocale _lc(QLocale::system());
  QString _query; // query statement
  // init months range map
  if (!initMaps())
    return false;

  // create volume chart data
  QSqlQuery _q = getTplSqlQuery("statistics_payments_month");
  if (_q.size() < 1)
    return false;

  while (_q.next()) {
    int _c = _q.value("counts").toInt();
    double _s = _q.value("sell").toDouble();
    const QDateTime _dt = getEpoch(_q.value("sepoch").toInt());
    int _y = getYear(_dt);
    int _m = getMonth(_dt);
    // verkäufe
    QMap<int, qint64> _vmap = p_voluMap[_y];
    _vmap[_m] += _c;
    p_voluMap[_y] = _vmap;
    // preise
    QMap<int, double> _smap = p_soldMap[_y];
    _smap[_m] += _s;
#ifdef ANTIQUA_DEVELOPMENT
    if (_y == p_date.year() && _m == p_date.month())
      qDebug() << _m << _s << _smap[_m];
#endif
    p_soldMap[_y] = _smap;
  }
  _query.clear();
  _q.clear();

  // finally insert chart data
  foreach (int y, p_voluMap.keys()) {
    QMap<int, qint64> _m = p_voluMap[y];
    MonthBarSet* m_counts = createBarset(y, MonthBarSet::Type::Volume);
    QMap<int, double> _s = p_soldMap[y];
    MonthBarSet* m_solded = createBarset(y, MonthBarSet::Type::Sales);
    m_solded->setSales(_s);
    for (int m = 1; m < 13; m++) {
      const QDate _d(y, m, 1);
      m_counts->append(_m[m]);
      m_solded->append(_s[m]);
      // @note label index begin with 0
      m_label->insert((m - 1), _lc.monthName(_d.month(), QLocale::LongFormat));
#ifdef ANTIQUA_DEVELOPMENT
      if (y == p_date.year() && m == p_date.month())
        qDebug() << _lc.monthName(_d.month(), QLocale::LongFormat) << _m[m] << _s[m];
#endif
    }
    m_numsBar->insert(0, m_counts);
    m_paidBar->insert(0, m_solded);
  }
  m_chart->addSeries(m_numsBar);
  m_chart->addSeries(m_paidBar);
  // updateHeight();
  return true;
}
