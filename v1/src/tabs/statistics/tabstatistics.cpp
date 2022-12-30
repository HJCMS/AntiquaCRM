// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabstatistics.h"
#include "categoriesinyear.h"
#include "paymentsinyear.h"

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

  QDate date = QDate::currentDate();
  AntiquaCRM::ASettings cfg(this);
  int year = cfg.value("statistics_year", 2022).toInt();
  p_statsDate = QDate(year, date.month(), date.day());
}

void TabStatistics::openPaymentsInYear(const QDate &d) {
  PaymentsInYear *m_view = new PaymentsInYear(d, this);
  insertWidget(0, m_view);
  setCurrentIndex(0);
}

void TabStatistics::openCategoriesInYear(const QDate &d) {
  CategoriesInYear *m_view = new CategoriesInYear(d, this);
  insertWidget(1, m_view);
  setCurrentIndex(1);
}

void TabStatistics::popupWarningTabInEditMode() {}

void TabStatistics::setDefaultTableView() {}

void TabStatistics::openStartPage() {}

void TabStatistics::createSearchQuery(const QString &query) {
  firstview = true;
  if (query == "PaymentsInYear") {
    openPaymentsInYear(p_statsDate);
  } else if (query == "CategoriesInYear") {
    openCategoriesInYear(p_statsDate);
  }
#ifdef ANTIQUA_DEVELOPEMENT
   else {
    qDebug() << Q_FUNC_INFO << "NOT FOUND" << query;
  }
#endif
}

void TabStatistics::openEntry(qint64) {}

void TabStatistics::onEnterChanged() {}

bool TabStatistics::customAction(const QJsonObject &obj) {
  Q_UNUSED(obj);
  return false;
}
