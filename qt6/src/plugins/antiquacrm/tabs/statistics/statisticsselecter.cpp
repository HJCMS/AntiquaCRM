// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statisticsselecter.h"

#include <QDate>

StatisticsSelecter::StatisticsSelecter(QWidget *parent) : QToolBar{parent} {
  setObjectName("statistics_selecter");
  setLayoutDirection(Qt::RightToLeft);

  // WARNING: Direction changed!
  const QIcon _icon = AntiquaCRM::antiquaIcon("office-chart-area");
  QDate _dt = QDate::currentDate();

  m_button = new QPushButton(tr("Display selection"), this);
  m_button->setLayoutDirection(Qt::LeftToRight);
  m_button->setIcon(AntiquaCRM::antiquaIcon("view-search"));
  addWidget(m_button);
  addSeparator();

  m_date = new AntiquaCRM::ASpinBox(this);
  m_date->setLayoutDirection(Qt::LeftToRight);
  m_date->setToolTip(tr("Year"));
  m_date->setRange(_dt.year() - 2, _dt.year());
  m_date->setValue(_dt.year());
  addWidget(m_date);
  addSeparator();

  int _i = 0;
  m_box = new AntiquaCRM::AComboBox(this);
  m_box->setLayoutDirection(Qt::LeftToRight);
  m_box->insertItem(_i, tr("Sales in Month"));
  m_box->setItemData(_i++, _icon, Qt::DecorationRole);
  m_box->insertItem(_i, tr("Daily average"));
  m_box->setItemData(_i++, _icon, Qt::DecorationRole);
  m_box->insertItem(_i, tr("Categories sales"));
  m_box->setItemData(_i++, _icon, Qt::DecorationRole);
  m_box->insertItem(_i, tr("Provider sales"));
  m_box->setItemData(_i++, _icon, Qt::DecorationRole);
  addWidget(m_box);

  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(chartSelected(int)));
  connect(m_button, SIGNAL(clicked()), SLOT(updateView()));
}

void StatisticsSelecter::chartSelected(int index) {
  m_date->setEnabled((index > 0));
}

void StatisticsSelecter::updateView() {
  QString _chart;
  switch (m_box->currentIndex()) {
  case 0:
    _chart = QString("monthly_sales");
    break;

  case 1:
    _chart = QString("daily_average");
    break;

  case 2:
    _chart = QString("categories_sales");
    break;

  default:
    _chart = QString("provider_sales");
    break;
  };

  emit sendChartSelected(m_date->value(), _chart);
}
