// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "achartview.h"

#include <QFontMetricsF>
#include <cmath>

namespace AntiquaCRM {

AChartView::AChartView(QWidget *parent) : QChartView{parent} {
  setContentsMargins(0, 0, 0, 0);
  setRenderHint(QPainter::Antialiasing);

  m_sql = new AntiquaCRM::ASqlCore(this);
  AntiquaCRM::ASettings cfg(this);
  currency = cfg.value("payment/currency", "§").toString();
  // Font Settings
  const QString _fb = font().toString();
  cfg.beginGroup("statistics");
  headersFont.fromString(cfg.value("stats_font_header", _fb).toString());
  labelsFont.fromString(cfg.value("stats_font_chart", _fb).toString());
  cfg.endGroup();
}

QPieSeries *AChartView::initSeries(QChart *parent) const {
  QPieSeries *m_s = new QPieSeries(parent);
  m_s->setVisible(false); // no header labels
  return m_s;
}

const QDateTime AChartView::getEpoch(qint64 ct) const {
  return QDateTime::fromSecsSinceEpoch(ct, Qt::LocalTime);
}

int AChartView::getYear(const QDateTime &dt) const { return dt.date().year(); }

int AChartView::getMonth(const QDateTime &dt) const {
  return dt.date().month();
}

const QSqlQuery AChartView::getSqlQuery(const QString &query) {
  return m_sql->query(query);
}

const QSqlQuery AChartView::getTplSqlQuery(const QString &tpl) {
  QString _query = AntiquaCRM::ASqlFiles::queryStatement(tpl);
  if (_query.isEmpty()) {
    qWarning("Can't open SQL template file '%s'", qPrintable(tpl));
    return QSqlQuery();
  }
  return m_sql->query(_query);
}

double AChartView::roundPrice(double value) const {
  double _precision = 0.01;
  return std::round(value / _precision) * _precision;
}

AChartView::~AChartView() {
  if (m_sql != nullptr)
    m_sql->deleteLater();
}

} // namespace AntiquaCRM
