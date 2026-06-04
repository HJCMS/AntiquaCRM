// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "salesinmonth.h"

#include <QFontMetricsF>
#include <QSqlQuery>
#include <QToolTip>

// #define DEBUG_SALES_IN_MONTH

MonthSeries::MonthSeries(qint64 year, QMap<qint16, qint64> map, QObject* parent)
    : QLineSeries{parent}, Year{year}, MonthData{map} {
  setName(QString::number(year));
  setPointsVisible(true);
  setPointLabelsFormat("@yPoint");
  setPointLabelsVisible(true);
  setPointLabelsClipping(true);
  setVisible(true);

  /* A line series used as an edge series for QAreaSeries.
   * For this behavior it cannot use OpenGL acceleration!
   */
  setUseOpenGL(false);

  connect(this, SIGNAL(hovered(QPointF,bool)), SLOT(toolTip(QPointF,bool)));
}

bool MonthSeries::labelIsVisible(const QPointF &p) const {
  return (p.x() > 0 && p.y() > 0 && p.x() < 12);
}

void MonthSeries::toolTip(const QPointF& p, bool b) {
  if (b && !p.isNull()) {
    QLocale _lc;
    qint16 _m = qRound(p.x());
    QDate _d(Year, _m, 1);
    const QString _info = tr("%1 %2 (%3)")
                              .arg(name())
                              .arg(_lc.monthName(_d.month(), QLocale::ShortFormat), _d.month())
                              .arg(MonthData[_m]);
    QToolTip::showText(QCursor::pos(), _info);
    return;
  }
  QToolTip::hideText();
}

void MonthSeries::updatePointLabels() {
  QHash<QXYSeries::PointConfiguration, QVariant> cfg;
  cfg[QXYSeries::PointConfiguration::Visibility] = true;
  cfg[QXYSeries::PointConfiguration::LabelVisibility] = true;
  QListIterator<QPointF> it(points());
  qint8 _index = 0;
  while (it.hasNext()) {
    const QPointF _p = it.next();
    cfg[QXYSeries::PointConfiguration::LabelVisibility] = labelIsVisible(_p);
    setPointConfiguration(_index++, cfg);
  }
}

bool MonthSeries::setPoints() {
  if (MonthData.size() < 12)
    return false;

  const QMap<qint16, qint64> _map(MonthData);

  qint64 _year = 0;
  foreach (qint64 v, _map.values()) {
    _year += v;
  }
  setName(QString::number(Year) + " (" + QString::number(_year) + ")");

  for (int m = 1; m <= 12; m++) {
    const QDate _d(Year, m, 1);
    // hide this and create a horizontal line to next month
    if (m == 0)
      continue;

    qint64 _v = _map[m];
    if (m == 1) // first month counts
      append(QPoint(0, _v));

    append(QPoint(m, _v));
  }
  return true;
}

QAbstractSeries::SeriesType MonthSeries::type() const {
  return QAbstractSeries::SeriesTypeLine;
}

SalesInMonth::SalesInMonth(QWidget* parent)
    : AntiquaCRM::AChartView{parent}, p_lc{QLocale::system()}, p_date{QDate::currentDate()} {
  setObjectName("statistics_sales_in_month");
  m_chart = new QChart(mainItem());
  m_chart->setTitleFont(headersFont);
  m_chart->setTitle(tr("Compare sales from past years with current."));
  m_chart->setMargins(QMargins(5, 0, 5, 0));
  m_chart->setAnimationOptions(QChart::NoAnimation);

  m_valueAxis = new QValueAxis(m_chart);
  m_valueAxis->setMin(0);
  m_valueAxis->setMax(100);
  m_chart->addAxis(m_valueAxis, Qt::AlignLeft);

  m_monthsAxis = new QCategoryAxis(m_chart);
  m_monthsAxis->setObjectName("MonthsAxis");
  m_monthsAxis->setLabelsFont(labelsFont);
  m_monthsAxis->setMin(0);
  m_monthsAxis->setMax(12);
  for (int m = 1; m < 13; m++) {
    const QDate _d(QDate::currentDate().year(), m, 1);
    m_monthsAxis->append(p_lc.monthName(_d.month(), QLocale::LongFormat), _d.month());
  }
  m_chart->addAxis(m_monthsAxis, Qt::AlignBottom);

  if (initialChartView()) {
    setChart(m_chart);
  } else {
    qWarning("No Sales in Month Charts data");
  }
}

const QList<MonthSeries*> SalesInMonth::series() {
  QList<MonthSeries*> _list;
  QListIterator<QAbstractSeries*> it(m_chart->series());
  while (it.hasNext()) {
    MonthSeries* m_s = static_cast<MonthSeries*>(it.next());
    if (m_s != nullptr)
      _list.append(m_s);
  }
  return _list;
}

SalesInMonth::~SalesInMonth() {
  if (m_chart != nullptr)
    m_chart->deleteLater();
}

bool SalesInMonth::initMaps() {
  QString _sql = AntiquaCRM::ASqlFiles::queryStatement("statistics_from_until_delivery_year");
  QSqlQuery _q = getSqlQuery(_sql);
  if (_q.size() < 1) {
    qWarning("Sales in Month chart, without ranges!");
    return false;
  }
  // Create existing years maps
  _q.next();
  const QDate _from = _q.value("min").toDate();
  const QDate _until = _q.value("max").toDate();
  for (qint64 _y = _from.year(); _y <= _until.year(); _y++) {
    QMap<qint16, qint64> _vol;
    for (qint16 m = 1; m < 13; m++) {
      _vol.insert(m, 0);
    }
#ifdef DEBUG_SALES_IN_MONTH
    qDebug() << Q_FUNC_INFO << _vol.size();
#endif
    p_dataMap.insert(_y, _vol);
  }
  _q.clear();

  return true;
}

bool SalesInMonth::initialChartView(int year) {
  Q_UNUSED(year);
  QString _query; // query statement
  // init months range map
  if (!initMaps())
    return false;

  // create volume chart data
  QSqlQuery _q = getTplSqlQuery("statistics_payments_month");
  if (_q.size() < 1)
    return false;

  while (_q.next()) {
    qint64 _c = _q.value("counts").toInt();
    const QDateTime _dt = getEpoch(_q.value("sepoch").toInt());
    qint64 _y = getYear(_dt);
    qint16 _m = getMonth(_dt);
    QMap<qint16, qint64> _vmap = p_dataMap[_y];
    _vmap[_m] += _c;
    p_dataMap[_y] = _vmap;
  }
  _query.clear();
  _q.clear();

  if(p_dataMap.keys().size() < 1)
    return false;

  // finally insert chart data
  foreach (int _y, p_dataMap.keys()) {
    MonthSeries* m_series = new MonthSeries(_y, p_dataMap[_y], this);
    m_series->setObjectName("obj_" + QString::number(_y));
    m_series->setPointLabelsFont(headersFont);
    if (!m_series->setPoints()) {
      qWarning("Missing data for year %d.", _y);
      break;
    }
    m_chart->addSeries(m_series);
    // note we need attach when add multible lines
    m_valueAxis->setTickCount(10);
    m_series->attachAxis(m_valueAxis);
    // update label visibilities
    m_series->updatePointLabels();
#ifdef DEBUG_SALES_IN_MONTH
    qDebug() << "Year:" << _y << Qt::endl << "Points:" << m_series->points();
#endif
  }

  return true;
}
