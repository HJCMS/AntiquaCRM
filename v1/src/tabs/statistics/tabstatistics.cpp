// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabstatistics.h"
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
}

void TabStatistics::openPaymentsInYear(const QDate &d) {
  PaymentsInYear *m_view = new PaymentsInYear(d, this);
  insertWidget(0, m_view);
  setCurrentIndex(0);
}

void TabStatistics::popupWarningTabInEditMode() {
  // Unused
}

void TabStatistics::setDefaultTableView() {
  // Unused
}

void TabStatistics::openStartPage() {
  firstview = true;
  openPaymentsInYear(QDate::currentDate());
}

void TabStatistics::createSearchQuery(const QString &query) {
  qDebug() << Q_FUNC_INFO << "__TODO__" << query;
}

void TabStatistics::openEntry(qint64 statsId) {
  qDebug() << Q_FUNC_INFO << "__TODO__" << statsId;
}

void TabStatistics::onEnterChanged() {
  if (firstview)
    return;

  openStartPage();
}

bool TabStatistics::customAction(const QJsonObject &obj) {
  qDebug() << Q_FUNC_INFO << "__TODO__" << obj;
  return false;
}
