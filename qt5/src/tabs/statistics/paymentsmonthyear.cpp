// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "paymentsmonthyear.h"
#include "ahorizontalbarseries.h"

#include <QCursor>
#include <QLayout>
#include <QToolTip>
#include <cmath>

MonthBarSet::MonthBarSet(int year, QtCharts::QChart *parent)
    : QtCharts::QBarSet{QString::number(year), parent}, p_year{year} {
  setLabelColor(Qt::black);
  connect(this, SIGNAL(hovered(bool, int)), SLOT(showToolTip(bool, int)));
}

void MonthBarSet::showToolTip(bool b, int i) {
  int _m = (i + 1); // month
  if (b) {
    double _money = p_sales[_m];
    if (_money > 0.00) {
      const QString _info =
          tr("Article sale %1").arg(AntiquaCRM::AUtil::toMoney(_money));
      QWidget *_parent = qobject_cast<QWidget *>(parent());
      QToolTip::showText(QCursor::pos(), _info, _parent);
      return;
    }
  }
  QToolTip::hideText();
}

void MonthBarSet::setSales(const QMap<int, double> &sales) { p_sales = sales; }

int MonthBarSet::year() const { return p_year; }

PaymentsMonthYear::PaymentsMonthYear(const QDate &date, QWidget *parent)
    : QScrollArea{parent}, p_date(date) {
  setWidgetResizable(true);
  setWindowTitle(tr("Sales in Month"));

  AntiquaCRM::ASettings cfg(this);
  p_currency = cfg.value("payment/currency", "§").toString();

  m_view = new QChartView(this);
  m_view->setRenderHint(QPainter::Antialiasing);
  setWidget(m_view);

  m_chart = new QtCharts::QChart;
  m_chart->setTitle(tr("Sales from past to current year."));
  m_chart->setMargins(QMargins(0, 0, 0, 0));
  m_chart->setAnimationOptions(QChart::SeriesAnimations);

  m_label = new QBarCategoryAxis(m_chart);
  QFont barFont = m_label->labelsFont();
  barFont.setPointSize(m_label->labelsFont().pointSize() - 4);

  m_barSeries = new AHorizontalBarSeries(this);
  m_barSeries->setBarWidth(0.95);

  m_sql = new AntiquaCRM::ASqlCore(this);
  if (initialChartView()) {
    // Vertikale Achse
    m_chart->addAxis(m_label, Qt::AlignLeft);
    m_barSeries->attachAxis(m_label);
    m_view->setChart(m_chart);
  } else {
    qWarning("No Sales in Month Charts data");
  }
}

bool PaymentsMonthYear::initMaps() {
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

inline const QDateTime PaymentsMonthYear::fsepoch(qint64 e) const {
  return QDateTime::fromSecsSinceEpoch(e, Qt::LocalTime);
}

MonthBarSet *PaymentsMonthYear::createBarset(int y) {
  MonthBarSet *bs = new MonthBarSet(y, m_chart);
  bs->setObjectName("average_curr_year");
  bs->setLabelFont(m_label->labelsFont());
  return bs;
}

bool PaymentsMonthYear::initialChartView() {
  if (!initMaps())
    return false;

  QString _sql =
      AntiquaCRM::ASqlFiles::queryStatement("statistics_payments_month");
  if (_sql.isEmpty())
    return false;

  QSqlQuery _q = m_sql->query(_sql);
  if (_q.size() < 1)
    return false;

  while (_q.next()) {
    int _c = _q.value("counts").toInt();
    double _s = _q.value("sell").toInt();
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
  _q.clear();

  foreach (int y, p_voluMap.keys()) {
    QMap<int, qint64> _vm = p_voluMap[y];
    MonthBarSet *m_barSet = createBarset(y);
    m_barSet->setSales(p_soldMap[y]);
    for (int m = 1; m <= 12; m++) {
      QDate _curr(y, m, 1);
      m_barSet->insert(m, _vm[m]);
      m_label->insert((m - 1), _curr.toString("MMMM"));
      // qDebug() << _curr.toString("MMMM") << _m[m];
    }
    m_barSeries->insert(0, m_barSet);
  }
  m_chart->addSeries(m_barSeries);
  return true;
}

PaymentsMonthYear::~PaymentsMonthYear() {
  if (m_view != nullptr)
    m_view->deleteLater();
}
