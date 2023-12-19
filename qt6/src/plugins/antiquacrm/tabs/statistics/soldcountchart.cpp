// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "soldcountchart.h"

SoldCountChart::SoldCountChart(QWidget *parent)
    : AntiquaCRM::AChartView{parent} {
  m_chart = new QChart(itemAt(0, 0));
  m_chart->setMargins(contentsMargins());
  m_chart->setAnimationOptions(QChart::SeriesAnimations);
  m_chart->setTitleFont(headersFont);
}

void SoldCountChart::highlight(bool b) {
  if (sender() != nullptr) {
    qobject_cast<QPieSlice *>(sender())->setExploded(b);
  }
}

bool SoldCountChart::initialChartView(int year) {
  const QString _strYear = QString::number(year);
  AntiquaCRM::ASqlFiles _tpl("statistics_provider_orders_year");
  if (_tpl.openTemplate()) {
    _tpl.setWhereClause(_strYear);
    QSqlQuery _q = getSqlQuery(_tpl.getQueryContent());
    if (_q.size() > 0) {
      int summary = 0;
      QPieSeries *m_series = initSeries(m_chart);
      while (_q.next()) {
        int _c = _q.value("pr_count").toInt();
        QString _lb = _q.value("pr_name").toString();
        _lb.append(" (");
        _lb.append(QString::number(_c));
        _lb.append(")");
        m_series->append(_lb, _c);
        summary += _c;
      }
      // polish
      _q.clear();

      // Labels setzen
      for (int s = 0; s < m_series->count(); s++) {
        QPieSlice *_slice = m_series->slices().at(s);
        _slice->setLabelFont(labelsFont);
        _slice->setExplodeDistanceFactor(0.05);
        connect(_slice, SIGNAL(hovered(bool)), SLOT(highlight(bool)));
        if (_slice->value() > (summary / 100)) {
          _slice->setLabelVisible();
        }
      }
      m_chart->addSeries(m_series);
      m_chart->setTitle(tr("Sold in %1").arg(year));
      setChart(m_chart);
      return true;
    }
  }
  return false;
}
