// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerpricevolume.h"

#include <AntiquaCRM>
#include <QMap>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

using namespace QtCharts;

ProviderPriceVolume::ProviderPriceVolume(const QDate &date, QWidget *parent)
    : QScrollArea{parent}, p_date{date} {
  setWidgetResizable(true);
  setWidgetResizable(true);
  int _year = p_date.year();
  str_year = QString::number(_year);
  setWindowTitle(tr("Provider Payment volume %1.").arg(str_year));
  m_chartView = new QChartView(this);
  m_chartView->setContentsMargins(0, 0, 0, 0);
  m_chartView->setRenderHint(QPainter::Antialiasing);
  setWidget(m_chartView);
  setVolumeChart();
}

void ProviderPriceVolume::setVolumeChart() {
  AntiquaCRM::ASqlFiles sqf("statistics_provider_volume_year");
  if (sqf.openTemplate()) {
    AntiquaCRM::ASqlCore p_sql(this);
    QChart *m_chart = new QChart;
    m_chart->setTitle(windowTitle());
    QStringList query("(o_order_status=4 OR o_order_status=6)");
    query.append("(o_payment_status=1 OR o_payment_status=4)");
    query.append("date_part('year', o_since)=" + str_year);
    sqf.setWhereClause(query.join(" AND "));
    QSqlQuery q = p_sql.query(sqf.getQueryContent());
    if (q.size() > 0) {
      QMap<QString, double> m;
      while (q.next()) {
        QString label = q.value("pr_name").toString();
        double price = q.value("pr_price").toDouble();
        if (m.contains(label)) {
          double fp = m.take(label);
          fp += price;
          m.insert(label, fp);
        } else {
          m.insert(label, price);
        }
      }
      // Darstellung erzeugen
      AntiquaCRM::ASettings cfg(this);
      QString currency = cfg.value("payment/currency", "$").toString();
      QPieSeries *m_series = new QPieSeries(m_chart);
      QMapIterator<QString, double> it(m);
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
      m_chart->addSeries(m_series);
      m_chartView->setChart(m_chart);
      m.clear();
    }
  }
}

void ProviderPriceVolume::highlight(bool b) {
  if (sender() != nullptr) {
    qobject_cast<QPieSlice *>(sender())->setExploded(b);
  }
}
