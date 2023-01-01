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

  AntiquaCRM::ASettings cfg(this);
  int _year = p_date.year();
  currency = cfg.value("payment/currency", "§").toString();
  QString str_year = QString::number(_year);
  QString title(tr("Number of items sold by category in the year"));
  setWindowTitle(QString("%1 %2.").arg(title, str_year));

  m_view = new QChartView(this);
  m_view->setRenderHint(QPainter::Antialiasing);
  m_view->setMinimumHeight(1000);
  m_view->setContentsMargins(0, 0, 0, 0);
  setWidget(m_view);

  QChart *m_chart = new QChart;
  m_chart->setTitle(windowTitle());
  m_chart->legend()->hide();
  m_chart->setContentsMargins(0, 0, 0, 0);
  m_chart->setAnimationOptions(QChart::SeriesAnimations);

  QBarCategoryAxis *m_axisY = new QBarCategoryAxis(m_chart);
  m_axisY->setLineVisible(false);
  m_axisY->setGridLineVisible(true);

  QFont barFont = m_axisY->labelsFont();
  barFont.setPointSize(m_axisY->labelsFont().pointSize() - 4);

  AntiquaCRM::ASqlFiles sqf("statistics_payments_storage");
  if (sqf.openTemplate()) {
    QHorizontalBarSeries *catbar_series = new QHorizontalBarSeries(m_chart);
    catbar_series->setName(tr("Categories"));
    catbar_series->setLabelsFormat("@value");
    catbar_series->setLabelsPosition(QAbstractBarSeries::LabelsCenter);
    catbar_series->setLabelsVisible(true);

    QBarSet *m_barCrowd = new QBarSet(tr("Quantity"), catbar_series);
    m_barCrowd->setObjectName("quantity_bar");
    m_barCrowd->setLabelFont(barFont);
    connect(m_barCrowd, SIGNAL(hovered(bool, int)),
            SLOT(onHoverTip(bool, int)));

    QHorizontalBarSeries *volbar_series = new QHorizontalBarSeries(m_chart);
    volbar_series->setName(tr("Volumes"));
    volbar_series->setLabelsFormat("@value " + currency);
    volbar_series->setLabelsPosition(QAbstractBarSeries::LabelsCenter);
    volbar_series->setLabelsVisible(true);

    QBarSet *m_barVolume = new QBarSet(tr("Volume"), volbar_series);
    m_barVolume->setObjectName("volume_bar");
    m_barVolume->setLabelFont(barFont);
    connect(m_barVolume, SIGNAL(hovered(bool, int)),
            SLOT(onHoverTip(bool, int)));

    sqf.setWhereClause(str_year);
    AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
    QSqlQuery q = m_sql->query(sqf.getQueryContent());
    int size = q.size();
    if (size > 0) {
      int index = 0;
      while (q.next()) {
        int count = q.value("counts").toInt();
        if (count < 1)
          continue;

        m_barCrowd->insert(index, count);
        m_barVolume->insert(index, q.value("volume").toDouble());
        m_axisY->insert(index, q.value("sl_identifier").toString());
        index++;
      }
      catbar_series->insert(0, m_barCrowd);
      volbar_series->insert(1, m_barVolume);
      m_chart->addSeries(catbar_series);
      m_chart->addSeries(volbar_series);
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

void CategoriesInYear::onHoverTip(bool status, int index) {
  if (!status)
    return;

  QBarSet *m_bar = qobject_cast<QBarSet *>(sender());
  if (m_bar == nullptr)
    return;

  qreal value = m_bar->at(index);
  QString tip;
  if (m_bar->objectName() == "quantity_bar") {
    tip = QString::number(value);
    tip.append(" " + tr("Quantity"));
  } else if (m_bar->objectName() == "volume_bar") {
    tip = QString::number(value, 'f', 2);
    tip.append(" " + currency);
  } else {
    qWarning("Missing Tip object: %s", qPrintable(tip));
    return;
  }
  setToolTip(tip);
}

CategoriesInYear::~CategoriesInYear() {
  if (m_view != nullptr)
    m_view->deleteLater();
}
