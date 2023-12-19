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
  m_box->insertItem(_i, tr("Sales in Month"), QString("monthly_sales"));
  m_box->setItemData(_i++, _icon, Qt::DecorationRole);
  m_box->insertItem(_i, tr("Daily average"), QString("daily_average"));
  m_box->setItemData(_i++, _icon, Qt::DecorationRole);
  m_box->insertItem(_i, tr("Categories sales"), QString("categories_sales"));
  m_box->setItemData(_i++, _icon, Qt::DecorationRole);
  m_box->insertItem(_i, tr("Provider sales"), QString("provider_sales"));
  m_box->setItemData(_i++, _icon, Qt::DecorationRole);
  addWidget(m_box);

  m_button = new QPushButton(tr("Display selection"), this);
  m_button->setIcon(_icon);
  addWidget(m_button);

  connect(m_button, SIGNAL(clicked()), SLOT(updateView()));
}

void StatisticsSelecter::updateView() {
  int _index = m_box->currentIndex();
  const QString _chart = m_box->itemData(_index, Qt::UserRole).toString();
  emit sendChartSelected(m_date->value(), _chart);
}
