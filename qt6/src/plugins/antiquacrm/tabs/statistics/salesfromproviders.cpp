// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "salesfromproviders.h"

#include <QLayout>

SalesFromProviders::SalesFromProviders(QWidget *parent) : QWidget{parent} {
  setObjectName("providers_chart");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_sql = new AntiquaCRM::ASqlCore(this);

  m_averageView = new QChartView(this);
  m_averageView->setContentsMargins(contentsMargins());
  m_averageView->setRenderHint(QPainter::Antialiasing);
  m_averageChart = new QChart(m_averageView->itemAt(0, 0));
  m_averageChart->setMargins(m_averageView->contentsMargins());
  m_averageChart->setAnimationOptions(QChart::SeriesAnimations);
  layout->addWidget(m_averageView);

  m_volumeView = new QChartView(this);
  m_volumeView->setContentsMargins(contentsMargins());
  m_volumeView->setRenderHint(QPainter::Antialiasing);
  m_volumeChart = new QChart(m_volumeView->itemAt(0, 0));
  m_volumeChart->setMargins(m_averageView->contentsMargins());
  m_volumeChart->setAnimationOptions(QChart::SeriesAnimations);
  layout->addWidget(m_volumeView);

  setLayout(layout);
}

SalesFromProviders::~SalesFromProviders() {
  if (m_sql != nullptr)
    m_sql->deleteLater();
}

QPieSeries *SalesFromProviders::initSeries(QChart *chart) {
  QPieSeries *m_s = new QPieSeries(chart);
  m_s->setVisible(false); // no header labels
  return m_s;
}

void SalesFromProviders::highlight(bool b) {
  if (sender() != nullptr) {
    qobject_cast<QPieSlice *>(sender())->setExploded(b);
  }
}

bool SalesFromProviders::initChartView(int year) {
  bool _status = false;
  if (year < 2000)
    return _status;

  const QString _strYear = QString::number(year);
  // Average
  AntiquaCRM::ASqlFiles _tpA("statistics_provider_orders_year");
  if (_tpA.openTemplate()) {
    QStringList _query("o_order_status=4");
    _query.append("o_payment_status=1");
    _query.append("date_part('year', o_since)=" + _strYear);
    _tpA.setWhereClause(_query.join(" AND "));
    QSqlQuery _q = m_sql->query(_tpA.getQueryContent());
    if (_q.size() > 0) {
      int summary = 0;
      QPieSeries *m_series = initSeries(m_averageChart);
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
      _query.clear();
      _q.clear();

      // Labels setzen
      for (int s = 0; s < m_series->count(); s++) {
        QPieSlice *_slice = m_series->slices().at(s);
        _slice->setExplodeDistanceFactor(0.05);
        connect(_slice, SIGNAL(hovered(bool)), SLOT(highlight(bool)));
        if (_slice->value() > (summary / 100)) {
          _slice->setLabelVisible();
        }
      }
      m_averageChart->addSeries(m_series);
      m_averageChart->setTitle(tr("Average in %1").arg(year));
      m_averageView->setChart(m_averageChart);
      _status = true;
    } else {
      _status = false;
    }
  }

  // Volume
  AntiquaCRM::ASqlFiles _tpV("statistics_provider_volume_year");
  if (_tpV.openTemplate()) {
    QStringList _query("(o_order_status=4 OR o_order_status=6)");
    _query.append("(o_payment_status=1 OR o_payment_status=4)");
    _query.append("date_part('year', o_since)=" + _strYear);
    _tpV.setWhereClause(_query.join(" AND "));
    QSqlQuery _q = m_sql->query(_tpV.getQueryContent());
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
      _query.clear();
      _q.clear();

      // Darstellung erzeugen
      AntiquaCRM::ASettings cfg(this);
      QString currency = cfg.value("payment/currency", "$").toString();
      QPieSeries *m_series = initSeries(m_volumeChart);
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
        QPieSlice *slice = m_series->slices().at(s);
        slice->setExplodeDistanceFactor(0.05);
        connect(slice, SIGNAL(hovered(bool)), SLOT(highlight(bool)));
        slice->setLabelVisible();
      }
      m_volumeChart->addSeries(m_series);
      m_volumeChart->setTitle(tr("Volume in %1").arg(year));
      m_volumeView->setChart(m_volumeChart);
      _map.clear();
      _status = true;
    } else {
      _status = false;
    }
  }

  return _status;
}
