// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "categoriesinyear.h"

#include <AntiquaCRM>
#include <QMap>
#include <QVBoxLayout>
#include <QtCharts>

using namespace QtCharts;

CategoriesInYear::CategoriesInYear(const QDate &date, QWidget *parent)
    : QScrollArea{parent}, p_date{date} {
  setObjectName("payments_from_year");
  setWidgetResizable(true);

  int min_count = 0;
  int max_count = 40;
  int _year = p_date.year();
  QString str_year = QString::number(_year);
  setWindowTitle(tr("Categories sold per year %1.").arg(str_year));

  QChartView *m_chartView = new QChartView(this);
  m_chartView->setRenderHint(QPainter::Antialiasing);
  m_chartView->setMinimumHeight(2500);
  setWidget(m_chartView);

  QChart *m_chart = new QChart;
  m_chart->setTitle(windowTitle());
  m_chart->legend()->hide();
  m_chart->setAnimationOptions(QChart::SeriesAnimations);

  AntiquaCRM::ASqlFiles sqf("statistics_payments_storage");
  if (sqf.openTemplate()) {
    QBarCategoryAxis *m_axisY = new QBarCategoryAxis(m_chart);
    m_axisY->setLabelsVisible(true);
    m_axisY->setVisible(true);
    m_axisY->setGridLineVisible(false);

    QFont lbf = m_axisY->labelsFont();
    lbf.setPointSize(lbf.pointSize() - 2);
    m_axisY->setLabelsFont(lbf);

    QHorizontalBarSeries *catbar_series = new QHorizontalBarSeries(m_chart);
    catbar_series->setName(tr("Categories"));
    catbar_series->setLabelsPosition(QAbstractBarSeries::LabelsCenter);

    sqf.setWhereClause(str_year);
    AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
    QSqlQuery q = m_sql->query(sqf.getQueryContent());
    if (q.size() > 0) {
      QBarSet *m_barset = new QBarSet(tr("Category"), catbar_series);
      while (q.next()) {
        int count = q.value("counts").toInt();
        if (count < min_count)
          continue;

        max_count = (count > max_count) ? count : max_count;
        m_barset->append(count);

        QString label(q.value("sl_identifier").toString());
        label.append(" (" + QString::number(count) + ")");
        m_axisY->append(label);
      }
      catbar_series->append(m_barset);
      m_chart->addSeries(catbar_series);

      // Vertikale Achse
      m_chart->addAxis(m_axisY, Qt::AlignLeft);
      catbar_series->attachAxis(m_axisY);
    }
  }
  m_chartView->setChart(m_chart);
}
