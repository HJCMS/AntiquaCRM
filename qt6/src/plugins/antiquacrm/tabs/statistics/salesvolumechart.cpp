// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "salesvolumechart.h"

SalesVolumeChart::SalesVolumeChart(QWidget *parent)
    : AntiquaCRM::AChartView{parent} {
  m_chart = new QChart(itemAt(0, 0));
  m_chart->setMargins(contentsMargins());
  m_chart->setAnimationOptions(QChart::SeriesAnimations);
  m_chart->setTitleFont(headersFont);
}

void SalesVolumeChart::highlight(bool b) {
  if (sender() != nullptr) {
    qobject_cast<QPieSlice *>(sender())->setExploded(b);
  }
}

bool SalesVolumeChart::initialChartView(int year) {
  const QString _strYear = QString::number(year);
  AntiquaCRM::ASqlFiles _tpl("statistics_provider_volume_year");
  if (_tpl.openTemplate()) {
    _tpl.setWhereClause(_strYear);
    QSqlQuery _q = getSqlQuery(_tpl.getQueryContent());
    if (_q.size() > 0) {
      QMap<QString, double> _map;
      while (_q.next()) {
        QString _lb = _q.value("pr_name").toString();
        double price = _q.value("pr_price").toDouble();
        if (_map.contains(_lb)) {
          double fp = _map.take(_lb);
          fp += price;
          _map.insert(_lb, fp);
        } else {
          _map.insert(_lb, price);
        }
      }
      // polish
      _q.clear();

      // Darstellung erzeugen
      QPieSeries *m_series = initSeries(m_chart);
      QMapIterator<QString, double> it(_map);
      while (it.hasNext()) {
        it.next();
        double vol = it.value();
        QString name(it.key());
        name.append(" (");
        name.append(QString::number(vol, 'f', 2));
        name.append(" " + currency + ")");
        m_series->append(name, vol);
      }
      // Labels setzen
      for (int s = 0; s < m_series->count(); s++) {
        QPieSlice *_slice = m_series->slices().at(s);
        _slice->setLabelFont(labelsFont);
        _slice->setExplodeDistanceFactor(0.05);
        connect(_slice, SIGNAL(hovered(bool)), SLOT(highlight(bool)));
        _slice->setLabelVisible();
      }
      m_chart->addSeries(m_series);
      m_chart->setTitle(tr("Sales volume in %1").arg(year));
      setChart(m_chart);
      _map.clear();
      return true;
    }
  }
  return false;
}
