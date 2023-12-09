// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "categoriesinyear.h"
#include "ahorizontalbarseries.h"

#include <AntiquaCRM>
#include <QFontMetricsF>
#include <QMap>
#include <QMargins>
#include <QVBoxLayout>
#include <QtCharts>
#include <cmath>

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
  m_chart->setMargins(QMargins(0, 0, 0, 0));
  m_chart->setAnimationOptions(QChart::SeriesAnimations);

  QBarCategoryAxis *m_axisY = new QBarCategoryAxis(m_chart);

  QFont barFont = m_axisY->labelsFont();
  barFont.setPointSize(m_axisY->labelsFont().pointSize() - 4);

  m_countSeries = new AHorizontalBarSeries(this);
  m_countSeries->setLabelsFormat("@value.");

  m_doubleSeries = new AHorizontalBarSeries(this);
  m_doubleSeries->setLabelsFormat("@value " + currency);

  QBarSet *m_quantity = new QBarSet(tr("Quantity supplied"), m_chart);
  m_quantity->setLabelFont(barFont);
  m_quantity->setLabelColor(Qt::black);

  QBarSet *m_average = new QBarSet(tr("Price average"), m_chart);
  m_average->setObjectName("average_bar");
  m_average->setLabelFont(barFont);
  m_average->setLabelColor(Qt::black);

  QBarSet *m_volume = new QBarSet(tr("Price volume"), m_chart);
  m_volume->setObjectName("volume_bar");
  m_volume->setLabelFont(barFont);
  m_volume->setLabelColor(Qt::black);

  AntiquaCRM::ASqlFiles sqf("statistics_payments_storage");
  if (sqf.openTemplate()) {
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

        m_quantity->append(count);
        double volume = q.value("volume").toDouble();
        m_average->append(currencyRound(volume / count));
        m_volume->append(volume);
        m_axisY->insert(index++, q.value("sl_identifier").toString());
      }
      m_doubleSeries->insert(0, m_average);
      m_doubleSeries->insert(1, m_volume);
      m_countSeries->append(m_quantity);
      m_chart->addSeries(m_doubleSeries);
      m_chart->addSeries(m_countSeries);
      // Vertikale Achse
      m_chart->addAxis(m_axisY, Qt::AlignLeft);
      m_doubleSeries->attachAxis(m_axisY);
    }
    // Update height
    QFontMetricsF fm(m_axisY->labelsFont());
    m_view->setMinimumHeight(qRound(fm.height() * 3.15) * size);
  }
  m_view->setChart(m_chart);

  connect(m_quantity, SIGNAL(hovered(bool, int)), SLOT(onHoverTip(bool, int)));
  connect(m_average, SIGNAL(hovered(bool, int)), SLOT(onHoverTip(bool, int)));
  connect(m_volume, SIGNAL(hovered(bool, int)), SLOT(onHoverTip(bool, int)));
}

double CategoriesInYear::currencyRound(double d) {
  double precision = 0.01;
  return std::round(d / precision) * precision;
}

void CategoriesInYear::onHoverTip(bool status, int index) {
  if (!status) {
    setToolTip(QString());
    return;
  }

  QBarSet *m_bar = qobject_cast<QBarSet *>(sender());
  if (m_bar == nullptr)
    return;

  qreal value = m_bar->at(index);
  QString tip = m_bar->label();
  QStringList currencyBar({"average_bar", "volume_bar"});
  if (currencyBar.contains(m_bar->objectName())) {
    tip.append(" " + QString::number(value, 'f', 2));
    tip.append(" " + currency);
  } else {
    tip.append(" " + QString::number(value));
  }
  setToolTip(tip);
}

CategoriesInYear::~CategoriesInYear() {
  if (m_view != nullptr)
    m_view->deleteLater();
}
