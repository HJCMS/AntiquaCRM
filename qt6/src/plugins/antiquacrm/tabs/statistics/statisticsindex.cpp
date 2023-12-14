// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statisticsindex.h"
#include "salesinmonth.h"
#include "statisticsselecter.h"

StatisticsIndex::StatisticsIndex(QWidget *parent)
    : AntiquaCRM::TabsIndex{"statistics_tab", parent} {
  setObjectName("statistics_tab_widget");
  setWindowIcon(AntiquaCRM::antiquaIcon("office-chart-area"));
  setWindowTitle(getTitle());
  setClosable(true);

  // Begin MainPage layout
  QWidget *m_mainPage = new QWidget(this);
  QVBoxLayout *m_p1Layout = new QVBoxLayout(m_mainPage);
  m_p1Layout->setContentsMargins(0, 0, 0, 0);
  m_area = new QScrollArea(m_mainPage);
  m_area->setWidgetResizable(true);
  m_p1Layout->addWidget(m_area);
  m_p1Layout->setStretch(0, 1);

  m_selecter = new StatisticsSelecter(m_mainPage);
  m_p1Layout->addWidget(m_selecter);

  m_mainPage->setLayout(m_p1Layout);
  insertWidget(0, m_mainPage);
  // End

  setCurrentIndex(ViewPage::MainView);

  connect(m_selecter, SIGNAL(sendChartSelected(qint64, const QString &)),
          SLOT(setChart(qint64, const QString &)));
}

void StatisticsIndex::setDefaultTableView() {
  if (m_area->widget() == nullptr)
    return;

  m_area->widget()->deleteLater();
}

void StatisticsIndex::setChart(qint64 year, const QString &chart) {
  Q_UNUSED(year);
  setDefaultTableView();
  if (chart == "monthly_sales") {
    SalesInMonth *m_c = new SalesInMonth(m_area);
    m_area->setWidget(m_c);
  } else if (chart == "daily_average") {
  } else if (chart == "categories_sales") {
  } else if (chart == "provider_sales") {
  }
}

void StatisticsIndex::openStartPage() {}

void StatisticsIndex::createSearchQuery(const QString &) {}

void StatisticsIndex::createNewEntry() {}

void StatisticsIndex::openEntry(qint64) {}

void StatisticsIndex::onEnterChanged() {}

const QString StatisticsIndex::getTitle() const { return tr("Statistics"); }

bool StatisticsIndex::customAction(const QJsonObject &) { return false; }

const QStringList StatisticsIndex::acceptsCustomActions() const {
  return QStringList();
}
