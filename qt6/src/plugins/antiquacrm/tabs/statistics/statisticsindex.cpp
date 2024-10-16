﻿// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statisticsindex.h"
#include "dailyaverage.h"
#include "salescategories.h"
#include "salesfromproviders.h"
#include "salesinmonth.h"
#include "statisticsselecter.h"
#include "statisticsviewarea.h"

StatisticsIndex::StatisticsIndex(QWidget* parent)
    : AntiquaCRM::TabsIndex{"statistics_tab", parent}, p_date{QDate::currentDate()} {
  setObjectName("statistics_tab_widget");
  setWindowIcon(AntiquaCRM::antiquaIcon("office-chart-area"));
  setWindowTitle(getTitle());
  setClosable(true);

  // Begin MainPage layout
  QWidget* m_mainPage = new QWidget(this);
  m_mainPage->setContentsMargins(0, 0, 0, 0);

  QVBoxLayout* m_p1Layout = new QVBoxLayout(m_mainPage);
  m_p1Layout->setContentsMargins(m_mainPage->contentsMargins());
  m_view = new StatisticsViewArea(m_mainPage);
  m_view->setContentsMargins(m_mainPage->contentsMargins());
  m_p1Layout->addWidget(m_view);
  m_p1Layout->setStretch(0, 1);

  m_selecter = new StatisticsSelecter(m_mainPage);
  m_p1Layout->addWidget(m_selecter);

  m_mainPage->setLayout(m_p1Layout);
  insertWidget(0, m_mainPage);
  // End

  setCurrentIndex(ViewPage::MainView);

  connect(m_selecter, SIGNAL(sendChartSelected(qint64,QString)), SLOT(setChart(qint64,QString)));
}

void StatisticsIndex::setDefaultTableView() {
  if (m_view->widget() == nullptr)
    return;

  m_view->widget()->deleteLater();
}

void StatisticsIndex::setChart(qint64 year, const QString& chart) {
  setDefaultTableView(); // cleanup

  if (chart == "monthly_sales") {
    SalesInMonth* m_c = new SalesInMonth(m_view);
    m_view->setWidget(m_c);
    return;
  } else if (chart == "daily_average") {
    DailyAverage* m_c = new DailyAverage(m_view);
    if (m_c->initialChartView(year)) {
      m_view->setWidget(m_c);
    } else {
      m_c->deleteLater();
    }
    return;
  } else if (chart == "categories_sales") {
    SalesCategories* m_c = new SalesCategories(m_view);
    if (m_c->initialChartView(year)) {
      m_view->setWidget(m_c);
    } else {
      m_c->deleteLater();
    }
    return;
  } else if (chart == "provider_sales") {
    SalesFromProviders* m_c = new SalesFromProviders(this);
    if (m_c->initView(year)) {
      m_view->setWidget(m_c);
    } else {
      m_c->deleteLater();
    }
    return;
  }
  qInfo("Unknown chart operation.");
}

void StatisticsIndex::onEnterChanged() {
}

const QString StatisticsIndex::getTitle() const {
  return tr("Statistics");
}

bool StatisticsIndex::customAction(const QJsonObject&) {
  return false;
}

const QStringList StatisticsIndex::acceptsCustomActions() const {
  return QStringList();
}
