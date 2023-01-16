// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabstatistics.h"
#include "categoriesinyear.h"
#include "paymentsinyear.h"
#include "providerstatistics.h"
#include "statsmainpage.h"

#include <QComboBox>
#include <QDebug>
#include <QLabel>
#include <QLayout>

TabStatistics::TabStatistics(QWidget *parent)
    : Inventory{"tab_statistics", parent} {
  setWindowTitle(tr("Statistics"));
  setWindowIcon(getTabIcon("kchart"));
  setObjectName("antiqua_statistics_tab");
  setClosable(true);

  m_mainPage = new StatsMainPage(this);
  m_mainPage->setWindowTitle(windowTitle());
  insertWidget(0, m_mainPage);

  connect(m_mainPage, SIGNAL(sendOpenChart(const QString &, const QDate &)),
          SLOT(openChartView(const QString &, const QDate &)));
}

void TabStatistics::openChartView(const QString &name, const QDate &date) {
  if (name.contains("default_page")) {
    setCurrentIndex(0);
    return;
  }

  if (count() > 1) {
    // qDebug() << "Clear Chart:" << name << date;
    for (int i = 0; i < count(); ++i) {
      if (i == 0)
        continue;

      QWidget *w = widget(i);
      if (w != nullptr) {
        removeWidget(w);
      }
    }
  }
  // Anzahl der Bestellungen pro Tag im Jahr
  QStringList countChart({"PaymentsLastYear", "DailyOrdersInYear"});
  // Anzahl der Bestellten Kategorien im Jahr
  QStringList barChart({"CategoriesLastYear", "SelledCategoriesInYear"});
  // Dienstleister Bestellungen im Jahr
  QStringList providerChart({"ProviderOrdersInYear", "PrOrdersLastYear"});
  int index = count();
  if (countChart.contains(name)) {
    insertWidget(index, new PaymentsInYear(date, this));
  } else if (barChart.contains(name)) {
    insertWidget(index, new CategoriesInYear(date, this));
  } else if (providerChart.contains(name)) {
    insertWidget(index, new ProviderStatistics(date, this));
  } else {
    qDebug() << "No Chart found:" << name << date;
  }

  if (!initialed)
    initialed = true;

  setCurrentIndex(index);
}

void TabStatistics::openStartPage() {
  if (currentIndex() != 0)
    setCurrentIndex(0);

  initialed = true;
}

void TabStatistics::createSearchQuery(const QString &query) {
  // Einträge aus StatisticsActionGroup
  if (query.contains("LastYear")) {
    QDate d = QDate::currentDate();
    openChartView(query, d.addYears(-1));
  } else {
    openChartView(query, m_mainPage->selectedDate());
  }
}

void TabStatistics::onEnterChanged() {
  if (!initialed)
    openStartPage();
}

bool TabStatistics::customAction(const QJsonObject &obj) {
  Q_UNUSED(obj);
  return false;
}

TabStatistics::~TabStatistics() {
  for (int i = 0; i < count(); ++i) {
    QWidget *w = widget(i);
    if (w != nullptr) {
      removeWidget(w);
    }
  }
}
