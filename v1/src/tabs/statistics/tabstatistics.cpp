// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabstatistics.h"
#include "paymentsinyear.h"

#include <QDate>
#include <QDebug>

TabStatistics::TabStatistics(QWidget *parent)
    : Inventory{"tab_statistics", parent} {
  setWindowTitle(tr("Statistics"));
  setWindowIcon(getTabIcon("kchart"));
  setObjectName("antiqua_statistics_tab");
  setClosable(true);
}

void TabStatistics::popupWarningTabInEditMode() {
  // Unused
}

void TabStatistics::setDefaultTableView() {
  // Unused
}

void TabStatistics::openStartPage() {
  QDate d = QDate::currentDate();
  PaymentsInYear *m_view = new PaymentsInYear(d, this);
  insertWidget(0, m_view);
  firstview = true;
}

void TabStatistics::createSearchQuery(const QString &query) {
  qDebug() << Q_FUNC_INFO << "__TODO__" << query;
}

void TabStatistics::createNewEntry() {
  // Unused
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
