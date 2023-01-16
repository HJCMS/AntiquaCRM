// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerstatistics.h"

#include <AntiquaCRM>
#include <QMap>
#include <QSplitter>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

using namespace QtCharts;

ProviderStatistics::ProviderStatistics(const QDate &date, QWidget *parent)
    : QScrollArea{parent}, p_date{date} {
  setWidgetResizable(true);

  int _year = p_date.year();
  str_year = QString::number(_year);
  setWindowTitle(tr("Provider Orders in Year %1.").arg(str_year));

  m_widget = new QWidget(this);
  m_widget->setContentsMargins(0, 0, 0, 0);
  m_widget->setMinimumHeight(1000);

  m_layout = new QVBoxLayout(m_widget);
  m_layout->setContentsMargins(0, 0, 0, 0);

  m_sql = new AntiquaCRM::ASqlCore(this);

  setOrders();
  setCompleted();

  m_widget->setLayout(m_layout);
  setWidget(m_widget);
}

void ProviderStatistics::setOrders() {
  AntiquaCRM::ASqlFiles sqf("statistics_provider_orders_in_year");
  if (sqf.openTemplate()) {
    // Alle Aufträge
    QChartView *m_chartView = new QChartView(m_widget);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_layout->addWidget(m_chartView);

    QChart *m_chart = new QChart;
    m_chart->setTitle(tr("Orders in Year %1.").arg(str_year));

    QStringList query;
    query.append("date_part('year', o_since)=" + str_year);
    sqf.setWhereClause(query.join(" AND "));
    QSqlQuery q = m_sql->query(sqf.getQueryContent());
    if (q.size() > 0) {
      QPieSeries *m_series = new QPieSeries(m_chart);
      int summary = 0;
      while (q.next()) {
        int count = q.value("pr_count").toInt();
        QString name = q.value("pr_name").toString();
        name.append(" (");
        name.append(QString::number(count));
        name.append(")");
        m_series->append(name, count);
        summary += count;
      }
      // Labels setzen
      for (int s = 0; s < m_series->count(); s++) {
        QPieSlice *slice = m_series->slices().at(s);
        slice->setExplodeDistanceFactor(0.05);
        connect(slice, SIGNAL(hovered(bool)), SLOT(highlight(bool)));
        if (slice->value() > (summary / 100)) {
          slice->setLabelVisible();
        }
      }
      m_chart->addSeries(m_series);
      m_chartView->setChart(m_chart);
    }
  }
}

void ProviderStatistics::setCompleted() {
  AntiquaCRM::ASqlFiles sqf("statistics_provider_orders_in_year");
  if (sqf.openTemplate()) {
    // Bezahlte Aufträge
    QChartView *m_chartView = new QChartView(m_widget);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    m_layout->addWidget(m_chartView);

    QChart *m_chart = new QChart;
    m_chart->setTitle(tr("Completed in Year %1.").arg(str_year));

    QStringList query("o_order_status=4");
    query.append("o_payment_status=1");
    query.append("date_part('year', o_since)=" + str_year);
    sqf.setWhereClause(query.join(" AND "));
    QSqlQuery q = m_sql->query(sqf.getQueryContent());
    if (q.size() > 0) {
      int summary = 0;
      QPieSeries *m_series = new QPieSeries(m_chart);
      while (q.next()) {
        int count = q.value("pr_count").toInt();
        QString name = q.value("pr_name").toString();
        name.append(" (");
        name.append(QString::number(count));
        name.append(")");
        m_series->append(name, count);
        summary += count;
      }
      // Labels setzen
      for (int s = 0; s < m_series->count(); s++) {
        QPieSlice *slice = m_series->slices().at(s);
        slice->setExplodeDistanceFactor(0.05);
        connect(slice, SIGNAL(hovered(bool)), SLOT(highlight(bool)));
        if (slice->value() > (summary / 100)) {
          slice->setLabelVisible();
        }
      }
      m_chart->addSeries(m_series);
      m_chartView->setChart(m_chart);
    }
  }
}

void ProviderStatistics::highlight(bool b) {
  if (sender() != nullptr) {
    qobject_cast<QPieSlice *>(sender())->setExploded(b);
  }
}

void ProviderStatistics::resizeEvent(QResizeEvent *e) {
  m_widget->setMinimumHeight(e->size().width());
  QScrollArea::resizeEvent(e);
}
