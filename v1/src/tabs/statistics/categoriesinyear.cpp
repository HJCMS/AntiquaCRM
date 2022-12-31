// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "categoriesinyear.h"

#include <AntiquaCRM>
#include <QFontMetricsF>
#include <QMap>
#include <QVBoxLayout>
#include <QtCharts>

using namespace QtCharts;

CategoriesInYear::CategoriesInYear(const QDate &date, QWidget *parent)
    : QScrollArea{parent}, p_date{date} {
  setWidgetResizable(true);

  int _year = p_date.year();
  QString str_year = QString::number(_year);
  QString title(tr("Number of items sold by category in the year"));
  setWindowTitle(QString("%1 %2.").arg(title, str_year));

  m_view = new QChartView(this);
  m_view->setRenderHint(QPainter::Antialiasing);
  m_view->setMinimumHeight(1000);
  setWidget(m_view);

  QChart *m_chart = new QChart;
  m_chart->setTitle(windowTitle());
  m_chart->legend()->hide();
  m_chart->createDefaultAxes();
  m_chart->setAnimationOptions(QChart::SeriesAnimations);

  AntiquaCRM::ASqlFiles sqf("statistics_payments_storage");
  if (sqf.openTemplate()) {
    QBarCategoryAxis *m_axisY = new QBarCategoryAxis(m_chart);
    m_axisY->setLineVisible(false);
    m_axisY->setGridLineVisible(false);

    QHorizontalBarSeries *catbar_series = new QHorizontalBarSeries(m_chart);
    catbar_series->setName(tr("Categories"));
    catbar_series->setLabelsPosition(QAbstractBarSeries::LabelsCenter);
    catbar_series->setLabelsFormat("@value");
    catbar_series->setLabelsVisible(true);

    sqf.setWhereClause(str_year);
    AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
    QSqlQuery q = m_sql->query(sqf.getQueryContent());
    int size = q.size();
    if (size > 0) {
      QBarSet *m_barset = new QBarSet(tr("Category"), catbar_series);
      while (q.next()) {
        int count = q.value("counts").toInt();
        if (count < 1)
          continue;

        m_barset->append(count);
        m_axisY->append(q.value("sl_identifier").toString());
      }
      catbar_series->append(m_barset);
      m_chart->addSeries(catbar_series);
      // Vertikale Achse
      m_chart->addAxis(m_axisY, Qt::AlignLeft);
      catbar_series->attachAxis(m_axisY);
    }
    // Update height
    QFontMetricsF fm(m_axisY->labelsFont());
    m_view->setMinimumHeight(qRound(fm.height() * 1.95) * size);
  }
  m_view->setChart(m_chart);
}

CategoriesInYear::~CategoriesInYear() {
  if (m_view != nullptr)
    m_view->deleteLater();
}
