// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statisticsselecter.h"

#include <QDate>

StatisticsSelecter::StatisticsSelecter(QWidget *parent) : QToolBar{parent} {
  setObjectName("statistics_selecter");

  const QIcon _icon = AntiquaCRM::antiquaIcon("office-chart-area");

  QDate _dt = QDate::currentDate();
  m_date = new AntiquaCRM::ASpinBox(this);
  m_date->setToolTip(tr("Year"));
  m_date->setRange(_dt.year() - 2, _dt.year());
  m_date->setValue(_dt.year());
  addWidget(m_date);
  addSeparator();

  int _i = 0;
  m_box = new AntiquaCRM::AComboBox(this);
  m_box->insertItem(_i, tr("Select a chart"), QString());
  m_box->setItemData(_i++, _icon, Qt::DecorationRole);
  m_box->insertItem(_i, tr("Sales in Month"), QString("monthly_sales"));
  m_box->setItemData(_i++, _icon, Qt::DecorationRole);
  m_box->insertItem(_i, tr("Daily average"), QString("daily_average"));
  m_box->setItemData(_i++, _icon, Qt::DecorationRole);
  m_box->insertItem(_i, tr("Categories sales"), QString("categories_sales"));
  m_box->setItemData(_i++, _icon, Qt::DecorationRole);
  m_box->insertItem(_i, tr("Provider sales"), QString("provider_sales"));
  m_box->setItemData(_i++, _icon, Qt::DecorationRole);
  addWidget(m_box);

  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(chartSelected(int)));
}

void StatisticsSelecter::chartSelected(int index) {
  const QString _chart = m_box->itemData(index, Qt::UserRole).toString();
  emit sendChartSelected(m_date->value(), _chart);
}
