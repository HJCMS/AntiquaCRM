// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "salescategories.h"
#include "statsbarseries.h"

#include <QBarCategoryAxis>
#include <QDate>
#include <QPainter>

SalesCategories::SalesCategories(QWidget *parent)
    : AntiquaCRM::AChartView{parent} {
  setObjectName("categories_chart");
  p_year = QDate::currentDate().year();
  m_chart = new QChart(itemAt(0, 0));
  m_chart->setTitleFont(headersFont);
  m_chart->setTitle(windowTitle());
  m_chart->setMargins(contentsMargins());
  m_chart->setAnimationOptions(QChart::SeriesAnimations);

  m_countSeries = new HorizontalBarSeries(this);
  m_countSeries->setLabelsFormat("@value.");

  m_doubleSeries = new HorizontalBarSeries(this);
  m_doubleSeries->setLabelsFormat("@value " + currency);
}

QBarSet *SalesCategories::createBarSet(const QString &title, QChart *parent) {
  QBarSet *m_b = new QBarSet(title, parent);
  m_b->setLabelColor(Qt::black);
  m_b->setLabelFont(labelsFont);
  return m_b;
}

void SalesCategories::onHoverTip(bool status, int index) {
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

void SalesCategories::updateHeight(int rows) {
  QFontMetricsF fm(m_chart->font());
  setMinimumHeight(qRound(fm.height() * 3.15) * rows);
}

bool SalesCategories::initialChartView(int year) {
  p_year = year;

  const QString strYear(QString::number(year));
  QBarCategoryAxis *m_axisY = new QBarCategoryAxis(m_chart);
  QBarSet *m_quantity = createBarSet(tr("Quantity supplied"), m_chart);
  QBarSet *m_average = createBarSet(tr("Price average"), m_chart);
  QBarSet *m_volume = createBarSet(tr("Price volume"), m_chart);

  AntiquaCRM::ASqlFiles _tpl("statistics_payments_storage");
  if (_tpl.openTemplate()) {
    _tpl.setWhereClause(strYear);
    QSqlQuery _q = getSqlQuery(_tpl.getQueryContent());
    int _size = _q.size();
    if (_size > 0) {
      int _i = 0;
      while (_q.next()) {
        int _c = _q.value("counts").toInt();
        if (_c < 1)
          continue;

        m_quantity->append(_c);
        double _v = _q.value("volume").toDouble();
        m_average->append(roundPrice(_v / _c));
        m_volume->append(_v);
        m_axisY->insert(_i++, _q.value("sl_identifier").toString());
      }
      // polish
      _q.clear();

      updateHeight(_size);
      m_doubleSeries->insert(0, m_average);
      m_doubleSeries->insert(1, m_volume);
      m_countSeries->append(m_quantity);
      m_chart->addSeries(m_doubleSeries);
      m_chart->addSeries(m_countSeries);
      // Vertikale Achse
      m_chart->addAxis(m_axisY, Qt::AlignLeft);
      m_doubleSeries->attachAxis(m_axisY);
    }
    setChart(m_chart);

    connect(m_quantity, SIGNAL(hovered(bool, int)), // tip
            SLOT(onHoverTip(bool, int)));
    connect(m_average, SIGNAL(hovered(bool, int)), // tip
            SLOT(onHoverTip(bool, int)));
    connect(m_volume, SIGNAL(hovered(bool, int)), // tip
            SLOT(onHoverTip(bool, int)));

    return (_size > 0);
  }
  return false;
}
